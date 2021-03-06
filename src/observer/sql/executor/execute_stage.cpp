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
// Created by Longda on 2021/4/13.
//

#include <string>
#include <sstream>
#include <list>
#include <algorithm>
#include "execute_stage.h"

#include "common/io/io.h"
#include "common/log/log.h"
#include "common/seda/timer_stage.h"
#include "common/manual.h"
#include "common/lang/string.h"
#include "session/session.h"
#include "event/storage_event.h"
#include "event/sql_event.h"
#include "event/session_event.h"
#include "event/execution_plan_event.h"
#include "sql/executor/execution_node.h"
#include "sql/executor/tuple.h"
#include "storage/common/table.h"
#include "storage/default/default_handler.h"
#include "storage/common/condition_filter.h"
#include "storage/common/table.h"
#include "storage/trx/trx.h"

using namespace common;

TupleValue* build_value(Value* immediate);
class Arbiter
{
public:
  bool judge(const Tuple& tup) const;
  Arbiter(Condition* rule, const TupleSchema& tsc);
private:
  int lhs;
  int rhs;
  TupleValue* ltv;
  TupleValue* rtv;
  CompOp cmpop;
  int valid;
};

void print_tuple(const Tuple& tup)
{
  for(const auto& it : tup.values())
    {
      it->to_string(std::cout);
      std::cout<<"  |  ";
    }
  std::cout<<std::endl;
}

void print_cond(Condition* cond)
{
  if(cond->left_is_attr)
    printf(COLOR_CYAN "%s" COLOR_YELLOW "." COLOR_CYAN "%s " COLOR_GREEN,
           cond->left_attr.relation_name,
           cond->left_attr.attribute_name);
  else
    {
      build_value(&cond->left_value)->to_string(std::cout);
    }
  switch(cond->comp)
    {
    case EQUAL_TO:
      printf("==");
      break;
    case LESS_EQUAL:
      printf("==");
      break;
    case NOT_EQUAL:
      printf("==");
      break;
    case LESS_THAN:
      printf("==");
      break;
    case GREAT_EQUAL:
      printf("==");
      break;
    case GREAT_THAN:
      printf("==");
      break;
    default:
      printf("[UNK]");
      break;
    }

  if(cond->right_is_attr)
    printf(COLOR_CYAN " %s" COLOR_YELLOW "." COLOR_CYAN "%s",
           cond->right_attr.relation_name,
           cond->right_attr.attribute_name);
  else
    {
      build_value(&cond->right_value)->to_string(std::cout);
    }
  
}
void attr_to_number(const TupleSchema& tsc,
                    const char* relation_name,
                    const char* attribute_name,
                    const std::vector<Table*>& tables,
                    std::vector<int>& numbers);



/// Resolve each attribute's corresponding table. 
    /// This function will directly modify [selects] 
/// to add context information. 
std::pair<RC, std::string> Resolve_Attr_Scope(
                                              std::list<RelAttr*> attr_array,     /// Attribute Array
                                              Table* table                        /// Table structure
                                              );


/// Create SELECT's executor for each table, only considering local filters
std::pair<RC, std::string>
Create_Select_Table_Executor(Trx* transaction,                // Transaction
                             Table* table,                    // Table struct
                             const std::list<RelAttr*>& attr_list,   // Attribute list
                             std::list<std::pair<Condition*, bool>>& cond_list, // Condition list
                             SelectExeNode& select_node);     // [Select Node]

void dfs_cartesian(const std::vector<TupleSet>& ts,
                   int ts_depth,
                   int max_depth,
                   Tuple& current_tuple,
                   const std::vector<Arbiter>& arbiters,
                   std::vector<Tuple>& output);

RC check_tuple(TupleSchema& tsc,
               std::vector<Tuple>& tup,
               Condition* rule);

TupleValue* build_value(Value* immediate);

float float_from_string(std::string s);

//! Constructor
ExecuteStage::ExecuteStage(const char *tag) : Stage(tag) { }

//! Destructor
ExecuteStage::~ExecuteStage() { }

//! Parse properties, instantiate a stage object
Stage* ExecuteStage::make_stage(const std::string &tag)
{
  ExecuteStage *stage = new(std::nothrow) ExecuteStage(tag.c_str());
  if (stage == nullptr)
    {
      LOG_ERROR("new ExecuteStage failed");
      return nullptr;
    }
  stage->set_properties();
  return stage;
}

//! Set properties for this object set in stage specific properties
bool ExecuteStage::set_properties()
{
  //  std::string stageNameStr(stageName);
  //  std::map<std::string, std::string> section = theGlobalProperties()->get(
  //    stageNameStr);
  //
  //  std::map<std::string, std::string>::iterator it;
  //
  //  std::string key;

  return true;
}

//! Initialize stage params and validate outputs
bool ExecuteStage::initialize()
{
  LOG_TRACE("Enter");

  std::list<Stage *>::iterator stgp = next_stage_list_.begin();
  default_storage_stage_ = *(stgp++);
  mem_storage_stage_ = *(stgp++);

  LOG_TRACE("Exit");
  return true;
}

//! Cleanup after disconnection
void ExecuteStage::cleanup()
{
  LOG_TRACE("Enter");

  LOG_TRACE("Exit");
}

