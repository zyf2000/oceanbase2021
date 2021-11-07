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

RC create_selection_executor(Trx *trx, Selects &selects, const char *db, const char *table_name, SelectExeNode &select_node);

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
                             const std::list<Condition*>& cond_list, // Condition list
                             SelectExeNode& select_node);     // [Select Node]

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


// 这里没有对输入的某些信息做合法性校验，比如查询的列名、where条件中的列名等，没有做必要的合法性校验
// 需要补充上这一部分. 校验部分也可以放在resolve，不过跟execution放一起也没有关系
RC ExecuteStage::do_select(const char *db, Query *sql, SessionEvent *session_event)
{
  RC rc = RC::SUCCESS;
  Session *session = session_event->get_client()->session;
  Trx *trx = session->current_trx();
  Selects &selects = sql->sstr.selection;
  // 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
  std::vector<SelectExeNode *> select_nodes;

  for (size_t i = 0; i < selects.relation_num; i++)
    {
      const char *table_name = selects.relations[i];
      SelectExeNode *select_node = new SelectExeNode;

      rc = create_selection_executor(trx, selects, db, table_name, *select_node);
      /// NOTE: the [WHERE] clause is evaluated in this function

      if (rc != RC::SUCCESS)
        {
          delete select_node;
          for (SelectExeNode *&tmp_node: select_nodes)
            {
              delete tmp_node;
            }
          end_trx_if_need(session, trx, false);
          return rc;
        }
      select_nodes.push_back(select_node);
    }
  /// Check used count
  for (int i = 0; i < selects.attr_num; i++)
    {
      if (selects.attributes[i].used_count != 1)
        {
          char str[256];
          sprintf(str, COLOR_RED "[ERROR] " COLOR_YELLOW
                  "The attribute " COLOR_GREEN "%s"
                  COLOR_YELLOW " is referenced for "
                  COLOR_GREEN "%d" COLOR_YELLOW " times.\n"
                  COLOR_NONE,
                  selects.attributes[i].attribute_name,
                  selects.attributes[i].used_count);
          printf("%s", str);
          session_event->set_response(str);
          if (selects.attributes[i].used_count == 0)
            {
              return RC::SCHEMA_FIELD_NOT_EXIST;
            } else
            {
              return RC::SCHEMA_FIELD_REDUNDAN;
            }
        }
    }


  /// If there is no table given
  if (select_nodes.empty())
    {
      LOG_ERROR("No table given");
      end_trx_if_need(session, trx, false);
      return RC::SQL_SYNTAX;
    }


  std::vector<TupleSet> tuple_sets;
  for (SelectExeNode *&node: select_nodes)
    {
      TupleSet tuple_set;
      rc = node->execute(tuple_set);
      if (rc != RC::SUCCESS)
        {
          for (SelectExeNode *&tmp_node: select_nodes)
            {
              delete tmp_node;
            }
          end_trx_if_need(session, trx, false);
          return rc;
        } else
        {
          tuple_sets.push_back(std::move(tuple_set));
        }
    }

  std::stringstream ss;
  if (tuple_sets.size() > 1)
    {
      // 本次查询了多张表，需要做join操作
      /// TODO: JOIN
    } else
    {
      // 当前只查询一张表，直接返回结果即可
      tuple_sets.front().print(ss);
    }

  for (SelectExeNode *&tmp_node: select_nodes)
    {
      delete tmp_node;
    }
  session_event->set_response(ss.str());
  end_trx_if_need(session, trx, true);
  return rc;
}

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
  std::reverse(selects.attributes, selects.attributes + selects.attr_num);
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
  {
    /// Collect all attributes.
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
        
    for(size_t i = 0;
        i < selects.relation_num;
        i++)
      {
        Table* table = DefaultHandler::get_default().find_table(db, selects.relations[i]);
        if(table == nullptr)
          {
            LOG_ERROR(COLOR_RED "[ERROR] " COLOR_YELLOW "No such table ["
                 COLOR_GREEN "%s" COLOR_YELLOW "] in db ["
                 COLOR_GREEN "%s" COLOR_YELLOW "]\n", selects.relations[i], db);
            end_trx_if_need(session, transaction, false);
            return RC::SCHEMA_TABLE_NOT_EXIST;
          }
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
        if(it->related_table == nullptr)
          {
          printf(COLOR_RED "[ERROR] " COLOR_YELLOW "Attribute ["
                 COLOR_GREEN "%s.%s" COLOR_YELLOW "] is not attached in any table.\n",
                 it->relation_name != nullptr?it->relation_name:"(NULL)",
                 it->attribute_name);
          end_trx_if_need(session, transaction, false);
          return RC::SCHEMA_FIELD_NOT_EXIST;
                
          }
      }
    
  }

  /**
   * Pass 2, Mapping the selection operation and local WHERE statements to each table
   **/
  std::vector<SelectExeNode*> select_nodes;
  
  {
    // Collect all attributions and conditions
    std::list<RelAttr*> attr_list;
    for (int i = 0; i < selects.attr_num; i++)
      {
        attr_list.push_back(&selects.attributes[i]);
      }
    std::list<Condition*> cond_list;
    for(int i = 0; i < selects.condition_num; i++)
      {
        cond_list.push_back(&selects.conditions[i]);
      }
    for(size_t i = 0;
        i < selects.relation_num;
        i++)
      {
        SelectExeNode *select_node = new SelectExeNode;
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
    
    std::stringstream ss;
    if (tuple_sets.size() > 1)
      {
        /// TODO: JOIN
      }
    else
      {
        // 当前只查询一张表，直接返回结果即可
        tuple_sets.front().print(ss);
      }

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
      printf(COLOR_WHITE "[INFO] " COLOR_YELLOW "Trying to find attachment for attribute %s.\n",
             attr.attribute_name);
      if (attr.relation_name != nullptr)
        {
          if(strcmp(table->name(), attr.relation_name) == 0)
            {
              assert(attr.related_table == nullptr);
              /// This attribute shouldn't attach 
                /// to any table 
                attr.related_table = table;
            
                if (table->table_meta().field(attr.attribute_name) == nullptr)
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
            /// Star
            {
              attr.related_table = table;
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

  schema.add_if_not_exists(field_meta->type(), table->name(), field_meta->name());
  return RC::SUCCESS;
}

// 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
/// Do selection on each table(relation) specified.
RC create_selection_executor(Trx *trx,                    /// Transaction
                             Selects &selects,      /// Select Description
                             const char *db,              /// Database
                             const char *table_name,      /// Such a relation
                             SelectExeNode &select_node)  /// (Ref to) Select Node
{
  // 列出跟这张表关联的Attr
  TupleSchema schema;
  /// This is the final returning value

  Table *table = DefaultHandler::get_default().find_table(db, table_name);
  if (nullptr == table)
    {
      LOG_ERROR(COLOR_RED "[ERROR] " COLOR_YELLOW "No such table ["
                COLOR_GREEN "%s" COLOR_YELLOW "] in db ["
                COLOR_GREEN "%s" COLOR_YELLOW "]", table_name, db);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }

  /// Perform Selection
  {
    bool star_flag = false;
    bool data_flag = false;

    for (int i = selects.attr_num - 1; i >= 0; i--)
      {
        RelAttr &attr = selects.attributes[i];
        if (nullptr == attr.relation_name || 0 == strcmp(table_name, attr.relation_name))
          /// The field corresponds to current table
          {
            if (0 == strcmp("*", attr.attribute_name))
              {
                // 列出这张表所有字段
                /// Checking
                if (star_flag || data_flag)
                  {
                    return RC::INVALID_ARGUMENT;
                  }
                TupleSchema::from_table(table, schema);
                star_flag = true;
              } else
              {
                // 列出这张表相关字段
                RC rc = schema_add_field(table, attr.attribute_name, schema);
                data_flag = true;
                if (rc != RC::SUCCESS)
                  {
                    /// Special treatment for multi-table SELECTs
                    if (rc == RC::SCHEMA_FIELD_MISSING && attr.relation_name == nullptr)
                      {
                        continue;
                      }
                    printf(COLOR_RED "[ERROR] " COLOR_YELLOW "Missing attribute "
                           COLOR_GREEN "%s" COLOR_YELLOW " from table "
                           COLOR_GREEN "%s\n",
                           attr.attribute_name, attr.relation_name);
                    return rc;
                  }
                if (star_flag)
                  {
                    return RC::INVALID_ARGUMENT;
                  }

              }
            attr.used_count++;
          }
      }
  }

  // 找出仅与此表相关的过滤条件, 或者都是值的过滤条件
  /// WHERE clause
  std::vector<DefaultConditionFilter *> condition_filters;
  for (size_t i = 0; i < selects.condition_num; i++)
    {
      const Condition &condition = selects.conditions[i];
      if (
          (condition.left_is_attr == 0 && condition.right_is_attr == 0) ||
          /// [value] [OP] [value]
          (condition.left_is_attr == 1 && condition.right_is_attr == 0
           && match_table(selects, condition.left_attr.relation_name, table_name)) ||
          /// [attr] [OP] [value]
          (condition.left_is_attr == 0 && condition.right_is_attr == 1
           && match_table(selects, condition.right_attr.relation_name, table_name)) ||
          /// [value] OP [attr]
          (condition.left_is_attr == 1 && condition.right_is_attr == 1
           && match_table(selects, condition.left_attr.relation_name, table_name)
           && match_table(selects, condition.right_attr.relation_name, table_name))
          /// [attr] OP [attr]
          )
        {
          DefaultConditionFilter *condition_filter = new DefaultConditionFilter();
          RC rc = condition_filter->init(*table, condition);
          if (rc != RC::SUCCESS)
            {
              delete condition_filter;
              for (DefaultConditionFilter *&filter : condition_filters)
                {
                  delete filter;
                }
              return rc;
            }
          condition_filters.push_back(condition_filter);
        }
    }

  return select_node.init(trx, table, std::move(schema), std::move(condition_filters));
}

std::pair<RC, std::string>
Create_Select_Table_Executor(Trx* transaction,                
                             Table* table,                    
                             const std::list<RelAttr*>& attr_list,  
                             const std::list<Condition*>& cond_list,
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
      if(it->related_table == table)
        {
          if(strcmp("*", it->attribute_name) == 0)
            {
              TupleSchema::from_table(table, schema);
              // Add the whole table to selection range.
            }
          else
            {
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
  for(auto it : cond_list)
    {
      /// More check, more happy
      assert(it->left_is_attr == 0 ||
             it->left_attr.related_table != nullptr);

      assert(it->right_is_attr == 0 ||
             it->right_attr.related_table != nullptr);

      if(
         ( it->left_is_attr == 0 && it->right_is_attr == 0 ) ||
         
         ( it->left_is_attr == 1 && it->right_is_attr == 0
           && it->left_attr.related_table == table) ||

         ( it->left_is_attr == 0 && it->right_is_attr == 1
           && it->right_attr.related_table == table) ||

         ( it->left_is_attr == 1 && it->right_is_attr == 1
           && it->left_attr.related_table == table
           && it->right_attr.related_table == table)
         )
        /// Local to current table
        {
          DefaultConditionFilter *condition_filter = new DefaultConditionFilter();
          RC rc = condition_filter->init(*table, *it);
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
