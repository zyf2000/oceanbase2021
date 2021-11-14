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

#include "sql/executor/tuple.h"
#include "storage/common/table.h"
#include "common/log/log.h"
#include "common/manual.h"

Tuple::Tuple(const Tuple &other) :
  values_(other.values())
{
  // LOG_PANIC("Copy constructor of tuple is not supported");
  // exit(1);
  //////////

}

Tuple::Tuple(Tuple &&other) noexcept : values_(std::move(other.values_)) {
}

Tuple & Tuple::operator=(Tuple &&other) noexcept {
  if (&other == this) {
    return *this;
  }

  values_.clear();
  values_.swap(other.values_);
  return *this;
}

Tuple::~Tuple() {
}

// add (Value && value)
void Tuple::add(TupleValue *value) {
  values_.emplace_back(value);
}
void Tuple::add(const std::shared_ptr<TupleValue> &other) {
  values_.emplace_back(other);
}
void Tuple::add(int value) {
  add(new IntValue(value));
}

void Tuple::add(float value) {
  add(new FloatValue(value));
}

void Tuple::add(const char *s, int len) {
  add(new StringValue(s, len));
}

void Tuple::pop_back()
{
    values_.pop_back();
}

int Tuple::tuple_cmp(int order_num, int *order_index, int *order_cmp, Tuple *tuple)
{
    for (int i = 0; i < order_num; ++i)
    {
        int index = order_index[i];
        std::shared_ptr<TupleValue> tuple_value_ = values_[index];
        std::shared_ptr<TupleValue> tuple_value = tuple->get_pointer(index);
        int value_cmp = tuple_value_->compare(*tuple_value);
        if (value_cmp == 0) continue;
        int result = value_cmp > 0;
        if (result == order_cmp[i]) return 0;
        else return 1;
    }
    return 0;
}
int Tuple::tuple_compare(int group_num, int *compare, Tuple *tuple)
{
    for (int i = 0; i < group_num; ++i)
    {
        int index = compare[i];
        std::shared_ptr<TupleValue> tuple_value_ = values_[index];
        std::shared_ptr<TupleValue> tuple_value = tuple->get_pointer(index);
        int value_cmp = tuple_value_->compare(*tuple_value);
        if (value_cmp != 0) return 0;
    }
    return 1;
}

////////////////////////////////////////////////////////////////////////////////

// std::string TupleField::to_string() const {
//   return std::string(table_name_) + "." + field_name_ + std::to_string(type_);
// }

////////////////////////////////////////////////////////////////////////////////
void TupleSchema::from_table(const Table *table, TupleSchema &schema) {
  const char *table_name = table->name();
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num();
  for (int i = 0; i < field_num; i++) {
    const FieldMeta *field_meta = table_meta.field(i);
    if (field_meta->visible()) {
      schema.add(field_meta->type(), table_name, field_meta->name(), nullptr);
    }
  }
}

void TupleSchema::add(AttrType type, const char *table_name, const char *field_name, const char *agg_name) {
  fields_.emplace_back(type, table_name, field_name, agg_name);
}

void TupleSchema::add_if_not_exists(AttrType type, const char *table_name, const char *field_name, const char *agg_name) {
  for (const auto &field: fields_) {
    if (0 == strcmp(field.table_name(), table_name) &&
        0 == strcmp(field.field_name(), field_name)) {
      return;
    }
  }

  add(type, table_name, field_name, agg_name);
}

void TupleSchema::append(const TupleSchema &other) {
  fields_.reserve(fields_.size() + other.fields_.size());
  for (const auto &field: other.fields_) {
    fields_.emplace_back(field);
  }
}

int TupleSchema::index_of_field(const char *table_name, const char *field_name) const {
  const int size = fields_.size();
  for (int i = 0; i < size; i++) {
    const TupleField &field = fields_[i];
    if (0 == strcmp(field.table_name(), table_name) && 0 == strcmp(field.field_name(), field_name)) {
      return i;
    }
  }
  return -1;
}

