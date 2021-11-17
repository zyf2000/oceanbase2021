/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2021/5/13.
//

#include <string>
#include <sstream>
#include <limits.h>
#include <string.h>
#include <algorithm>
#include <list>

#include "storage/common/table.h"
#include "storage/common/table_meta.h"
#include "common/log/log.h"
#include "common/manual.h"
#include "common/lang/string.h"
#include "storage/default/disk_buffer_pool.h"
#include "storage/common/record_manager.h"
#include "storage/common/condition_filter.h"
#include "storage/common/meta_util.h"
#include "storage/common/index.h"
#include "storage/common/bplus_tree_index.h"
#include "storage/trx/trx.h"

Table::Table() :
    data_buffer_pool_(nullptr),
    file_id_(-1),
    record_handler_(nullptr)
{
}

Table::~Table()
{
    delete record_handler_;
    record_handler_ = nullptr;

    if (data_buffer_pool_ != nullptr && file_id_ >= 0)
    {
        data_buffer_pool_->close_file(file_id_);
        data_buffer_pool_ = nullptr;
    }

    LOG_INFO("Table has been closed: %s", name());
}

RC Table::create(const char *path, const char *name, const char *base_dir, int attribute_count, const AttrInfo attributes[])
{

    if (nullptr == name || common::is_blank(name))
    {
        LOG_WARN("Name cannot be empty");
        return RC::INVALID_ARGUMENT;
    }
    LOG_INFO("Begin to create table %s:%s", base_dir, name);

    if (attribute_count <= 0 || nullptr == attributes)
    {
        LOG_WARN("Invalid arguments. table_name=%s, attribute_count=%d, attributes=%p",
                 name, attribute_count, attributes);
        return RC::INVALID_ARGUMENT;
    }

    RC rc = RC::SUCCESS;

    // 使用 table_name.table记录一个表的元数据
    // 判断表文件是否已经存在

    int fd = ::open(path, O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0600);
    if (-1 == fd)
    {
        if (EEXIST == errno)
        {
            LOG_ERROR("Failed to create table file, it has been created. %s, EEXIST, %s",
                      path, strerror(errno));
            return RC::SCHEMA_TABLE_EXIST;
        }
        LOG_ERROR("Create table file failed. filename=%s, errmsg=%d:%s",
                  path, errno, strerror(errno));
        return RC::IOERR;
    }

    close(fd);

    // 创建文件
    if ((rc = table_meta_.init(name, attribute_count, attributes)) != RC::SUCCESS)
    {
        LOG_ERROR("Failed to init table meta. name:%s, ret:%d", name, rc);
        return rc; // delete table file
    }

    std::fstream fs;
    fs.open(path, std::ios_base::out | std::ios_base::binary);
    if (!fs.is_open())
    {
        LOG_ERROR("Failed to open file for write. file name=%s, errmsg=%s", path, strerror(errno));
        return RC::IOERR;
    }

    // 记录元数据到文件中
    table_meta_.serialize(fs);
    fs.close();

    std::string data_file = std::string(base_dir) + "/" + name + TABLE_DATA_SUFFIX;
    data_buffer_pool_ = theGlobalDiskBufferPool();
    rc = data_buffer_pool_->create_file(data_file.c_str());
    if (rc != RC::SUCCESS)
    {
        printf(COLOR_RED "[ERROR] " COLOR_YELLOW "Failed to create disk buffer pool of data file. file name="
                COLOR_GREEN "%s" COLOR_YELLOW ".\n", data_file.c_str());
        return rc;
    }

    std::string data_text_file = std::string(base_dir) + "/" + name + TABLE_TEXT_SUFFIX;
    rc = data_buffer_pool_->create_file(data_text_file.c_str());
    if (rc != RC::SUCCESS)
    {
        printf(COLOR_RED "[ERROR] " COLOR_YELLOW "Failed to create disk buffer pool of data file. file name="
                COLOR_GREEN "%s" COLOR_YELLOW ".\n", data_file.c_str());
        return rc;
    }

    rc = init_record_handler(base_dir);

    base_dir_ = base_dir;
    printf(COLOR_WHITE "[INFO] " COLOR_YELLOW "Successfully create table "
            COLOR_GREEN "%s" COLOR_YELLOW":" COLOR_GREEN "%s" COLOR_YELLOW ".\n", base_dir, name);
    return rc;
}

RC Table::create_virtual_table(const char *name, int attribute_count, const AttrInfo attributes[])
{
    printf(COLOR_WHITE "[INFO] " COLOR_YELLOW "Create virtual table named: " COLOR_GREEN "%s", name);
    if (nullptr == name || common::is_blank(name))
    {
        printf(COLOR_RED "[ERROR] Failed to create virtual table, name is not exist!\n");
        return RC::INVALID_ARGUMENT;
    }
    if (attribute_count <= 0 || nullptr == attributes)
    {
        printf(COLOR_RED "[ERROR] Failed to create virtual table, invalid argument\n");
        return RC::INVALID_ARGUMENT;
    }
    RC rc = RC::SUCCESS;
        // 创建文件
    if ((rc = table_meta_.init(name, attribute_count, attributes)) != RC::SUCCESS)
        printf(COLOR_RED "[ERROR] Failed to init table meta. " COLOR_YELLOW "name:" COLOR_GREEN "%s"
                COLOR_YELLOW ", ret:" COLOR_GREEN "%d", name, rc);
    fflush(stdout);

    return rc;
}