void ExecuteStage::handle_event(StageEvent *event)
{
  LOG_TRACE("Enter\n");

  handle_request(event);

  LOG_TRACE("Exit\n");
  return;
}

void ExecuteStage::callback_event(StageEvent *event, CallbackContext *context)
{
  LOG_TRACE("Enter\n");

  // here finish read all data from disk or network, but do nothing here.
  ExecutionPlanEvent *exe_event = static_cast<ExecutionPlanEvent *>(event);
  SQLStageEvent *sql_event = exe_event->sql_event();
  sql_event->done_immediate();

  LOG_TRACE("Exit\n");
  return;
}

void ExecuteStage::handle_request(common::StageEvent *event)
{

  ExecutionPlanEvent *exe_event = static_cast<ExecutionPlanEvent *>(event);
  SessionEvent *session_event = exe_event->sql_event()->session_event();
  Query *sql = exe_event->sqls();
  const char *current_db = session_event->get_client()->session->get_current_db().c_str();

  CompletionCallback *cb = new(std::nothrow) CompletionCallback(this, nullptr);
  if (cb == nullptr)
    {
      LOG_ERROR("Failed to new callback for ExecutionPlanEvent");
      exe_event->done_immediate();
      return;
    }
  exe_event->push_callback(cb);

  switch (sql->flag)
    {
    case SCF_SELECT:
      { // select
        //do_select(current_db, sql, exe_event->sql_event()->session_event());
        RC rc = manual_do_select(current_db, sql, exe_event->sql_event()->session_event());
        fflush(stdout);
        if (rc)
        {
            session_event->set_response("FAILURE\n");
        }
        exe_event->done_immediate();
      }
      break;

    case SCF_INSERT:
    case SCF_UPDATE:
    case SCF_DELETE:
    case SCF_CREATE_TABLE:
    case SCF_SHOW_TABLES:
    case SCF_DESC_TABLE:
    case SCF_DROP_TABLE:
    case SCF_CREATE_INDEX:
    case SCF_CREATE_MULTI_INDEX:
    case SCF_DROP_INDEX:
    case SCF_LOAD_DATA:
      {
        StorageEvent *storage_event = new(std::nothrow) StorageEvent(exe_event);
        if (storage_event == nullptr)
          {
            LOG_ERROR("Failed to new StorageEvent");
            event->done_immediate();
            return;
          }

        default_storage_stage_->handle_event(storage_event);
      }
      break;
    case SCF_SYNC:
      {
        RC rc = DefaultHandler::get_default().sync();
        session_event->set_response(strrc(rc));
        exe_event->done_immediate();
      }
      break;
    case SCF_BEGIN:
      {
        session_event->get_client()->session->set_trx_multi_operation_mode(true);
        session_event->set_response(strrc(RC::SUCCESS));
        exe_event->done_immediate();
      }
      break;
    case SCF_COMMIT:
      {
        Trx *trx = session_event->get_client()->session->current_trx();
        RC rc = trx->commit();
        session_event->get_client()->session->set_trx_multi_operation_mode(false);
        session_event->set_response(strrc(rc));
        exe_event->done_immediate();
      }
      break;
    case SCF_ROLLBACK:
      {
        Trx *trx = session_event->get_client()->session->current_trx();
        RC rc = trx->rollback();
        session_event->get_client()->session->set_trx_multi_operation_mode(false);
        session_event->set_response(strrc(rc));
        exe_event->done_immediate();
      }
      break;
    case SCF_HELP:
      {
        const char *response = "show tables;\n"
          "desc `table name`;\n"
          "create table `table name` (`column name` `column type`, ...);\n"
          "create index `index name` on `table` (`column`);\n"
          "insert into `table` values(`value1`,`value2`);\n"
          "update `table` set column=value [where `column`=`value`];\n"
          "delete from `table` [where `column`=`value`];\n"
          "select [ * | `columns` ] from `table`;\n";
        session_event->set_response(response);
        exe_event->done_immediate();
      }
      break;
    case SCF_EXIT:
      {
        // do nothing
        const char *response = "Unsupported\n";
        session_event->set_response(response);
        exe_event->done_immediate();
      }
      break;
    default:
      {
        exe_event->done_immediate();
        LOG_ERROR("Unsupported command=%d\n", sql->flag);
      }
    }
}

void end_trx_if_need(Session *session, Trx *trx, bool all_right)
{
  if (!session->is_trx_multi_operation_mode())
    {
      if (all_right)
        {
          trx->commit();
        } else
        {
          trx->rollback();
        }
    }
}


// ?????????????????????????????????????????????????????????????????????????????????where?????????????????????????????????????????????????????????
// ???????????????????????????. ???????????????????????????resolve????????????execution????????????????????????

