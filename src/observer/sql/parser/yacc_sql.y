
%{

#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.tab.h"
#include "sql/parser/lex.yy.h"
// #include "common/log/log.h" // 包含C++中的头文件

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct ParserContext {
  Query * ssql;
  size_t select_length;
  size_t condition_length;
  size_t from_length;
  size_t value_length;
  size_t insert_tuple_length;
  Value values[MAX_NUM];
  Condition conditions[MAX_NUM];
  CompOp comp;
  char id[MAX_NUM];
  InsertTuple insert_tuples[MAX_NUM];
} ParserContext;

//获取子串
char *substr(const char *s,int n1,int n2)/*从s中提取下标为n1~n2的字符组成一个新字符串，然后返回这个新串的首地址*/
{
  char *sp = malloc(sizeof(char) * (n2 - n1 + 2));
  int i, j = 0;
  for (i = n1; i <= n2; i++) {
    sp[j++] = s[i];
  }
  sp[j] = 0;
  return sp;
}

void yyerror(yyscan_t scanner, const char *str)
{
  ParserContext *context = (ParserContext *)(yyget_extra(scanner));
  query_reset(context->ssql);
  context->ssql->flag = SCF_ERROR;
  context->condition_length = 0;
  context->from_length = 0;
  context->select_length = 0;
  context->value_length = 0;
  context->ssql->sstr.insertion.insert_tuple_num = 0;
  printf("parse sql failed. error=%s\n", str);
}

ParserContext *get_context(yyscan_t scanner)
{
  return (ParserContext *)yyget_extra(scanner);
}

#define CONTEXT get_context(scanner)

%}

%define api.pure full
%lex-param { yyscan_t scanner }
%parse-param { void *scanner }

//标识tokens
%token  SEMICOLON
        CREATE
        DROP
        TABLE
        TABLES
        INDEX
        UNIQUE
        SELECT
        INNER
        JOIN
        DESC
        ASC
        SHOW
        SYNC
        INSERT
        DELETE
        UPDATE
        LBRACE
        RBRACE
        COMMA
        TRX_BEGIN
        TRX_COMMIT
        TRX_ROLLBACK
        NULLABLE
        IS
        NOT
//        NULL_T
        INT_T
        STRING_T
        FLOAT_T
        DATE_T
        TEXT_T
        HELP
        EXIT
        DOT //QUOTE
        INTO
        VALUES
        FROM
        WHERE
        ORDER
        GROUP
        BY
        AND
        SET
        ON
        LOAD
        DATA
        INFILE
        EQ
        LT
        GT
        LE
        GE
        NE

%union {
  struct _Attr *attr;
  struct _Condition *condition1;
  struct _Value *value1;
  char *string;
  int number;
  float floats;
  char *position;
  struct _Vector* ve;
}

%token <number> NUMBER
%token <floats> FLOAT 
%token <string> ID
%token <string> PATH
%token <string> DATESSS
%token <string> SSS
%token <string> NULL_T
%token <string> STAR
%token <string> STRING_V
//非终结符

%type <number> type;
%type <condition1> condition;
%type <value1> value;
%type <number> number;
%type <number> whether_null;
%type <number> cmp;
%type <ve> id_list;
%%

commands:		//commands or sqls. parser starts here.
    /* empty */
    | commands command
    ;

command:
	  select  
	| insert
	| update
	| delete
	| create_table
	| drop_table
	| show_tables
	| desc_table
	| create_index	
	| drop_index
	| sync
	| begin
	| commit
	| rollback
	| load_data
	| help
	| exit
    ;

exit:			
    EXIT SEMICOLON {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    };

help:
    HELP SEMICOLON {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    };