RC Table::drop()
{
   /* printf(COLOR_WHITE "[INFO] " COLOR_YELLOW "Drop base dir: "
           COLOR_GREEN "%s" COLOR_YELLOW ".\n", this->base_dir_.c_str());*/

    /// Close Buffer Pool
    if (this->data_buffer_pool_ != nullptr
        && this->file_id_ >= 0)
    {
        this->data_buffer_pool_->close_file(this->file_id_);
        this->data_buffer_pool_ = nullptr;
        this->file_id_ = -1;
    }

    /// Remove files on disk
    std::string prefix = this->base_dir_ + "/" + this->table_meta().name();
    if (remove((prefix + TABLE_INDEX_SUFFIX).c_str()) != 0)
    {
        if (errno == ENOENT) ;
        /*printf(COLOR_WHITE "[INFO] "
                                    COLOR_YELLOW "Index file does not exist\n"), fflush(stdout);*/
        else return RC::GENERIC_ERROR;
    }
    if (remove((prefix + TABLE_META_SUFFIX).c_str()) != 0) return RC::GENERIC_ERROR;
    if (remove((prefix + TABLE_DATA_SUFFIX).c_str()) != 0) return RC::GENERIC_ERROR;
    fflush(stdout);
    return RC::SUCCESS;
}


RC Table::open(const char *meta_file, const char *base_dir)
{
    // 加载元数据文件
    std::fstream fs;
    std::string meta_file_path = std::string(base_dir) + "/" + meta_file;
    fs.open(meta_file_path, std::ios_base::in | std::ios_base::binary);
    if (!fs.is_open())
    {
        LOG_ERROR("Failed to open meta file for read. file name=%s, errmsg=%s", meta_file, strerror(errno));
        return RC::IOERR;
    }
    if (table_meta_.deserialize(fs) < 0)
    {
        LOG_ERROR("Failed to deserialize table meta. file name=%s", meta_file);
        return RC::GENERIC_ERROR;
    }
    fs.close();

    // 加载数据文件
    RC rc = init_record_handler(base_dir);

    base_dir_ = base_dir;

    const int index_num = table_meta_.index_num();
    for (int i = 0; i < index_num; i++)
    {
        const IndexMeta *index_meta = table_meta_.index(i);
        const FieldMeta *field_meta = table_meta_.field(index_meta->field());
        if (field_meta == nullptr)
        {
            LOG_PANIC("Found invalid index meta info which has a non-exists field. table=%s, index=%s, field=%s",
                      name(), index_meta->name(), index_meta->field());
            return RC::GENERIC_ERROR;
        }

        BplusTreeIndex *index = new BplusTreeIndex();
        std::string index_file = index_data_file(base_dir, name(), index_meta->name());
        rc = index->open(index_file.c_str(), *index_meta, *field_meta);
        if (rc != RC::SUCCESS)
        {
            delete index;
            LOG_ERROR("Failed to open index. table=%s, index=%s, file=%s, rc=%d:%s",
                      name(), index_meta->name(), index_file.c_str(), rc, strrc(rc));
            return rc;
        }
        indexes_.push_back(index);
    }
    return rc;
}

RC Table::commit_insert(Trx *trx, const RID &rid)
{
    Record record;
    RC rc = record_handler_->get_record(&rid, &record);
    if (rc != RC::SUCCESS)
    {
        return rc;
    }

    return trx->commit_insert(this, record);
}

RC Table::rollback_insert(Trx *trx, const RID &rid)
{

    Record record;
    RC rc = record_handler_->get_record(&rid, &record);
    if (rc != RC::SUCCESS)
    {
        return rc;
    }

    // remove all indexes
    rc = delete_entry_of_indexes(record.data, rid, false);
    if (rc != RC::SUCCESS)
    {
        LOG_ERROR("Failed to delete indexes of record(rid=%d.%d) while rollback insert, rc=%d:%s",
                  rid.page_num, rid.slot_num, rc, strrc(rc));
    } else
    {
        rc = record_handler_->delete_record(&rid);
    }
    return rc;
}

RC Table::insert_record(Trx *trx, Record *record)
{
    RC rc = RC::SUCCESS;

    // Transaction initialization.
    if (trx != nullptr)
    {
        trx->init_trx_info(this, *record);
    }

    /// Perform insertion
    rc = record_handler_->insert_record(record->data, table_meta_.record_size(), &record->rid);
    if (rc != RC::SUCCESS)
    {
        printf( COLOR_RED "[ERROR] " COLOR_YELLOW "Insert record failed. table name=" COLOR_GREEN "%s" COLOR_YELLOW ", rc="
                    COLOR_GREEN "%d" COLOR_YELLOW ":" COLOR_GREEN "%s" COLOR_YELLOW ".\n", table_meta_.name(), rc, strrc(rc));
        return rc;
    }

    /// Transaction logging
    if (trx != nullptr)
    {
        rc = trx->insert_record(this, record);
        if (rc != RC::SUCCESS)
        {
            LOG_ERROR("Failed to log operation(insertion) to trx");

            RC rc2 = record_handler_->delete_record(&record->rid);
            if (rc2 != RC::SUCCESS)
            {
                LOG_PANIC("Failed to rollback record data when insert index entries failed. table name=%s, rc=%d:%s",
                          name(), rc2, strrc(rc2));
            }
            return rc;
        }
    }

    /// Indexing
    rc = insert_entry_of_indexes(record->data, record->rid);
    if (rc != RC::SUCCESS)
    {
        RC rc2 = delete_entry_of_indexes(record->data, record->rid, true);
        if (rc2 != RC::SUCCESS)
        {
            LOG_PANIC("Failed to rollback index data when insert index entries failed. table name=%s, rc=%d:%s",
                      name(), rc2, strrc(rc2));
        }
        rc2 = record_handler_->delete_record(&record->rid);
        if (rc2 != RC::SUCCESS)
        {
            LOG_PANIC("Failed to rollback record data when insert index entries failed. table name=%s, rc=%d:%s",
                      name(), rc2, strrc(rc2));
        }
        return rc;
    }
    return rc;
}

