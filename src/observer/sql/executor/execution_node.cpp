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
// Created by Wangyunlai on 2021/5/14.
//

#include "sql/executor/execution_node.h"
#include "storage/common/table.h"
#include "common/log/log.h"

SelectExeNode::SelectExeNode() : table_(nullptr) {
}

SelectExeNode::~SelectExeNode() {
  for (DefaultConditionFilter * &filter : condition_filters_) {
    delete filter;
  }
  condition_filters_.clear();
}

RC SelectExeNode::init(Trx *trx, Table *table, TupleSchema &&tuple_schema, std::vector<DefaultConditionFilter *> &&condition_filters) {
    trx_ = trx;
    table_ = table;
    tuple_schema_ = tuple_schema;
    condition_filters_ = std::move(condition_filters);
    return RC::SUCCESS;
}

class ValueRecordConverter
{
public:
    void add_record(const char* record)
    {
        const TableMeta& table_meta = table_->table_meta();
        
        std::vector<Value> val;
        
        for (int i = 0; i < table_->table_meta().field_num(); i++)
        {
          const FieldMeta* field_meta = table_meta.field(i);
          switch (field_meta->type())
            {
            case INTS: {
              int value = *(int*)(record + field_meta->offset());
              Value t;
              t.type = INTS;
              t.data = new int;
              memcpy(t.data, &value, sizeof(int));
              val.emplace_back(std::move(t));
            }
              break;
            case FLOATS: {
              float value = *(float *)(record + field_meta->offset());
              Value t;
              t.type = FLOATS;
              t.data = new float;
              memcpy(t.data, &value, sizeof(float));
              val.emplace_back(std::move(t));
            }
              break;
            case CHARS: {
              const char *s = record + field_meta->offset();  // 现在当做Cstring来处理
              int len = strlen(s);
              Value t;
              t.type = CHARS;
              t.data = new char[len+1];
              memcpy(t.data, s, len + 1);
              val.emplace_back(std::move(t));
            }
              break;
            case DATES: {
              assert(0);
              // Not Implemented yet.
                
              // int value = *(int*)(record + field_meta->offset());
              // const char *s = int_to_char(value);
              // tuple.add(s, strlen(s));
            }
              break;
            default: {
              LOG_PANIC("Unsupported field type. type=%d", field_meta->type());
            }
            }
        }

        this->values_.emplace_back(val);
    }
  
  ValueRecordConverter(Table* table,     std::vector<std::vector<Value>>& values) :
    table_(table), values_(values) {}
private:
    Table* table_;
    std::vector<std::vector<Value>>& values_;
};

void my_record_reader(const char* data, void* context)
{
  ValueRecordConverter* conv = (ValueRecordConverter*) context;
  conv->add_record(data);
}

void record_reader(const char *data, void *context) {
  TupleRecordConverter *converter = (TupleRecordConverter *)context;
  converter->add_record(data);
}

RC SelectExeNode::execute_to_value(std::vector<std::vector<Value>> &value_set) {
  CompositeConditionFilter condition_filter;
  condition_filter.init((const ConditionFilter **)condition_filters_.data(), condition_filters_.size());

  value_set.clear();
  ValueRecordConverter converter(table_, value_set);
  return table_->scan_record(trx_, &condition_filter, -1, (void *)&converter, record_reader);
}

RC SelectExeNode::execute(TupleSet &tuple_set) {
  CompositeConditionFilter condition_filter;
  condition_filter.init((const ConditionFilter **)condition_filters_.data(), condition_filters_.size());

  tuple_set.clear();
  tuple_set.set_schema(tuple_schema_);
  TupleRecordConverter converter(table_, tuple_set);
  return table_->scan_record(trx_, &condition_filter, -1, (void *)&converter, record_reader);
}