void TupleSchema::print(std::ostream &os, bool print_header) const {
  if (fields_.empty()) {
    os << "No schema";
    return;
  }

  // 判断有多张表还是只有一张表
  std::set<std::string> table_names;
  for (const auto &field: fields_) {
    table_names.insert(field.table_name());
  }

  for (std::vector<TupleField>::const_iterator iter = fields_.begin(), end = --fields_.end();
       iter != end; ++iter) {
    if (iter->agg_name() != nullptr)
        os << iter->agg_name() << "(";
    if (table_names.size() > 1 || print_header) {
      os << iter->table_name() << ".";
    }
    os << iter->field_name();
    if (iter->agg_name() != nullptr)
        os << ")";
    os << " | ";
  }

    if (fields_.back().agg_name() != nullptr)
        os << fields_.back().agg_name() << "(";
  if (table_names.size() > 1 || print_header) {
    os << fields_.back().table_name() << ".";
  }
//   os << fields_.back().field_name() << std::endl;
    os << fields_.back().field_name();
    if (fields_.back().agg_name() != nullptr)
        os << ")";
    os << std::endl;
}

/////////////////////////////////////////////////////////////////////////////
TupleSet::TupleSet(TupleSet &&other) : tuples_(std::move(other.tuples_)), schema_(other.schema_){
  other.schema_.clear();
}

TupleSet &TupleSet::operator=(TupleSet &&other) {
  if (this == &other) {
    return *this;
  }

  schema_.clear();
  schema_.append(other.schema_);
  other.schema_.clear();

  tuples_.clear();
  tuples_.swap(other.tuples_);
  return *this;
}

void TupleSet::add(Tuple &&tuple) {
  tuples_.emplace_back(std::move(tuple));
}

void TupleSet::clear() {
  tuples_.clear();
  schema_.clear();
}

void TupleSet::print(std::ostream &os, bool print_header) const {
  if (schema_.fields().empty()) {
    LOG_WARN("Got empty schema");
    return;
  }

  schema_.print(os, print_header);

  for (const Tuple &item : tuples_) {
    const std::vector<std::shared_ptr<TupleValue>> &values = item.values();
    for (std::vector<std::shared_ptr<TupleValue>>::const_iterator iter = values.begin(), end = --values.end();
          iter != end; ++iter) {
      (*iter)->to_string(os);
      os << " | ";
    }
    values.back()->to_string(os);
    os << std::endl;
  }
}

RC TupleSet::order_by(Selects *selects)
{
    int attr_num = selects->order_attr_num;
    RelAttr *attrs = selects->order_attrs;

    printf( COLOR_WHITE "[INFO] " COLOR_YELLOW "Order By: Prepare sorting order.\n");
    int *order_index = new int[attr_num];
    const std::vector<TupleField> fields = schema_.fields();
    for (int i = 0; i < attr_num; ++i)
    {
        order_index[i] = -1;
        for (int j = 0; j < fields.size(); ++j)
        {
            if ( strcmp((const char*)attrs[i].relation_name, fields[j].table_name()) == 0
            && strcmp(attrs[i].attribute_name, fields[j].field_name()) == 0 )
            {
                order_index[i] = j;
                break;
            }
        }
        if (order_index[i] == -1)
        {
            printf(COLOR_RED "[ERROR] Order By: invalied attribute.\n");
            return RC::INVALID_ARGUMENT;
        }
    }

    printf( COLOR_WHITE "[INFO] " COLOR_YELLOW "Order By: Bubble sorting tuple set.\n");
    int n = tuples_.size();
    Tuple tuples[n + 2];
    for (int i = 0; i < n ; ++i)
        tuples[i] = (Tuple)tuples_[i];

    for (int i = 0; i < n; ++i)
        for (int j = 1; j < n - i; ++j)
        {
            // printf("%d %d :",j-1,j);
            Tuple tuple_l = tuples[j - 1];
            Tuple tuple_r = tuples[j];
            if (tuple_l.tuple_cmp(attr_num, order_index, selects->order_cmp, &tuple_r))
            {
                // printf("swap\n");
                tuples[j - 1] = (Tuple)tuple_r;
                tuples[j] = (Tuple)tuple_l;
            }
        }
    printf( COLOR_WHITE "[INFO] " COLOR_YELLOW "Order By: Put sorting result into tuple set.\n");
    tuples_.clear();
    for (int i = 0; i < n; ++i)
        tuples_.push_back(tuples[i]);
    delete[] order_index;
    return RC::SUCCESS;
}