/// <summary> Calling entry point
/// </summary>
/// <param name="trx"> Transactions </param>
/// <param name="value_num"> number of values to be
/// inserted</param>
/// <param name="values">value array</param>
/// <returns> Return Code </returns>
RC Table::insert_record(Trx *trx, int value_num, const Value *values)
{
    if (value_num <= 0 || nullptr == values)
    {
        LOG_ERROR("Invalid argument. value num=%d, values=%p", value_num, values);
        return RC::INVALID_ARGUMENT;
    }

    char *record_data;
    RC rc = make_record(value_num, values, record_data);
    /// Make [record_data] to be a bit-array of this record to add.
    if (rc != RC::SUCCESS)
    {
        printf(COLOR_RED "[ERROR] "
                COLOR_YELLOW "Failed to create a record. rc=" COLOR_GREEN "%d" COLOR_YELLOW ":" COLOR_GREEN "%s" COLOR_YELLOW ".\n",
                rc, strrc(rc));
        return rc;
    }

    Record record;
    record.data = record_data;
    // record.valid = true;
    rc = insert_record(trx, &record);
    delete[] record_data;
    return rc;
}

RC Table::insert_record_tuples(Trx *trx, int insert_tuple_num, const InsertTuple *insert_tuples)
{
    // if (value_num <= 0 || nullptr == values)
    // {
    //     LOG_ERROR("Invalid argument. value num=%d, values=%p", value_num, values);
    //     return RC::INVALID_ARGUMENT;
    // }
    if (insert_tuple_num <= 0 || nullptr == insert_tuples)
    {
        printf(COLOR_RED "[ERROR] "
                COLOR_YELLOW "Invalid argument. insert tuple num="
               COLOR_GREEN "%d" COLOR_YELLOW ", insert tuples="
               COLOR_GREEN "%p" COLOR_YELLOW ".\n",
                insert_tuple_num, insert_tuples);
        return RC::INVALID_ARGUMENT;
    }

    RC rc = RC::SUCCESS;
    std::vector<const RID*> rid_array;
    for (int i = 0; i < insert_tuple_num; ++i)
    {
        int value_num = insert_tuples[i].value_num;
        const Value *values = insert_tuples[i].values;

        if (value_num <= 0 || nullptr == values)
        {
            printf(COLOR_RED "[ERROR] "
                COLOR_YELLOW "Invalid argument. value num=" COLOR_GREEN "%d" 
                   COLOR_YELLOW ", values="
                   COLOR_GREEN "%p" COLOR_YELLOW ".\n",
                value_num, values);
            break;
        }
        
        char *record_data;
        rc = make_record(value_num, values, record_data);
        if (rc != RC::SUCCESS)
        {
            printf(COLOR_RED "[ERROR] "
                COLOR_YELLOW "Failed to create a record. rc=" COLOR_GREEN "%d" COLOR_YELLOW ":" COLOR_GREEN "%s" COLOR_YELLOW ".\n",
                rc, strrc(rc));
            break;
        }
        
        Record record;
        record.data = record_data;
        rc = insert_record(trx, &record);
        if (rc == RC::SUCCESS)
            rid_array.push_back(&record.rid);
        // delete[] record_data;
        if (rc != RC::SUCCESS)
            break;
    }
    if (rc != RC::SUCCESS)
    {
        for(auto it : rid_array)
        {
            RC ret = record_handler_->delete_record(it);
            assert(ret == RC::SUCCESS);
        }
    }
    return rc;
}

const char* Table::name() const
{
    return table_meta_.name();
}

const TableMeta& Table::table_meta() const
{
    return table_meta_;
}
RecordFileHandler* Table::record_handler()
{
    return record_handler_;
}
RecordFileHandler* Table::text_handler()
{
    return text_handler_;
}