///  The manually implementation of [SELECT]
/// <param name="db">database name(default is 'sys')</param>
/// <param name="sql">Query structure</param>
/// <param name="session_event">Connection session</param>
/// <returns>a Return Code implies the query result</returns>
RC ExecuteStage::manual_do_select(const char *db, Query *sql, SessionEvent *session_event)
{
  Session* session     = session_event->get_client()->session; /// Session struct
  Trx*     transaction = session->current_trx(); /// Transaction
  Selects& selects     = sql->sstr.selection;  /// Select Structure
  // std::reverse(selects.attributes, selects.attributes + selects.attr_num);
  std::reverse(selects.relations, selects.relations + selects.relation_num);
  /// Test log
  printf(COLOR_WHITE "[INFO] " COLOR_YELLOW "Select from tables: " COLOR_GREEN);
  for (int i = 0; i < selects.relation_num; i++)
    {
      printf("%s, ", selects.relations[i]);
    }
  printf("\n");
  printf(COLOR_WHITE "[INFO] " COLOR_YELLOW "The selected attributes are: " COLOR_GREEN);
  
  for (int i = 0; i < selects.attr_num; i++)
    {
      LOG_ERROR(COLOR_GREEN "%s" COLOR_WHITE "." COLOR_CYAN "%s" COLOR_WHITE ", ",
             selects.attributes[i].relation_name == NULL ?
             "(NULL)" : selects.attributes[i].relation_name,
             selects.attributes[i].attribute_name);
    }
  LOG_ERROR("\n");
  fflush(stdout);
  /// Test log END

    
  /**
   * Pass 1, Resolve each attribute's valid scopes.
   */
  std::vector<Table*> tables;
  {
    /// Collect all attributes to attr_array
    std::list<RelAttr*> attr_array;
    /// SELECT [***] from ......
    for (int i = 0; i < selects.attr_num; i++)
      {
        attr_array.push_back(&selects.attributes[i]);
      }
    /// SELECT ... from ... WHERE [***]
    Table *table_temp = new Table;
    for(int i = 0; i < selects.condition_num; i++)
      {
        if(selects.conditions[i].left_is_attr)
          attr_array.push_back(&selects.conditions[i].left_attr);
        else
        {
            // check if DATES is valid (left condition is value)
            if (selects.conditions[i].left_value.type == DATES)
            {
                RC rc = table_temp->check_dates(&selects.conditions[i].left_value);
                if (rc != RC::SUCCESS)
                {
                    printf(COLOR_RED "[ERROR] Invalid dates.\n");
                    return rc;
                }
            }
        }
        if(selects.conditions[i].right_is_attr)
          attr_array.push_back(&selects.conditions[i].right_attr);
        else
        {
            // check if DATES is valid (right condition is value)
            if (selects.conditions[i].right_value.type == DATES)
            {
                RC rc = table_temp->check_dates(&selects.conditions[i].right_value);
                if (rc != RC::SUCCESS)
                {
                    printf(COLOR_RED "[ERROR] Invalid dates.\n");
                    return rc;
                }
            }
        }
      }
    delete table_temp;
    /// SELECT ... ORDER BY [***]
    for (int i = 0; i < selects.order_attr_num; ++i)
        attr_array.push_back(&selects.order_attrs[i]);
    /// SELECT ... GROUP BY [***]
    for (int i = 0; i < selects.group_attr_num; ++i)
        attr_array.push_back(&selects.group_attrs[i]);
    
        
    for(size_t i = 0;
        i < selects.relation_num;
        i++)
      {
          // find each table's point
        Table* table = DefaultHandler::get_default().find_table(db, selects.relations[i]);
        if(table == nullptr)
          {
            LOG_ERROR(COLOR_RED "[ERROR] " COLOR_YELLOW "No such table ["
                 COLOR_GREEN "%s" COLOR_YELLOW "] in db ["
                 COLOR_GREEN "%s" COLOR_YELLOW "]\n", selects.relations[i], db);
            end_trx_if_need(session, transaction, false);
            return RC::SCHEMA_TABLE_NOT_EXIST;
          }
        tables.emplace_back(table);
        // modify selects.conditions.related_table
        auto pr = Resolve_Attr_Scope(attr_array, table);
        if(pr.first != RC::SUCCESS)
          {
            LOG_ERROR("%s", pr.second.c_str());
            end_trx_if_need(session, transaction, false);
            return pr.first;
          }
      }
        
    /// Check if there is an attribute that can't be attached
    for(auto it : attr_array)
      {
      LOG_ERROR("The attachment of %s is %p\n", it->attribute_name, it->related_table);
      fflush(stdout);
      if(it->related_table == nullptr && strcmp(it->attribute_name, "*") != 0)
          {
          printf(COLOR_RED "[ERROR] " COLOR_YELLOW "Attribute ["
                 COLOR_GREEN "%s.%s" COLOR_YELLOW "] is not attached in any table.\n",
                 it->relation_name != nullptr?it->relation_name:"(NULL)",
                 it->attribute_name);
          fflush(stdout);
          end_trx_if_need(session, transaction, false);
          return RC::SCHEMA_FIELD_NOT_EXIST;
                
          }
      }
    
  }

  /**
   * Pass 2, Mapping the selection operation and local WHERE statements to each table
   **/
  std::vector<SelectExeNode*> select_nodes;
  std::list<std::pair<Condition*, bool>> cond_list;
  /// The second element yields if this condition is used.
  {
    /// Collect all attributions and conditions
    std::list<RelAttr*> attr_list;
    
    for (int i = 0; i < selects.attr_num; i++)
      {
        attr_list.push_back(&selects.attributes[i]);
      }

    for(int i = 0; i < selects.condition_num; i++)
      {
        cond_list.push_back({&selects.conditions[i], false});

        // Patch: Cross-table query
        if(selects.conditions[i].left_is_attr)
            attr_list.push_back(&selects.conditions[i].left_attr);

        if(selects.conditions[i].right_is_attr)
            attr_list.push_back(&selects.conditions[i].right_attr);
      }
    
    
    for(size_t i = 0;
        i < selects.relation_num;
        i++)
      {
        SelectExeNode *select_node = new SelectExeNode;
        // find each table's point
        Table* table = DefaultHandler::get_default().find_table(db, selects.relations[i]);
        assert(table != nullptr);
        auto pr = Create_Select_Table_Executor(transaction, table, attr_list, cond_list, *select_node);
        if(pr.first != RC::SUCCESS)
          {
            delete select_node;
            for (SelectExeNode *&tmp_node: select_nodes)
              {
                delete tmp_node;
              }
            LOG_ERROR(pr.second.c_str());
            end_trx_if_need(session, transaction, false);
            return pr.first;
          }
        select_nodes.push_back(select_node);
      }

    /// If there is no table given
    if (select_nodes.empty())
      {
        LOG_ERROR(COLOR_RED "[ERROR]" COLOR_YELLOW "No table given.");
        end_trx_if_need(session, transaction, false);
        return RC::SQL_SYNTAX;
      }
  }

  /**
   * Pass 3. Perform execution of SELECT, and join the table selected.
   **/
  {
    std::vector<TupleSet> tuple_sets;
    for (SelectExeNode *&node: select_nodes)
      {
        TupleSet tuple_set;
        RC rc = node->execute(tuple_set);
        
        if (rc != RC::SUCCESS)
          {
            for (SelectExeNode *&tmp_node: select_nodes)
              {
                delete tmp_node;
              }
            end_trx_if_need(session, transaction, false);
            return rc;
          }
        else
          {
            tuple_sets.push_back(std::move(tuple_set));
          }
        
      }
    TupleSet* result;
    std::stringstream ss;
    if (tuple_sets.size() > 1)
    {
        /// Hint: this query is (worst) O(n^k) for [k] tables.
        
        /// Note: This algorithm is of LOW PERFORMANCE, 
          /// use INNER JOIN for more efficiency (and restricted) 
          /// algorithm.

          /// Note: This algorithm is only valid in condition between 2 tables.
        /// Pass 3.1. Cartesian Product

        TupleSchema tsc;
        tsc.append(tuple_sets[0].get_schema());
        for(int i = 1; i < tuple_sets.size(); i++)
          {
            tsc.append(tuple_sets[1].get_schema());
            std::vector<Tuple> tup_vec;

            std::vector<Arbiter> arbiters;
            for(const auto& it : cond_list)
              {
                if(it.second == false)
                  arbiters.emplace_back(Arbiter(it.first, tsc));
              }
            Tuple current_tuple;
            printf("Tuple Set Size: %d -- %d\n", tuple_sets[0].size(), tuple_sets[1].size());
            fflush(stdout);
            dfs_cartesian(tuple_sets, 0, 2, current_tuple, arbiters, tup_vec);
            TupleSet res;
            res.set_schema(tsc);
            for(int i = 0; i < tup_vec.size(); i++)
              {
                res.add(std::move(tup_vec[i]));
              }
            tuple_sets[0] = std::move(res);
            if(i < tuple_sets.size() - 1)
              tuple_sets[1] = std::move(tuple_sets[i+1]);
          }
        /// Pass 3.2. Selection under WHERE condition
        // for(const auto& it : cond_list)
        //   if(it.second == false)
        //     // Cross-table condition
        //     {
        //       check_tuple(tsc, tup_vec, it.first);
        //     }
        TupleSchema real_tsc;
        std::vector<Tuple> real_tup_vec;
        std::vector<int> order;
        for(int i = 0; i < selects.attr_num; i++)
          attr_to_number(tsc,
                         selects.attributes[i].relation_name,
                         selects.attributes[i].attribute_name,
                         tables, order);
        TupleSet tus;
        // result = new TupleSet;
        for(auto it : order)
          {
            real_tsc.add(tsc.field(it).type(),
                         tsc.field(it).table_name(),
                         tsc.field(it).field_name(), nullptr);
          }
        tus.set_schema(real_tsc);
        // result->set_schema(real_tsc);

        for(auto it : tuple_sets[0].tuples())
        {
            Tuple tp;
            for(auto itt : order)
            {
                tp.add(it.get_pointer(itt));
            }
            tus.add(std::move(tp));
            // result->add(std::move(tp));
        }
        if (selects.order_attr_num > 0)
            tus.order_by(&selects);
        RC rc = tus.group_by(&selects);
        if (rc != RC::SUCCESS)
            return rc;
        tus.print(ss, true);
    }
    else
    {
        if (selects.order_attr_num > 0) 
            tuple_sets.front().order_by(&selects);
        RC rc = tuple_sets.front().group_by(&selects);
        if (rc != RC::SUCCESS)
            return rc;
        tuple_sets.front().print(ss);
        //   if (aggregate)
        //   {
        //       printf("aggregate\n");
        //       const TupleSet *ori_tuple_sets = &tuple_sets.front();
        //       TupleSet agg_tuple_set;

        //       /// Pass 3.1. Make up schema for agg_tuple_set
        //       TupleSchema agg_schema;
        //       printf( COLOR_WHITE "[INFO] " COLOR_YELLOW "Make up schema for aggregate tuple set.\n");
        //       Table* table = DefaultHandler::get_default().find_table(db, selects.relations[0]);
        //       std::vector<AttrType> fields_type;
        //       int loc[selects.attr_num];
        //       for (int i = 0; i < selects.attr_num; ++i)
        //       {

        //           const RelAttr *attr = &selects.attributes[i];
        //           char *schema_field_name = new char[30];
        //           const char *schema_field_table = selects.relations[0];
        //           AttrType schema_field_type = UNDEFINED;

        //           loc[i] = i;
        //           for (int j = 0; j < i; ++j)
        //             if (strcmp(attr->attribute_name, selects.attributes[j].attribute_name) == 0)
        //             {
        //                 loc[i] = j;
        //                 break;
        //             }

        //           // make up schema_field_name
        //           strcpy(schema_field_name, attr->aggregate_func_name);
        //           strcat(schema_field_name, "(");
        //           strcat(schema_field_name, attr->attribute_name);
        //           strcat(schema_field_name, ")");

        //           // make up schema_field_type
        //           TableMeta table_meta_ = table->table_meta();
        //           const int normal_field_start_index = table_meta_.sys_field_num();
        //           const int attr_num = table_meta_.field_num() - normal_field_start_index;
        //           for (int j = 0; j < attr_num; ++j)
        //             {
        //                 const FieldMeta *field = table_meta_.field(j + normal_field_start_index);
        //                 if (strcmp(field->name(), attr->attribute_name) == 0)
        //                 {
        //                     schema_field_type = field->type();
        //                     break;
        //                 }
        //             }
        //             if (schema_field_type == UNDEFINED)
        //             {
        //                 if (attr->aggregate_func != AGG_COUNT)
        //                     return RC::INVALID_ARGUMENT;
        //             }
        //           // if aggregate function is count, then schema field type = INTS
        //           // if aggregate function is avg, then schema field type = FLOATS
        //           // if aggregate function is max or min, then schema field type is attributes' original type
        //           switch (attr->aggregate_func)
        //           {
        //             case AGG_COUNT:{
        //                 schema_field_type = INTS;
        //             }
        //             break;
        //             case AGG_AVG:{
        //                 schema_field_type = FLOATS;
        //             }
        //             break;
        //           }
        //           // add this attribute's schema
        //           agg_schema.add_if_not_exists(schema_field_type, schema_field_table, schema_field_name);
        //           fields_type.push_back(schema_field_type);
        //           delete[] schema_field_name;
        //       }
        //       // add schema to tupleset
        //       agg_tuple_set.set_schema(agg_schema);


        //       /// Pass 3.2. Add data to agg_tuple_set
        //       printf( COLOR_WHITE "[INFO] " COLOR_YELLOW "Add data to aggregate tuple set.\n");
        //       const std::vector<Tuple> ori_tuple_set = ori_tuple_sets->tuples();
        //       Tuple tuple;
        //       for (int i = 0; i < selects.attr_num; ++i)
        //       {
        //           printf("%d\n", loc[i]);
        //           const RelAttr *attr = &selects.attributes[i];
        //           const AttrType field_type = fields_type[i];

        //           int Count = 0;
        //           float Avg = -0.0;
        //           bool null_avg = true;
        //           std::shared_ptr<TupleValue> Max = nullptr;
        //           std::shared_ptr<TupleValue> Min = nullptr;

        //           for (auto it : ori_tuple_set)
        //           {
        //               Tuple *tuple = &it;
        //               const std::shared_ptr<TupleValue> tuple_value = tuple->get_pointer(loc[i]);
                      
        //               std::stringstream ss_;
        //               tuple_value->to_string(ss_);
        //               std::string s = ss_.str();
        //               ss_.clear();
        //               if (strcasecmp(s.c_str(), "null") == 0)
        //                 continue;

        //               ++Count;
        //               switch (attr->aggregate_func)
        //               {
        //                  case AGG_MAX:{
        //                     if (Max == nullptr)
        //                         Max = tuple_value;
        //                     else
        //                     {
        //                         if (Max->compare(*tuple_value) < 0)
        //                             Max = tuple_value;
        //                     }
        //                  } 
        //                  break;
        //                  case AGG_MIN:{
        //                     if (Min == nullptr)
        //                         Min = tuple_value;
        //                     else
        //                     {
        //                         if (Min->compare(*tuple_value) > 0)
        //                             Min = tuple_value;
        //                     }
        //                  }
        //                  break;
        //                  case AGG_AVG:{
        //                     null_avg = false;
        //                     float value_float = float_from_string(s);
        //                     Avg += value_float;
        //                  }
        //                  break;
        //               }
        //           }
        //           if (Count != 0)
        //           {
        //               Avg /= (float)Count;
        //               int temp = (int)(Avg * 100 + 0.5);
        //               Avg = (float)temp / 100.0;
        //           }
                      

        //           TupleValue* tuple_value;
        //           const char *nullstmp = new char[5];
        //           nullstmp = "NULL";
        //           switch (attr->aggregate_func)
        //           {
        //               case AGG_COUNT:{
        //                   tuple.add(Count);
        //               }
        //               break;
        //               case AGG_MAX:{
        //                   if (Max == nullptr)
        //                     tuple.add(nullstmp, strlen(nullstmp));
        //                   else
        //                     tuple.add(Max);
        //               }
        //               break;
        //               case AGG_MIN:{
        //                   if (Min == nullptr)
        //                     tuple.add(nullstmp, strlen(nullstmp));
        //                   else
        //                     tuple.add(Min);
        //               }
        //               break;
        //               case AGG_AVG:{
        //                   if (null_avg == true)
        //                     tuple.add(nullstmp, strlen(nullstmp));
        //                   else
        //                     tuple.add(Avg);
        //               }
        //               break;
        //               default:
        //                 assert(0);
        //               break;
        //           }
        //         }
        //       agg_tuple_set.add(std::move(tuple));
        //       agg_tuple_set.print(ss);
        //       fflush(stdout);
        //   }
        //   else  // No aggregate functions
        //   {
        //         printf("no aggregate\n");
        //         // ???????????????????????????????????????????????????
        //         if (selects.order_attr_num > 0) 
        //             tuple_sets.front().order_by(&selects);
        //         tuple_sets.front().print(ss);
        //   }
    }
    fflush(stdout);
    
    for (SelectExeNode *&tmp_node: select_nodes)
      {
        delete tmp_node;
      }
    session_event->set_response(ss.str());
    end_trx_if_need(session, transaction, true);
    return RC::SUCCESS;
  }
  assert(0);
}