sync:
    SYNC SEMICOLON {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
    ;

begin:
    TRX_BEGIN SEMICOLON {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
    ;

commit:
    TRX_COMMIT SEMICOLON {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
    ;

rollback:
    TRX_ROLLBACK SEMICOLON {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
    ;

drop_table:		/*drop table 语句的语法解析树*/
    DROP TABLE ID SEMICOLON {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, $3);
    };

show_tables:
    SHOW TABLES SEMICOLON {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
    ;

desc_table:
    DESC ID SEMICOLON {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, $2);
    }
    ;

create_index:		/*create index 语句的语法解析树*/
    CREATE INDEX ID ON ID LBRACE ID RBRACE SEMICOLON 
		{
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, $3, $5, $7, 0);
		}
    |CREATE INDEX ID ON ID LBRACE ID id_list RBRACE SEMICOLON
  // [1]    [2]   [3][4][5][6]    [7][8]             
    {
        if($8->len < 32)
            {
                $8->data[$8->len] = $7;
                $8->len++;
            }
        else
                {
                    exit(-1);
                }
        create_multi_index_init(
            &CONTEXT->ssql->sstr.create_multi_index,
            $3, $5, $8->data, $8->len, 0);
        CONTEXT->ssql->flag = SCF_CREATE_MULTI_INDEX;
    }
    |CREATE UNIQUE INDEX ID ON ID LBRACE ID RBRACE SEMICOLON
        {
            CONTEXT->ssql->flag = SCF_CREATE_INDEX;
            create_index_init(&CONTEXT->ssql->sstr.create_index, $4, $6, $8, 1);
        }
    ;
id_list: COMMA ID {
             $$ = (Vector*) malloc(sizeof(Vector*));
             memset($$,0,sizeof(Vector*));
             $$->len = 0;
             $$->data = (char**) malloc(32*sizeof(char*));
             memset($$->data,0,32*sizeof(char*));
             if($$->len < 32)
                 {
                     $$->data[$$->len] = $2;
                     $$->len++;
                }
                else
                {
                    exit(-1);
                }
    }
    |   COMMA ID id_list
                {
                    $$ = $3;
                    if($$->len < 32)
                        {
                            $$->data[$$->len] = $2;
                            $$->len++;
                }
                else
                {
                    exit(-1);
                }
                }
                ;
drop_index:			/*drop index 语句的语法解析树*/
    DROP INDEX ID  SEMICOLON 
		{
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, $3);
		}
    ;
create_table:		/*create table 语句的语法解析树*/
    CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE SEMICOLON 
		{
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			// CONTEXT->ssql->sstr.create_table.attribute_count = CONTEXT->value_length;
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, $3);
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
    ;
attr_def_list:
    /* empty */
    | COMMA attr_def attr_def_list {    }
    ;
    
attr_def:
    ID_get type LBRACE number RBRACE whether_null
		{
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, $2, $4, $6);
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name =(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type = $2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length = $4;
			CONTEXT->value_length++;
		}
    |ID_get type whether_null
		{
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, $2, 5, $3);
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name=(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type=$2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length=4; // default attribute length
			CONTEXT->value_length++;
		}
    ;
whether_null:
    /* empty */ { $$ = 0; }
    | NOT NULL_T { $$ = 0; }
    | NULLABLE { $$ = 1; }
    ;
number:
		NUMBER {$$ = $1;}
		;
type:
	INT_T { $$=INTS; }
       | STRING_T { $$=CHARS; }
       | FLOAT_T { $$=FLOATS; }
       | DATE_T { $$=DATES; }
       | TEXT_T { $$=TEXTS; }
       ;
ID_get:
	ID 
	{
		char *temp=$1; 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
	;

	
insert:				/*insert   语句的语法解析树*/
    // INSERT INTO ID VALUES LBRACE value value_list RBRACE SEMICOLON
    INSERT INTO ID VALUES insert_tuple insert_tuple_list SEMICOLON 
		{
			// CONTEXT->values[CONTEXT->value_length++] = *$6;

			CONTEXT->ssql->flag=SCF_INSERT;//"insert";
			// CONTEXT->ssql->sstr.insertion.relation_name = $3;
			// CONTEXT->ssql->sstr.insertion.value_num = CONTEXT->value_length;
			// for(i = 0; i < CONTEXT->value_length; i++){
			// 	CONTEXT->ssql->sstr.insertion.values[i] = CONTEXT->values[i];
      // }
			// inserts_init(&CONTEXT->ssql->sstr.insertion, $3, CONTEXT->values, CONTEXT->value_length);
            inserts_init(&CONTEXT->ssql->sstr.insertion, $3, CONTEXT->insert_tuples, CONTEXT->insert_tuple_length);

      //临时变量清零
      CONTEXT->insert_tuple_length = 0;
    };
insert_tuple_list:
    /* empty */
    | COMMA insert_tuple insert_tuple_list
        {}
    ;
insert_tuple:
    LBRACE value value_list RBRACE
    {
        inserts_tuple_append_value(&CONTEXT->insert_tuples[CONTEXT->insert_tuple_length++], CONTEXT->values, CONTEXT->value_length);
        CONTEXT->value_length = 0;
    }

value_list:
    /* empty */
    | COMMA value value_list  { 
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
    ;
value:
    NUMBER{	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], $1);
		}
    |FLOAT{
  		value_init_float(&CONTEXT->values[CONTEXT->value_length++], $1);
		}
    |SSS {
        $1 = substr($1,1,strlen($1)-2);
  		value_init_string(&CONTEXT->values[CONTEXT->value_length++], $1);
		}
    |DATESSS {
        $1 = substr($1,1,strlen($1)-2);
        value_init_dates(&CONTEXT->values[CONTEXT->value_length++], $1);
    }
    |NULL_T {
        value_init_null(&CONTEXT->values[CONTEXT->value_length++], $1);
    }
    ;
    
delete:		/*  delete 语句的语法解析树*/
    DELETE FROM ID where SEMICOLON 
		{
			CONTEXT->ssql->flag = SCF_DELETE;//"delete";
			deletes_init_relation(&CONTEXT->ssql->sstr.deletion, $3);
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;	
    }
    ;
update:			/*  update 语句的语法解析树*/
    UPDATE ID SET ID EQ value where SEMICOLON
		{
			CONTEXT->ssql->flag = SCF_UPDATE;//"update";
			Value *value = &CONTEXT->values[0];
			updates_init(&CONTEXT->ssql->sstr.update, $2, $4, value, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;
		}
    ;
select:				/*  select 语句的语法解析树*/
    SELECT select_attr FROM relation_container where group order SEMICOLON
		{
			// CONTEXT->ssql->sstr.selection.relations[CONTEXT->from_length++]=$4;

			selects_append_conditions(&CONTEXT->ssql->sstr.selection, CONTEXT->conditions, CONTEXT->condition_length);

			CONTEXT->ssql->flag=SCF_SELECT;//"select";
			// CONTEXT->ssql->sstr.selection.attr_num = CONTEXT->select_length;

			//临时变量清零
			CONTEXT->condition_length=0;
			CONTEXT->from_length=0;
			CONTEXT->select_length=0;
			CONTEXT->value_length = 0;
        }
	;

relation_container:
                ID join_container
                {
                    selects_append_relation(&CONTEXT->ssql->sstr.selection, $1);
                }
        |       ID join_container COMMA relation_container
                {
                    selects_append_relation(&CONTEXT->ssql->sstr.selection, $1);
                }
        ;

join_container:
                /* Empty */
                {}
        |       INNER JOIN ID ON condition condition_list join_container
                {
                    selects_append_relation(&CONTEXT->ssql->sstr.selection, $3);
                }
                
// SELECT *
// SELECT id
// SELECT max(count)
// SELECT count(*)
// SELECT count(1)
select_attr:    select_attr_expr attr_list  // Multiple
                {}
        ;

select_attr_expr: STAR
                {
                    RelAttr attr;
                    relation_attr_init(&attr, NULL, "*", NULL);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
        |       ID
                {
                    RelAttr attr;
                    relation_attr_init(&attr, NULL, $1, NULL);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
        |       ID DOT ID
                {
                    RelAttr attr;
                    relation_attr_init(&attr, $1, $3, NULL);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
        |       ID DOT STAR
                {
                    RelAttr attr;
                    relation_attr_init(&attr, $1, "*", NULL);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
        |       ID LBRACE STAR RBRACE
                {
                    RelAttr attr;
                    relation_attr_init(&attr, NULL, "*", $1);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
        |       ID LBRACE ID RBRACE
                {
                    RelAttr attr;
                    relation_attr_init(&attr, NULL, $3, $1);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
        |       ID LBRACE ID DOT ID RBRACE
                {
                    RelAttr attr;
                    relation_attr_init(&attr, $3, $5, $1);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
        |       ID LBRACE NUMBER RBRACE
                {
                    RelAttr attr;
                    relation_attr_init(&attr, NULL, "*", $1);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
        ;
attr_list: /* Empty */
                {}
        |       COMMA select_attr_expr attr_list
                {}
        ;

//rel_list:
//    /* empty */
//    | COMMA ID rel_list {	
//				selects_append_relation(&CONTEXT->ssql->sstr.selection, $2);
//		  }
//    ;

order:
    /* empty */
    | ORDER BY order_attr_expr order_attr_list {}
    ;
order_attr_expr:
    ID cmp
    {
        RelAttr attr;
        relation_attr_init(&attr, NULL, $1, NULL);
        selects_append_order_attrs(&CONTEXT->ssql->sstr.selection, &attr, $2);
    }
    | ID DOT ID cmp
    {
        RelAttr attr;
        relation_attr_init(&attr, $1, $3, NULL);
        selects_append_order_attrs(&CONTEXT->ssql->sstr.selection, &attr, $4);
    }
    ;
cmp:
    /* empty */ { $$ = 0; }
    | ASC { $$ = 0; }
    | DESC { $$ = 1; }
    ;
order_attr_list:
    /* empty */
    | COMMA order_attr_expr order_attr_list {}
    ;
group:
    /* empty */
    | GROUP BY group_attr_expr group_attr_list {}
    ;
group_attr_expr:
    ID
    {
        RelAttr attr;
        relation_attr_init(&attr, NULL, $1, NULL);
        selects_append_group_attrs(&CONTEXT->ssql->sstr.selection, &attr);
    }
    | ID DOT ID
    {
        RelAttr attr;
        relation_attr_init(&attr, $1, $3, NULL);
        selects_append_group_attrs(&CONTEXT->ssql->sstr.selection, &attr);
    }
    ;
group_attr_list:
    /* empty */
    | COMMA group_attr_expr group_attr_list {}
    ;
where:
    /* empty */ 
    | WHERE condition condition_list {	
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
    ;
condition_list:
    /* empty */
    | AND condition condition_list {
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
    ;
condition:
                ID comOp value           // 1
		{
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, $1, NULL);

			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$ = ( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;
			// $$->left_attr.relation_name = NULL;
			// $$->left_attr.attribute_name= $1;
			// $$->comp = CONTEXT->comp;
			// $$->right_is_attr = 0;
			// $$->right_attr.relation_name = NULL;
			// $$->right_attr.attribute_name = NULL;
			// $$->right_value = *$3;

		}
        |       value comOp value        // 2
		{
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$ = ( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 0;
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=NULL;
			// $$->left_value = *$1;
			// $$->comp = CONTEXT->comp;
			// $$->right_is_attr = 0;
			// $$->right_attr.relation_name = NULL;
			// $$->right_attr.attribute_name = NULL;
			// $$->right_value = *$3;

		}
        |       ID comOp ID              // 3
		{
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, $1, NULL);
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, $3, NULL);

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=$1;
			// $$->comp = CONTEXT->comp;
			// $$->right_is_attr = 1;
			// $$->right_attr.relation_name=NULL;
			// $$->right_attr.attribute_name=$3;

		}
        |       value comOp ID           // 4
		{
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, $3, NULL);

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 0;
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=NULL;
			// $$->left_value = *$1;
			// $$->comp=CONTEXT->comp;
			
			// $$->right_is_attr = 1;
			// $$->right_attr.relation_name=NULL;
			// $$->right_attr.attribute_name=$3;
		
		}
        |       ID DOT ID comOp value    // 5
		{
			RelAttr left_attr;
			relation_attr_init(&left_attr, $1, $3, NULL);
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;
			// $$->left_attr.relation_name=$1;
			// $$->left_attr.attribute_name=$3;
			// $$->comp=CONTEXT->comp;
			// $$->right_is_attr = 0;   //属性值
			// $$->right_attr.relation_name=NULL;
			// $$->right_attr.attribute_name=NULL;
			// $$->right_value =*$5;			
							
    }
        |       value comOp ID DOT ID    // 6
		{
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

			RelAttr right_attr;
			relation_attr_init(&right_attr, $3, $5, NULL);

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 0;//属性值
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=NULL;
			// $$->left_value = *$1;
			// $$->comp =CONTEXT->comp;
			// $$->right_is_attr = 1;//属性
			// $$->right_attr.relation_name = $3;
			// $$->right_attr.attribute_name = $5;
									
    }
        |       ID DOT ID comOp ID DOT ID// 7
		{
			RelAttr left_attr;
			relation_attr_init(&left_attr, $1, $3, NULL);
			RelAttr right_attr;
			relation_attr_init(&right_attr, $5, $7, NULL);

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;		//属性
			// $$->left_attr.relation_name=$1;
			// $$->left_attr.attribute_name=$3;
			// $$->comp =CONTEXT->comp;
			// $$->right_is_attr = 1;		//属性
			// $$->right_attr.relation_name=$5;
			// $$->right_attr.attribute_name=$7;
                }
        /* |       ID        IN    sub_select  // [8] */
        /*         { */
        /*             RelAttr left_attr; */
        /*             relation_attr_init(&left_attr, NULL, $1, NULL); */
        /*             Value* right_value = &CONTEXT->values[CONTEXT->value_length - 1]; */
                    
        /*         } */
        /* |       ID DOT ID IN    sub_select  // [9] */
        /* |       value     IN    sub_select  // [10] */
        /* |       ID        comOp sub_select  // [11] */
        /* |       ID DOT ID comOp sub_select  // [12] */
        /* |       value     comOp sub_select  // [13] */
    ;
comOp:
  	  EQ { CONTEXT->comp = EQUAL_TO; }
    | LT { CONTEXT->comp = LESS_THAN; }
    | GT { CONTEXT->comp = GREAT_THAN; }
    | LE { CONTEXT->comp = LESS_EQUAL; }
    | GE { CONTEXT->comp = GREAT_EQUAL; }
    | NE { CONTEXT->comp = NOT_EQUAL; }
    | IS { CONTEXT->comp = IS_NULL; }
    | IS NOT { CONTEXT->comp = IS_NOT_NULL; }
    ;

load_data:
		LOAD DATA INFILE SSS INTO TABLE ID SEMICOLON
		{
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, $7, $4);
		}
		;
%%
//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, Query *sqls){
	ParserContext context;
	memset(&context, 0, sizeof(context));
	yyscan_t scanner;
	yylex_init_extra(&context, &scanner);
	context.ssql = sqls;
	scan_string(s, scanner);
	int result = yyparse(scanner);
	yylex_destroy(scanner);
	return result;
}