RC Table::make_record(int value_num, const Value *values, char *&record_out)
{
    // 检查字段类型是否一致
    if (value_num + table_meta_.sys_field_num() != table_meta_.field_num())
    {
        return RC::SCHEMA_FIELD_MISSING;
    }

    const int normal_field_start_index = table_meta_.sys_field_num();

    /// Type Checking
    for (int i = 0; i < value_num; i++)
    {
        const FieldMeta *field = table_meta_.field(i + normal_field_start_index);
        const Value &value = values[i];
        if (field->type() != value.type && !(field->nullable() == 1 && value.type == NULLS)
            && !(field->type() == TEXTS && value.type == CHARS))
        {
            printf(COLOR_RED "[ERROR] " COLOR_YELLOW "Invalid value type. field name="
                    COLOR_GREEN "%s" COLOR_YELLOW ", type="
                    COLOR_GREEN "%d" COLOR_YELLOW ", but given="
                    COLOR_GREEN "%d" COLOR_YELLOW ".\n",
                      field->name(), field->type(), value.type);
            return RC::SCHEMA_FIELD_TYPE_MISMATCH;
        }
        if (value.type == DATES)
        {
            RC ret = check_dates(&value);
            if (ret != RC::SUCCESS)
            {
                printf(COLOR_RED "[ERROR] Invalid dates!\n");
                return RC::INVALID_ARGUMENT;
            }
        }
    }

    // 复制所有字段的值
    int record_size = table_meta_.record_size();
    char *record = new char[record_size];

    for (int i = 0; i < value_num; i++)
    {
        const FieldMeta *field = table_meta_.field(i + normal_field_start_index);
        const Value &value = values[i];
        if (field->type() == TEXTS)
        {
            Record *record_text = new Record;
            record_text->data = (char*)value.data;
            RC rc = text_handler_->insert_record(record_text->data, 4096, &record_text->rid);
            memcpy(record + field->offset(), &record_text->rid, field->len());
        }
        else
            memcpy(record + field->offset(), value.data, field->len());
    }
    record_out = record;
    return RC::SUCCESS;
}

RC Table::init_record_handler(const char *base_dir)
{
    std::string data_file = std::string(base_dir) + "/" + table_meta_.name() + TABLE_DATA_SUFFIX;
    std::string text_file = std::string(base_dir) + "/" + table_meta_.name() + TABLE_TEXT_SUFFIX;
    if (nullptr == data_buffer_pool_)
    {
        data_buffer_pool_ = theGlobalDiskBufferPool();
    }
    RC rc = RC::SUCCESS;

    int data_buffer_pool_file_id;
    rc = data_buffer_pool_->open_file(data_file.c_str(), &data_buffer_pool_file_id);
    if (rc != RC::SUCCESS)
    {
        printf( COLOR_RED "[ERROR] " COLOR_YELLOW "Failed to open disk buffer pool for file:"
                COLOR_GREEN "%s" COLOR_YELLOW ". rc=" COLOR_GREEN "%d" COLOR_YELLOW ":" COLOR_GREEN "%s"
                COLOR_YELLOW ".\n", data_file.c_str(), rc, strrc(rc));
        return rc;
    }

    int data_buffer_pool_text_file_id;
    rc = data_buffer_pool_->open_file(text_file.c_str(), &data_buffer_pool_text_file_id);
    if (rc != RC::SUCCESS)
    {
        printf( COLOR_RED "[ERROR] " COLOR_YELLOW "Failed to open disk buffer pool for file:"
                COLOR_GREEN "%s" COLOR_YELLOW ". rc=" COLOR_GREEN "%d" COLOR_YELLOW ":" COLOR_GREEN "%s"
                COLOR_YELLOW ".\n", text_file.c_str(), rc, strrc(rc));
        return rc;
    }

    record_handler_ = new RecordFileHandler();
    rc = record_handler_->init(*data_buffer_pool_, data_buffer_pool_file_id);
    if (rc != RC::SUCCESS)
    {
        printf( COLOR_RED "[ERROR] " COLOR_YELLOW "Failed to init record handler. rc="
                COLOR_GREEN "%d" COLOR_YELLOW ":" COLOR_GREEN "%s" COLOR_YELLOW ".\n", rc, strrc(rc));
        return rc;
    }

    text_handler_ = new RecordFileHandler();
    rc = text_handler_->init(*data_buffer_pool_, data_buffer_pool_text_file_id);
    if (rc != RC::SUCCESS)
    {
        printf( COLOR_RED "[ERROR] " COLOR_YELLOW "Failed to init text handler. rc="
                COLOR_GREEN "%d" COLOR_YELLOW ":" COLOR_GREEN "%s" COLOR_YELLOW ".\n", rc, strrc(rc));
        return rc;
    }

    file_id_ = data_buffer_pool_file_id;
    text_file_id_ = data_buffer_pool_text_file_id;
    return rc;
}

/**
 * 为了不把Record暴露出去，封装一下
 */
class RecordReaderScanAdapter
{
public:
    explicit RecordReaderScanAdapter(void (*record_reader)(const char *data, void *context), void *context)
        : record_reader_(record_reader), context_(context)
    {
    }

    void consume(const Record *record)
    {
        record_reader_(record->data, context_);
    }
private:
    void (*record_reader_)(const char *, void *);
    void *context_;
};
static RC scan_record_reader_adapter(Record *record, void *context)
{
    RecordReaderScanAdapter &adapter = *(RecordReaderScanAdapter *)context;
    adapter.consume(record);
    return RC::SUCCESS;
}

RC Table::scan_record(Trx *trx, ConditionFilter *filter, int limit, void *context, void (*record_reader)(const char *data, void *context))
{
    RecordReaderScanAdapter adapter(record_reader, context);
    return scan_record(trx, filter, limit, (void *)&adapter, scan_record_reader_adapter);
}