std::pair<RC, std::string> Resolve_Attr_Scope(
                                              std::list<RelAttr*> attr_array,
                                              Table* table
                                              )
{
  for (auto p = attr_array.begin();
       p != attr_array.end();
       ++p)
    {
      assert(*p != nullptr);

             
      RelAttr& attr = **p;
      printf(COLOR_WHITE "[INFO] " COLOR_YELLOW "Trying to find attachment for attribute "
             COLOR_GREEN "%s.%s" COLOR_YELLOW ".\n",
             attr.relation_name,
             attr.attribute_name);
      if (attr.relation_name != nullptr)
        {
          if(strcmp(table->name(), attr.relation_name) == 0)
            {
              assert(attr.related_table == nullptr);
              /// This attribute shouldn't attach 
                /// to any table 
                attr.related_table = table;

                if (table->table_meta().field(attr.attribute_name) == nullptr &&
                    strcmp(attr.attribute_name, "*") != 0)
                  /// Not Exist
                  {
                    char str[256];
                    sprintf(str, COLOR_RED "[ERROR] " COLOR_YELLOW
                            "The table " COLOR_GREEN "%s"
                            COLOR_YELLOW " don't have attribute "
                            COLOR_GREEN "%s" COLOR_YELLOW ".\n",
                            table->name(), attr.attribute_name);
                    LOG_ERROR("%s", str);
                    return { RC::SCHEMA_FIELD_NOT_EXIST,
                             std::string(str) };
                  }
            }
        }
      else /// Headless attach
        {
          if(strcmp(attr.attribute_name, "*") == 0)
            /// Star, this selection shouldn't be attached to any table.
            {
              // attr.related_table = table;
            }
          /// This method is of LOW PERFORMANCE
          else if (table->table_meta().field(attr.attribute_name) != nullptr)
            {
              if (attr.related_table != nullptr && attr.related_table != table)
                /// failed
                {
                  char str[256];
                  sprintf(str, COLOR_RED "[ERROR] " COLOR_YELLOW
                          "The attribute " COLOR_GREEN "%s"
                          COLOR_YELLOW " has ambigious reference.",
                          attr.attribute_name);
                  LOG_ERROR("%s", str);
                  return { RC::SCHEMA_FIELD_REDUNDAN,
                           std::string(str) };

                }
              attr.related_table = table;
              attr.relation_name = (char*) table->name();
            }
        }
    }
  return {RC::SUCCESS, ""};

}