RC TupleSet::group_by(Selects *selects)
{

    printf( COLOR_WHITE "[INFO] " COLOR_YELLOW "Group By: Check if there's no aggregation func in select attrs.\n");
    bool agg_flag = false;
    for (int i = 0; i < selects->attr_num; ++i)
    {
        if (selects->attributes[i].aggregate_func != AGG_UNDEFINED)
        {
            agg_flag = true;
            break;
        }
    }
    if (!agg_flag)
        return RC::SUCCESS;

    // TupleSet::       TupleSchema schema_;
    // TupleSchema::    std::vector<TupleField> fields_;
    // TupleField::     AttrType  type_;
    //                  std::string table_name_;
    //                  std::string field_name_;
    const std::vector<TupleField> fields_ = schema_.fields();
    TupleSchema schema;
    // Attrs in selects <-> which colon in fields_
    int loc[selects->attr_num];

    /// Pass 1. Remake tuple schema according to aggregation func
    ///         and find locations of attrs in fields_
    printf( COLOR_WHITE "[INFO] " COLOR_YELLOW "Group By: Remake schema for tuple set.\n");
    for (int i = 0; i < selects->attr_num; ++i)
    {
        const RelAttr *attr = &selects->attributes[i];
        const char *field_name;
        const char *field_table;
        AttrType field_type = UNDEFINED;
        const char *field_aggname;

        for (int j = 0; j < fields_.size(); ++j)
        {
            // printf("%d\n", j);
            // printf("%s.%s\n", fields_[j].table_name(), fields_[j].field_name());
            // printf("%s.%s\n", attr->relation_name, attr->attribute_name);
            if ( ( strcmp(attr->attribute_name, "*") == 0 || strcmp(attr->attribute_name, fields_[j].field_name()) == 0 )
                && ( attr->relation_name == nullptr || strcmp(attr->relation_name, fields_[j].table_name()) == 0 ) )
            {
                loc[i] = j;
                break;
            }
        }
        // printf("%d: %d\n", i, loc[i]);

        // not an attribute with aggregation func
        if (attr->aggregate_func == AGG_UNDEFINED)
        {
            field_name = fields_[loc[i]].field_name();
            field_table = fields_[loc[i]].table_name();
            field_type = fields_[loc[i]].type();
            field_aggname = nullptr;
        }
        else
        {
            switch (attr->aggregate_func)
            {
                case AGG_COUNT:
                    field_type = INTS;
                break;
                case AGG_MAX:
                    field_type = fields_[loc[i]].type();
                break;
                case AGG_MIN:
                    field_type = fields_[loc[i]].type();
                break;
                case AGG_AVG:
                    field_type = FLOATS;
                break;
                default:
                    assert(0);
            }
            field_table = attr->relation_name;
            if (field_table == nullptr)
                field_table = selects->relations[0];
            field_name = attr->attribute_name;
            field_aggname = attr->aggregate_func_name;
        }
        schema.add(field_type, field_table, field_name, field_aggname);
    }
    schema_ = schema;

    /// Pass 2. Make groups
    printf( COLOR_WHITE "[INFO] " COLOR_YELLOW "Group By: Make groups.\n");
    int compare[selects->group_attr_num];
    for (int i = 0; i < selects->group_attr_num; ++i)
    {
        const RelAttr *attr = &selects->group_attrs[i];
        for (int j = 0; j < fields_.size(); ++j)
        {
            if (strcmp(attr->relation_name, fields_[j].table_name()) == 0
                && strcmp(attr->attribute_name, fields_[j].field_name()) == 0)
            {
                compare[i] = j;
                break;
            }
        }
    }
    int group_cnt = 0;
    int group_id[tuples_.size()];
    for (int i = 0; i < tuples_.size(); ++i)
    {
        bool new_group = true;
        for (int j = 0; j < i; ++j)
        {
            if (tuples_[i].tuple_compare(selects->group_attr_num, compare, &tuples_[j]))
            {
                new_group = false;
                group_id[i] = group_id[j];
                break;
            }
        }
        if (new_group)
            group_id[i] = group_cnt++;
    }
    // printf("%d\n", group_cnt);
    // for (int i = 0; i < tuples_.size(); ++i)
    //     printf("%d: %d\n", i, group_id[i]);

    return RC::SUCCESS;
}