RC Table::scan_record(Trx *trx, ConditionFilter *filter, int limit, void *context, RC(*record_reader)(Record *record, void *context))
{
    if (nullptr == record_reader)
    {
        return RC::INVALID_ARGUMENT;
    }

    if (0 == limit)
    {
        return RC::SUCCESS;
    }

    if (limit < 0)
    {
        limit = INT_MAX;
    }

    // IndexScanner *index_scanner = find_index_for_scan(filter);
    // if (index_scanner != nullptr)
    // {
    //     return scan_record_by_index(trx, index_scanner, filter, limit, context, record_reader);
    // }

    RC rc = RC::SUCCESS;
    RecordFileScanner scanner;
    rc = scanner.open_scan(*data_buffer_pool_, file_id_, filter);
    if (rc != RC::SUCCESS)
    {
        LOG_ERROR("failed to open scanner. file id=%d. rc=%d:%s", file_id_, rc, strrc(rc));
        return rc;
    }

    int record_count = 0;
    Record record;
    rc = scanner.get_first_record(&record);
    for (; RC::SUCCESS == rc && record_count < limit; rc = scanner.get_next_record(&record))
    {
        if (trx == nullptr || trx->is_visible(this, &record))
        {
            rc = record_reader(&record, context);
            if (rc != RC::SUCCESS)
            {
                break;
            }
            record_count++;
        }
    }

    if (RC::RECORD_EOF == rc)
    {
        rc = RC::SUCCESS;
    } else
    {
        printf("failed to scan record. file id=%d, rc=%d:%s\n", file_id_, rc, strrc(rc));
    }
    scanner.close_scan();
    return rc;
}

RC Table::scan_record_by_index(Trx *trx, IndexScanner *scanner, ConditionFilter *filter, int limit, void *context,
                               RC(*record_reader)(Record *, void *))
{
    RC rc = RC::SUCCESS;
    RID rid;
    Record record;
    int record_count = 0;
    while (record_count < limit)
    {
        rc = scanner->next_entry(&rid);
        if (rc != RC::SUCCESS)
        {
            if (RC::RECORD_EOF == rc)
            {
                rc = RC::SUCCESS;
                break;
            }
            printf("Failed to scan table by index. rc=%d:%s", rc, strrc(rc));
            break;
        }

        rc = record_handler_->get_record(&rid, &record);
        if (rc != RC::SUCCESS)
        {
            printf("Failed to fetch record of rid=%d:%d, rc=%d:%s", rid.page_num, rid.slot_num, rc, strrc(rc));
            break;
        }
        
        if ((trx == nullptr || trx->is_visible(this, &record)) && (filter == nullptr || filter->filter(record)))
        {
            rc = record_reader(&record, context);
            if (rc != RC::SUCCESS)
            {
                LOG_TRACE("Record reader break the table scanning. rc=%d:%s", rc, strrc(rc));
                break;
            }
        }

        record_count++;
    }

    scanner->destroy();
    return rc;
}

class IndexInserter
{
public:
    explicit IndexInserter(Index *index) : index_(index)
    {
    }

    RC insert_index(const Record *record)
    {
        return index_->insert_entry(record->data, &record->rid);
    }
private:
    Index *index_;
};

static RC insert_index_record_reader_adapter(Record *record, void *context)
{
    IndexInserter &inserter = *(IndexInserter *)context;
    return inserter.insert_index(record);
}