bool match_table(const Selects &selects, const char *table_name_in_condition, const char *table_name_to_match)
{
  if (table_name_in_condition != nullptr)
    {
      return 0 == strcmp(table_name_in_condition, table_name_to_match);
    }

  return selects.relation_num == 1;
  /// Buggy, but I don't want to modify it anymore.
}

static RC schema_add_field(Table *table, const char *field_name, TupleSchema &schema)
{
  const FieldMeta *field_meta = table->table_meta().field(field_name);
  if (nullptr == field_meta)
    {
      LOG_WARN("No such field. %s.%s", table->name(), field_name);
      return RC::SCHEMA_FIELD_MISSING;
    }

  schema.add_if_not_exists(field_meta->type(), table->name(), field_meta->name(), nullptr);
  return RC::SUCCESS;
}


std::pair<RC, std::string>
Create_Select_Table_Executor(Trx* transaction,                
                             Table* table,                    
                             const std::list<RelAttr*>& attr_list,  
                             std::list<std::pair<Condition*, bool>>& cond_list,
                             SelectExeNode& select_node)
{
  TupleSchema schema;
  // This is our final selection result for this table
  
  /**
   * Pass 1. Find attributes to select
   **/
  // Perform selection
  for(auto it : attr_list)
    {
      if(it->related_table == table ||
         (strcmp("*", it->attribute_name) == 0 &&
          it->related_table == nullptr))
        {
          if(strcmp("*", it->attribute_name) == 0)
            {
              TupleSchema::from_table(table, schema);
              // Add the whole table to selection range.
            }
          else
            {
                // Add this attribute to selection range
              RC rc = schema_add_field(table, it->attribute_name, schema);
              if(rc != RC::SUCCESS) // Failed
                {
                  char buf[256];
                  sprintf(buf, COLOR_RED "[ERROR] " COLOR_YELLOW "Missing attribute "
                          COLOR_GREEN "%s" COLOR_YELLOW " from table "
                          COLOR_GREEN "%s\n",
                          it->attribute_name, table->name());
                  return {rc, buf};
                }
            }
        
        }
    }

  /**
   * Pass 2. Collect conditional filters
   **/
  std::vector<DefaultConditionFilter *> condition_filters;
  for(auto& it : cond_list)
    {
      /// More check, more happy
      assert(it.first->left_is_attr == 0 ||
             it.first->left_attr.related_table != nullptr);

      assert(it.first->right_is_attr == 0 ||
             it.first->right_attr.related_table != nullptr);
      
      if(
         ( it.first->left_is_attr == 0 && it.first->right_is_attr == 0 ) ||
         
         ( it.first->left_is_attr == 1 && it.first->right_is_attr == 0
           && it.first->left_attr.related_table == table) ||

         ( it.first->left_is_attr == 0 && it.first->right_is_attr == 1
           && it.first->right_attr.related_table == table) ||

         ( it.first->left_is_attr == 1 && it.first->right_is_attr == 1
           && it.first->left_attr.related_table == table
           && it.first->right_attr.related_table == table)
         )
        /// Local to current table
        {
          it.second = true;
          DefaultConditionFilter *condition_filter = new DefaultConditionFilter();
          RC rc = condition_filter->init(*table, *it.first);
          if (rc != RC::SUCCESS)
            {
              delete condition_filter;
              for (DefaultConditionFilter *&filter : condition_filters)
                {
                  delete filter;
                }
              return {rc, "UNKNOWN ERROR"};
            }
          condition_filters.push_back(condition_filter);
        }
    }
  return {
    select_node.init(transaction, table, std::move(schema), std::move(condition_filters)),
    ""};
}