void TupleSet::set_schema(const TupleSchema &schema) {
  schema_ = schema;
}

const TupleSchema &TupleSet::get_schema() const {
  return schema_;
}

bool TupleSet::is_empty() const {
  return tuples_.empty();
}

int TupleSet::size() const {
  return tuples_.size();
}

const Tuple &TupleSet::get(int index) const {
  return tuples_[index];
}

const std::vector<Tuple> &TupleSet::tuples() const {
  return tuples_;
}

/////////////////////////////////////////////////////////////////////////////
TupleRecordConverter::TupleRecordConverter(Table *table, TupleSet &tuple_set) :
      table_(table), tuple_set_(tuple_set){
}

void TupleRecordConverter::add_record(const char *record) {
  const TupleSchema &schema = tuple_set_.schema();
  Tuple tuple;
  const TableMeta &table_meta = table_->table_meta();
  for (const TupleField &field : schema.fields()) {
    const FieldMeta *field_meta = table_meta.field(field.field_name());
    assert(field_meta != nullptr);
    switch (field_meta->type()) {
      case INTS: {
        if (field_meta->nullable() == 1)
        {
            const char *tmp = record + field_meta->offset();
            if (strcasecmp(tmp, "null") == 0)
            {
                tuple.add(tmp, strlen(tmp));
                break;
            }
        }
        int value = *(int*)(record + field_meta->offset());
        tuple.add(value);
      }
      break;
      case FLOATS: {
        if (field_meta->nullable() == 1)
        {
            const char *tmp = record + field_meta->offset();
            if (strcasecmp(tmp, "null") == 0)
            {
                tuple.add(tmp, strlen(tmp));
                break;
            }
        }
        float value = *(float *)(record + field_meta->offset());
        tuple.add(value);
      }
        break;
      case CHARS: {
        const char *s = record + field_meta->offset();  // 现在当做Cstring来处理
        tuple.add(s, strlen(s));
      }
      break;
      case DATES: {
        if (field_meta->nullable() == 1)
        {
            const char *tmp = record + field_meta->offset();
            if (strcasecmp(tmp, "null") == 0)
            {
                tuple.add(tmp, strlen(tmp));
                break;
            }
        }
        int value = *(int*)(record + field_meta->offset());
        const char *s = int_to_char(value);
        tuple.add(s, strlen(s));
      }
      break;
      default: {
        LOG_PANIC("Unsupported field type. type=%d", field_meta->type());
      }
    }
  }

  tuple_set_.add(std::move(tuple));
}

const char * TupleRecordConverter::int_to_char(int dates_int)
{
    char *re = new char[11];
    // memset(re, 0, sizeof(re));
    memset(re, 0, 11);
    int yy = dates_int / 10000;
    int mm = dates_int % 10000 / 100;
    int dd = dates_int % 100;
    for (int i = 3; i >= 0; --i)
    {
        re[i] = yy % 10 + '0';
        yy /= 10;
    }
    re[4] = '-';
    for (int i = 6; i >= 5; --i)
    {
        re[i] = mm % 10 + '0';
        mm /= 10;
    }
    re[7] = '-';
    for (int i = 9; i >= 8; --i)
    {
        re[i] = dd % 10 + '0';
        dd /= 10;
    }
    return re;
}