RC Table::create_index(Trx *trx, const char *index_name, const char *attribute_name, int is_unique)
{
    if (index_name == nullptr || common::is_blank(index_name) ||
        attribute_name == nullptr || common::is_blank(attribute_name))
    {
        printf(COLOR_RED "[ERROR] " COLOR_YELLOW "Failed to create index ["
         COLOR_GREEN "%s" COLOR_YELLOW "] on database [" COLOR_GREEN
         "%s" COLOR_YELLOW "]. " COLOR_RED ": INVALID ARGUMENT\n", index_name, this->name());
        return RC::INVALID_ARGUMENT;
    }
    if (table_meta_.index(index_name) != nullptr ||
        table_meta_.find_index_by_field((attribute_name)))
    {
        printf(COLOR_RED "[ERROR] " COLOR_YELLOW "Failed to create index ["
         COLOR_GREEN "%s" COLOR_YELLOW "] on database [" COLOR_GREEN
         "%s" COLOR_YELLOW "]. " COLOR_RED ": SCHEMA INDEX EXIST\n", index_name, this->name());
        return RC::SCHEMA_INDEX_EXIST;
    }

    const FieldMeta *field_meta = table_meta_.field(attribute_name);
    if (!field_meta)
    {
        printf(COLOR_RED "[ERROR] " COLOR_YELLOW "Failed to create index ["
         COLOR_GREEN "%s" COLOR_YELLOW "] on database [" COLOR_GREEN
         "%s" COLOR_YELLOW "]. " COLOR_RED ": SCHEMA FIELD MISSING\n", index_name, this->name());
        return RC::SCHEMA_FIELD_MISSING;
    }

    IndexMeta new_index_meta;
    RC rc = new_index_meta.init(index_name, *field_meta, is_unique);
    if (rc != RC::SUCCESS)
    {
        printf(COLOR_RED "[ERROR] " COLOR_YELLOW "Failed to create index ["
         COLOR_GREEN "%s" COLOR_YELLOW "] on database [" COLOR_GREEN
         "%s" COLOR_YELLOW "]. " COLOR_RED ": INIT NEW_INDEX_META FAIL\n", index_name, this->name());
        return rc;
    }

    // 创建索引相关数据
    BplusTreeIndex *index = new BplusTreeIndex();
    std::string index_file = index_data_file(base_dir_.c_str(), name(), index_name);
    rc = index->create(index_file.c_str(), new_index_meta, *field_meta);
    if (rc != RC::SUCCESS)
    {
        delete index;
        LOG_ERROR("Failed to create bplus tree index. file name=%s, rc=%d:%s", index_file.c_str(), rc, strrc(rc));
        return rc;
    }

    // 遍历当前的所有数据，插入这个索引
    IndexInserter index_inserter(index);
    rc = scan_record(trx, nullptr, -1, &index_inserter, insert_index_record_reader_adapter);
    if (rc != RC::SUCCESS)
    {
        // rollback
        delete index;
        LOG_ERROR("Failed to insert index to all records. table=%s, rc=%d:%s", name(), rc, strrc(rc));
        return rc;
    }
    indexes_.push_back(index);

    TableMeta new_table_meta(table_meta_);
    rc = new_table_meta.add_index(new_index_meta);
    if (rc != RC::SUCCESS)
    {
        LOG_ERROR("Failed to add index (%s) on table (%s). error=%d:%s", index_name, name(), rc, strrc(rc));
        return rc;
    }
    // 创建元数据临时文件
    std::string tmp_file = table_meta_file(base_dir_.c_str(), name()) + ".tmp";
    std::fstream fs;
    fs.open(tmp_file, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if (!fs.is_open())
    {
        LOG_ERROR("Failed to open file for write. file name=%s, errmsg=%s", tmp_file.c_str(), strerror(errno));
        return RC::IOERR; // 创建索引中途出错，要做还原操作
    }
    if (new_table_meta.serialize(fs) < 0)
    {
        LOG_ERROR("Failed to dump new table meta to file: %s. sys err=%d:%s", tmp_file.c_str(), errno, strerror(errno));
        return RC::IOERR;
    }
    fs.close();

    // 覆盖原始元数据文件
    std::string meta_file = table_meta_file(base_dir_.c_str(), name());
    int ret = rename(tmp_file.c_str(), meta_file.c_str());
    if (ret != 0)
    {
        LOG_ERROR("Failed to rename tmp meta file (%s) to normal meta file (%s) while creating index (%s) on table (%s). "\
                      "system error=%d:%s", tmp_file.c_str(), meta_file.c_str(), index_name, name(), errno, strerror(errno));
        return RC::IOERR;
    }
    table_meta_.swap(new_table_meta);

    LOG_INFO("add a new index (%s) on the table (%s)", index_name, name());

    return rc;
}

RC Table::update_record(Trx *trx, const char *attribute_name, const Value *value, int condition_num, const Condition conditions[], int *updated_count)
{
    /// DEBUG INFO
    printf(COLOR_WHITE "\n[INFO] " COLOR_YELLOW "Update attribute "
           COLOR_GREEN "%s" COLOR_YELLOW
           " on table " COLOR_GREEN "%s" COLOR_YELLOW ".\n",
           attribute_name, this->name());

    RC rc = RC::SUCCESS;
    /// Pass 1. check if the set attribute is valid
    int update_attr_id = -1;
    const int normal_field_start_index = table_meta_.sys_field_num();
    const int attr_num = table_meta_.field_num() - normal_field_start_index;
    rc = RC::INVALID_ARGUMENT;
    for (int i = 0; i < attr_num; ++i)
    {
        const FieldMeta *field = table_meta_.field(i + normal_field_start_index);
        if (strcmp(field->name(), attribute_name) == 0)
        {
            update_attr_id = i;
            rc = RC::SUCCESS;
            break;
        }
    }
    if (rc != RC::SUCCESS)
    {
        printf(COLOR_RED "[ERROR] " COLOR_YELLOW "Failed to update attribute "
           COLOR_GREEN "%s" COLOR_YELLOW
           " on table " COLOR_GREEN "%s" COLOR_YELLOW ". " COLOR_RED
           "No such attribute in this table\n",
           attribute_name, this->name());
        return rc;
    }

    /// Pass 2. Collect conditional filters
    std::list<const Condition*> cond_list;
    for (int i = 0; i < condition_num; ++i)
    {
        cond_list.push_back(&conditions[i]);
    }    
    std::vector<DefaultConditionFilter*> condition_filters;
    for(auto it : cond_list)
    {
        if (it->left_is_attr == 1 && it->left_attr.relation_name != nullptr)
        {
            return RC::INVALID_ARGUMENT;
        }

        if (it->right_is_attr == 1 && it->right_attr.relation_name != nullptr)
        {
            return RC::INVALID_ARGUMENT;
        }

    //   if(
    //      ( it->left_is_attr == 0 && it->right_is_attr == 0 ) ||
         
    //      ( it->left_is_attr == 1 && it->right_is_attr == 0
    //        && it->left_attr.related_table == this->name()) ||

    //      ( it->left_is_attr == 0 && it->right_is_attr == 1
    //        && it->right_attr.related_table == this->name()) ||

    //      ( it->left_is_attr == 1 && it->right_is_attr == 1
    //        && it->left_attr.related_table == this->name()
    //        && it->right_attr.related_table == this->name())
    //      )
    //     /// Local to current table
        {
          DefaultConditionFilter *condition_filter_ = new DefaultConditionFilter();
          RC rc = condition_filter_->init(*this, *it);
          if (rc != RC::SUCCESS)
            {
              delete condition_filter_;
              for (DefaultConditionFilter *&filter : condition_filters)
                {
                  delete filter;
                }
              return rc;
            }
          condition_filters.push_back(condition_filter_);
        }
    }

    /// Pass 3. Find records and update
    RecordFileScanner scanner;
    CompositeConditionFilter condition_filter;
    condition_filter.init((const ConditionFilter **)condition_filters.data(), condition_filters.size());
    rc = scanner.open_scan(*data_buffer_pool_, file_id_, &condition_filter);
    if (rc != RC::SUCCESS)
    {
        printf(COLOR_RED "[ERROR] " COLOR_YELLOW "Failed to update attribute "
           COLOR_GREEN "%s" COLOR_YELLOW
           " on table " COLOR_GREEN "%s" COLOR_YELLOW ". " COLOR_RED
           "failed to open scanner. " COLOR_YELLOW "file id=" COLOR_GREEN "%d "
           COLOR_YELLOW, "rc=" COLOR_GREEN "%d" COLOR_YELLOW ":" COLOR_GREEN "%s",
           attribute_name, this->name(), file_id_, rc, strrc(rc));
        return rc;
    }

    int record_count = 0;
    Record record;
    rc = scanner.get_first_record(&record);
    for (; RC::SUCCESS == rc && record_count < INT_MAX; rc = scanner.get_next_record(&record))
    {
        Record update_record_ = record;
        RC ret = RC::SUCCESS;
        const FieldMeta *field = table_meta_.field(update_attr_id + normal_field_start_index);
        if (field->type() != value->type && !(field->nullable() == 1 && value->type == NULLS))
        {
            printf(COLOR_RED "[ERROR]"
            COLOR_YELLOW "Invalid value type. field name=" COLOR_GREEN "%s"
            COLOR_GREEN ", type=" COLOR_YELLOW "%d" COLOR_YELLOW ", but given="
            COLOR_GREEN "%d", field->name(), field->type(), value->type);
            return RC::SCHEMA_FIELD_TYPE_MISMATCH;
        }
        if (value->type == DATES)
        {
            ret = check_dates(value);
            if (ret != RC::SUCCESS)
            {
                printf(COLOR_RED "[ERROR] Invalid dates.\n");
                return RC::INVALID_ARGUMENT;
            }
        }
        memcpy(record.data + field->offset(), value->data, field->len());

        ret = record_handler_->update_record(&update_record_);
        if (ret != RC::SUCCESS)
        {
            printf(COLOR_RED "[ERROR] " COLOR_YELLOW "Failed to update attribute "
           COLOR_GREEN "%s" COLOR_YELLOW
           " on table " COLOR_GREEN "%s" COLOR_YELLOW ". " COLOR_RED
           "failed to get record. " COLOR_YELLOW "file id=" COLOR_GREEN "%d "
           COLOR_YELLOW, "ret=" COLOR_GREEN "%d" COLOR_YELLOW ":" COLOR_GREEN "%s",
           attribute_name, this->name(), file_id_, ret, strrc(ret));
           return ret;
        }
        ++record_count;
    }

    *updated_count = record_count;
    printf(COLOR_WHITE "[INFO] " COLOR_YELLOW "Update: "
    COLOR_GREEN "%d " COLOR_YELLOW "rows influenced.\n", record_count);
    scanner.close_scan();
    return RC::SUCCESS;
    // return RC::SQL_SYNTAX;
}

RC Table::check_dates(const Value *value)
{
    int dates_check;
    memcpy(&dates_check, value->data, sizeof(int));
    int yy = dates_check / 10000;
    int mm = dates_check % 10000 / 100;
    int dd = dates_check % 100;
    if (dd < 1) return RC::INVALID_ARGUMENT;
    if (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10 || mm == 12)
    {
        if (dd > 31) return RC::INVALID_ARGUMENT;
    }
    else if (mm == 4 || mm == 6 || mm == 9 || mm == 11)
    {
        if (dd > 30) return RC::INVALID_ARGUMENT;
    }
    else // mm == 2
    {
        if (yy % 400 == 0 || (yy % 4 == 0 && yy % 100 != 0))
        {
            if (dd > 29) return RC::INVALID_ARGUMENT;
        }
        else
        {
            if (dd > 28) return RC::INVALID_ARGUMENT;
        }
    }
    return RC::SUCCESS;
}

RC Table::set_meta(const TableMeta &meta)
{
  this->table_meta_ = meta;
  return RC::SUCCESS;
}

class RecordDeleter
{
public:
    RecordDeleter(Table &table, Trx *trx) : table_(table), trx_(trx)
    {
    }

    RC delete_record(Record *record)
    {
        RC rc = RC::SUCCESS;
        rc = table_.delete_record(trx_, record);
        if (rc == RC::SUCCESS)
        {
            deleted_count_++;
        }
        return rc;
    }

    int deleted_count() const
    {
        return deleted_count_;
    }

private:
    Table &table_;
    Trx *trx_;
    int deleted_count_ = 0;
};

static RC record_reader_delete_adapter(Record *record, void *context)
{
    RecordDeleter &record_deleter = *(RecordDeleter *)context;
    return record_deleter.delete_record(record);
}

RC Table::delete_record(Trx *trx, ConditionFilter *filter, int *deleted_count)
{
    RecordDeleter deleter(*this, trx);
    RC rc = scan_record(trx, filter, -1, &deleter, record_reader_delete_adapter);
    if (deleted_count != nullptr)
    {
        *deleted_count = deleter.deleted_count();
    }
    return rc;
}

RC Table::delete_record(Trx *trx, Record *record)
{
    RC rc = RC::SUCCESS;
    if (trx != nullptr)
    {
        rc = trx->delete_record(this, record);
    } else
    {
        rc = delete_entry_of_indexes(record->data, record->rid, false); // 重复代码 refer to commit_delete
        if (rc != RC::SUCCESS)
        {
            LOG_ERROR("Failed to delete indexes of record (rid=%d.%d). rc=%d:%s",
                      record->rid.page_num, record->rid.slot_num, rc, strrc(rc));
        } else
        {
            rc = record_handler_->delete_record(&record->rid);
        }
    }
    return rc;
}

RC Table::commit_delete(Trx *trx, const RID &rid)
{
    RC rc = RC::SUCCESS;
    Record record;
    rc = record_handler_->get_record(&rid, &record);
    if (rc != RC::SUCCESS)
    {
        return rc;
    }
    rc = delete_entry_of_indexes(record.data, record.rid, false);
    if (rc != RC::SUCCESS)
    {
        LOG_ERROR("Failed to delete indexes of record(rid=%d.%d). rc=%d:%s",
                  rid.page_num, rid.slot_num, rc, strrc(rc)); // panic?
    }

    rc = record_handler_->delete_record(&rid);
    if (rc != RC::SUCCESS)
    {
        return rc;
    }

    return rc;
}

RC Table::rollback_delete(Trx *trx, const RID &rid)
{
    RC rc = RC::SUCCESS;
    Record record;
    rc = record_handler_->get_record(&rid, &record);
    if (rc != RC::SUCCESS)
    {
        return rc;
    }

    return trx->rollback_delete(this, record); // update record in place
}

RC Table::insert_entry_of_indexes(const char *record, const RID &rid)
{
    RC rc = RC::SUCCESS;
    for (Index *index : indexes_)
    {
        rc = index->insert_entry(record, &rid);
        if (rc != RC::SUCCESS)
        {
            break;
        }
    }
    return rc;
}

RC Table::delete_entry_of_indexes(const char *record, const RID &rid, bool error_on_not_exists)
{
    RC rc = RC::SUCCESS;
    for (Index *index : indexes_)
    {
        rc = index->delete_entry(record, &rid);
        if (rc != RC::SUCCESS)
        {
            if (rc != RC::RECORD_INVALID_KEY || !error_on_not_exists)
            {
                break;
            }
        }
    }
    return rc;
}

Index* Table::find_index(const char *index_name) const
{
    for (Index *index: indexes_)
    {
        if (0 == strcmp(index->index_meta().name(), index_name))
        {
            return index;
        }
    }
    return nullptr;
}

IndexScanner* Table::find_index_for_scan(const DefaultConditionFilter &filter)
{
    const ConDesc *field_cond_desc = nullptr;
    const ConDesc *value_cond_desc = nullptr;
    if (filter.left().is_attr && !filter.right().is_attr)
    {
        field_cond_desc = &filter.left();
        value_cond_desc = &filter.right();
    } else if (filter.right().is_attr && !filter.left().is_attr)
    {
        field_cond_desc = &filter.right();
        value_cond_desc = &filter.left();
    }
    if (field_cond_desc == nullptr || value_cond_desc == nullptr)
    {
        return nullptr;
    }

    const FieldMeta *field_meta = table_meta_.find_field_by_offset(field_cond_desc->attr_offset);
    if (nullptr == field_meta)
    {
        LOG_PANIC("Cannot find field by offset %d. table=%s",
                  field_cond_desc->attr_offset, name());
        return nullptr;
    }

    const IndexMeta *index_meta = table_meta_.find_index_by_field(field_meta->name());
    if (nullptr == index_meta)
    {
        return nullptr;
    }

    Index *index = find_index(index_meta->name());
    if (nullptr == index)
    {
        return nullptr;
    }

    return index->create_scanner(filter.comp_op(), (const char *)value_cond_desc->value);
}

IndexScanner* Table::find_index_for_scan(const ConditionFilter *filter)
{
    if (nullptr == filter)
    {
        return nullptr;
    }

    // remove dynamic_cast
    const DefaultConditionFilter *default_condition_filter = dynamic_cast<const DefaultConditionFilter *>(filter);
    if (default_condition_filter != nullptr)
    {
        return find_index_for_scan(*default_condition_filter);
    }

    const CompositeConditionFilter *composite_condition_filter = dynamic_cast<const CompositeConditionFilter *>(filter);
    if (composite_condition_filter != nullptr)
    {
        int filter_num = composite_condition_filter->filter_num();
        for (int i = 0; i < filter_num; i++)
        {
            IndexScanner *scanner = find_index_for_scan(&composite_condition_filter->filter(i));
            if (scanner != nullptr)
            {
                return scanner; // 可以找到一个最优的，比如比较符号是=
            }
        }
    }
    return nullptr;
}

RC Table::sync()
{
    RC rc = data_buffer_pool_->flush_all_pages(file_id_);
    if (rc != RC::SUCCESS)
    {
        LOG_ERROR("Failed to flush table's data pages. table=%s, rc=%d:%s", name(), rc, strrc(rc));
        return rc;
    }

    for (Index *index: indexes_)
    {
        rc = index->sync();
        if (rc != RC::SUCCESS)
        {
            LOG_ERROR("Failed to flush index's pages. table=%s, index=%s, rc=%d:%s",
                      name(), index->index_meta().name(), rc, strrc(rc));
            return rc;
        }
    }
    LOG_INFO("Sync table over. table=%s", name());
    return rc;
}