void dfs_cartesian(const std::vector<TupleSet>& ts,
                   int ts_depth,
                   int max_depth,
                   Tuple& current_tuple,
                   const std::vector<Arbiter>& arbiters,
                   std::vector<Tuple>& output)
{
  // if(ts_depth <= 2)
  //   {
  //     printf("Checking: ");
  //     print_tuple(current_tuple);
  //   }
  if(ts_depth == max_depth)
    {
      auto ct = current_tuple;
      for(const auto& it : arbiters)
        {
          if(!it.judge(ct))
            return;
        }
      output.emplace_back(std::move(ct));
      return;
    }

  for(const auto& it : ts[ts_depth].tuples())
    {
      for(const auto& p : it.values())
        current_tuple.add(p);
      dfs_cartesian(ts, ts_depth+1, max_depth, current_tuple, arbiters, output);
      for(int i = 0; i < it.size(); i++)
        current_tuple.pop_back();
    }
}

bool match_result(CompOp oper, int result)
{
  switch(oper)
    {
    case EQUAL_TO:
      return result == 0;
    case LESS_EQUAL:
      return result <= 0;
    case NOT_EQUAL:
      return result != 0;
    case LESS_THAN:
      return result  < 0;
    case GREAT_EQUAL:
      return result >= 0;
    case GREAT_THAN:
      return result  > 0;
    default:
      {
        assert(0);
      }
    }
}

TupleValue* build_value(Value* immediate)
{

  TupleValue* tv;
  switch(immediate->type)
    {
    case CHARS:
      {
        tv = new StringValue((char*)immediate->data,
                             strlen((char*)immediate->data));
        return tv;
      }
    case INTS:
      {
        tv = new IntValue(*(int*)immediate->data);
        return tv;
      }
    case FLOATS:
      {
        tv = new FloatValue(*(float*)immediate->data);
        return tv;
      }
    case DATES:
      assert(0); // Not implemented yet.
    case UNDEFINED:
      assert(0);
    }
  
}


Arbiter::Arbiter(Condition* rule, const TupleSchema& tsc)
{
  lhs = -1, rhs = -1;
  this->valid = 1;
    if(!rule->left_is_attr)
    lhs = -2; // Value type
  if(!rule->right_is_attr)
    rhs = -2;

  // if(lhs == -2 && rhs == -2) // SPJ: Const
  //   {assert(0);}
  // else
  
  for(int i = 0; i < tsc.fields().size(); i++)
    {
      auto& it = tsc.fields()[i];
      if(lhs == -1 &&
         strcmp(it.field_name(),rule->left_attr.attribute_name) == 0 &&
         strcmp(it.table_name(),rule->left_attr.relation_name) == 0)
        {
          lhs = i;
        }
      if(rhs == -1 &&
         strcmp(it.field_name(),rule->right_attr.attribute_name) == 0 &&
         strcmp(it.table_name(),rule->right_attr.relation_name) == 0)
        {
          rhs = i;
        }
    }
  if(lhs == -1 || rhs == -1)
    {
      LOG_ERROR(COLOR_WHITE "[INFO] "
                COLOR_YELLOW "A field specified in WHERE clause seems not exist in any table.");
      this->valid = 0;
      printf("Rule: ");
      
      print_cond(rule);
      printf("\n");
      return;
    }
  
  if(lhs == -2) ltv = build_value(&rule->left_value);
  if(rhs == -2) rtv = build_value(&rule->right_value);
  
  // 2. Type Check
  AttrType ltype = (lhs == -2)?rule->left_value.type :tsc.fields()[lhs].type();
  AttrType rtype = (rhs == -2)?rule->right_value.type:tsc.fields()[rhs].type();
  if(ltype != rtype)
    {
      LOG_ERROR(COLOR_RED "[ERROR] "
                COLOR_YELLOW "The type of a WHERE clause mismatches. "
                COLOR_GREEN "%d" COLOR_YELLOW " <-> "
                COLOR_GREEN "%d" COLOR_YELLOW "\n",
                ltype, rtype);
      print_cond(rule);
      fflush(stdout);
      assert(0);
    }

  this->cmpop = rule->comp;
};

bool Arbiter::judge(const Tuple &tup) const
{
  if(this->valid == 0)
    return true;
#define GET(lr,cnt) cnt >= 0? &tup.get(cnt) : (lr ## tv)
  const TupleValue* lval = GET(l,lhs);
  const TupleValue* rval = GET(r,rhs);

  std::stringstream ssl,ssr;
  lval->to_string(ssl);
  rval->to_string(ssr);
  int lnull = ! strcasecmp(ssl.str().c_str(), "null");
  int rnull = ! strcasecmp(ssr.str().c_str(), "null");
  
  #define CMP(infix, cond, res) \
  {\
    if(this->cmpop == infix && cond)\
    {\
    return res;\
    }\
  }
  CMP(IS_NULL,lnull && rnull, true);
  CMP(IS_NULL, lnull && !rnull, false);
    CMP(IS_NULL, !lnull && rnull, false);
  CMP(IS_NOT_NULL,lnull && rnull, false);
  CMP(IS_NOT_NULL, lnull && !rnull, true);
    CMP(IS_NOT_NULL, !lnull && rnull, true);
    if(lnull || rnull) return false;
  #undef CMP
  
  if(match_result(this->cmpop, lval->compare(*rval)))
    {
      return true;
    }
  else
    {
      return false;
    }
#undef GET
}

void attr_to_number(const TupleSchema& tsc,
                    const char* relation_name,
                    const char* attribute_name,
                    const std::vector<Table*>& tables,
                    std::vector<int>& numbers)
{
  printf("Trying to attach to number for %s.%s\n",
         relation_name, attribute_name);
  if(strcmp(attribute_name, "*") == 0)
    {
      if(relation_name == nullptr)
        for(int i = 0; i < tables.size(); i++)
          {
            for(int j = 1; j < tables[i]->table_meta().field_num(); j++)
              {
                attr_to_number(tsc,
                               tables[i]->name(),
                               tables[i]->table_meta().field(j)->name(),
                               tables,
                               numbers);
              }
          }
      else
        {
          bool found = false;
          for(int i = 0; i < tables.size(); i++)
          {
            if(strcmp(tables[i]->name(), relation_name) == 0)
              {
                for(int j = 1; j < tables[i]->table_meta().field_num(); j++)
                  {
                    attr_to_number(tsc,
                                   tables[i]->name(),
                                   tables[i]->table_meta().field(j)->name(),
                                   tables,
                                   numbers);
                  }
                found = true;
                break;
              }
          
        }
        assert(found);
        }
    }
  else
    {
      for(int i = 0; i < tsc.fields().size(); i++)
        {
          const auto& it = tsc.fields().at(i);
          if(strcmp(it.table_name(), relation_name) == 0 &&
             strcmp(it.field_name(), attribute_name) == 0)
            {
              numbers.emplace_back(i);
              return;
            }
        }
      
      assert(0); // Should not reach here.
    }
}

float float_from_string(std::string s)
{
    float re = 0.0;
    int len = s.length();
    int p = -1;
    for (int i = 0; i < len; ++i)
        if (s[i] == '.')
        {
            p = i;
            break;
        }
    if (p < 0)
        p = len;
    float inte = 0.0;
    float deci = 0.0;
    for (int i = 0; i < p; ++i)
        inte = inte * 10 + s[i] - '0';
    for (int i = len - 1; i > p; --i)
        deci = deci / 10.0 + 0.1 * (float)(s[i] - '0');
    re = inte + deci;
    return re;
}
