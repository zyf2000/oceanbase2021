/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 2 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:339  */


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


#line 125 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "yacc_sql.tab.h".  */
#ifndef YY_YY_SRC_OBSERVER_SQL_PARSER_YACC_SQL_TAB_H_INCLUDED
# define YY_YY_SRC_OBSERVER_SQL_PARSER_YACC_SQL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SEMICOLON = 258,
    CREATE = 259,
    DROP = 260,
    TABLE = 261,
    TABLES = 262,
    INDEX = 263,
    UNIQUE = 264,
    SELECT = 265,
    INNER = 266,
    JOIN = 267,
    DESC = 268,
    ASC = 269,
    SHOW = 270,
    SYNC = 271,
    INSERT = 272,
    DELETE = 273,
    UPDATE = 274,
    LBRACE = 275,
    RBRACE = 276,
    COMMA = 277,
    TRX_BEGIN = 278,
    TRX_COMMIT = 279,
    TRX_ROLLBACK = 280,
    NULLABLE = 281,
    IS = 282,
    NOT = 283,
    INT_T = 284,
    STRING_T = 285,
    FLOAT_T = 286,
    DATE_T = 287,
    HELP = 288,
    EXIT = 289,
    DOT = 290,
    INTO = 291,
    VALUES = 292,
    FROM = 293,
    WHERE = 294,
    ORDER = 295,
    GROUP = 296,
    BY = 297,
    AND = 298,
    SET = 299,
    ON = 300,
    LOAD = 301,
    DATA = 302,
    INFILE = 303,
    EQ = 304,
    LT = 305,
    GT = 306,
    LE = 307,
    GE = 308,
    NE = 309,
    NUMBER = 310,
    FLOAT = 311,
    ID = 312,
    PATH = 313,
    DATESSS = 314,
    SSS = 315,
    NULL_T = 316,
    STAR = 317,
    STRING_V = 318
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 120 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:355  */

  struct _Attr *attr;
  struct _Condition *condition1;
  struct _Value *value1;
  char *string;
  int number;
  float floats;
  char *position;
  struct _Vector* ve;

#line 240 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (void *scanner);

#endif /* !YY_YY_SRC_OBSERVER_SQL_PARSER_YACC_SQL_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 256 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   240

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  64
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  113
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  233

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   318

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   151,   151,   153,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   177,   182,   187,   193,   199,   205,   211,   217,   223,
     230,   235,   252,   258,   274,   289,   296,   305,   307,   311,
     322,   335,   336,   337,   340,   343,   344,   345,   346,   349,
     359,   375,   377,   381,   387,   389,   394,   397,   400,   404,
     408,   414,   424,   434,   452,   456,   464,   465,   475,   479,
     485,   491,   497,   503,   509,   515,   521,   529,   530,   541,
     543,   546,   552,   560,   561,   562,   564,   566,   568,   570,
     573,   579,   586,   588,   590,   592,   596,   598,   603,   624,
     644,   664,   686,   707,   728,   761,   762,   763,   764,   765,
     766,   767,   768,   772
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SEMICOLON", "CREATE", "DROP", "TABLE",
  "TABLES", "INDEX", "UNIQUE", "SELECT", "INNER", "JOIN", "DESC", "ASC",
  "SHOW", "SYNC", "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE",
  "COMMA", "TRX_BEGIN", "TRX_COMMIT", "TRX_ROLLBACK", "NULLABLE", "IS",
  "NOT", "INT_T", "STRING_T", "FLOAT_T", "DATE_T", "HELP", "EXIT", "DOT",
  "INTO", "VALUES", "FROM", "WHERE", "ORDER", "GROUP", "BY", "AND", "SET",
  "ON", "LOAD", "DATA", "INFILE", "EQ", "LT", "GT", "LE", "GE", "NE",
  "NUMBER", "FLOAT", "ID", "PATH", "DATESSS", "SSS", "NULL_T", "STAR",
  "STRING_V", "$accept", "commands", "command", "exit", "help", "sync",
  "begin", "commit", "rollback", "drop_table", "show_tables", "desc_table",
  "create_index", "id_list", "drop_index", "create_table", "attr_def_list",
  "attr_def", "whether_null", "number", "type", "ID_get", "insert",
  "insert_tuple_list", "insert_tuple", "value_list", "value", "delete",
  "update", "select", "relation_container", "join_container",
  "select_attr", "select_attr_expr", "attr_list", "order",
  "order_attr_expr", "cmp", "order_attr_list", "group", "group_attr_expr",
  "group_attr_list", "where", "condition_list", "condition", "comOp",
  "load_data", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318
};
# endif

#define YYPACT_NINF -168

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-168)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -168,     3,  -168,    53,    17,   -17,   -40,    36,    21,     5,
      22,    13,    79,    80,    81,    92,    99,    61,  -168,  -168,
    -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,
    -168,  -168,  -168,  -168,  -168,  -168,    56,    62,   112,    64,
      65,    -5,  -168,    85,   102,   122,   123,  -168,    70,    71,
      87,  -168,  -168,  -168,  -168,  -168,    82,   109,    88,    75,
     131,   132,    16,    52,    83,   -17,  -168,  -168,  -168,   100,
      97,    84,    78,    89,    90,    94,  -168,  -168,   121,    11,
     124,  -168,  -168,   133,    97,   102,   128,    20,   140,   101,
     113,  -168,   129,    86,   134,    95,  -168,  -168,    96,  -168,
     143,   135,   115,  -168,    51,   136,  -168,  -168,    15,  -168,
    -168,  -168,     2,   116,  -168,    51,   154,    89,   141,  -168,
    -168,  -168,  -168,   -14,   104,   144,   142,   108,    83,   125,
     126,   146,   128,   166,   145,   114,  -168,  -168,  -168,  -168,
    -168,  -168,    30,    37,    20,  -168,    97,   117,   129,   167,
     120,  -168,   111,  -168,    12,   119,  -168,   137,  -168,   127,
     138,   174,    51,   157,   136,  -168,  -168,     2,  -168,  -168,
     148,  -168,   116,   176,   178,  -168,  -168,  -168,   164,  -168,
     183,   130,   168,   169,    20,   153,   170,   139,  -168,   146,
    -168,  -168,    44,   147,  -168,  -168,  -168,    46,  -168,   171,
     188,   191,   116,   149,   127,  -168,    -4,   173,  -168,   162,
    -168,  -168,  -168,  -168,  -168,  -168,   133,  -168,   170,  -168,
    -168,   150,  -168,   139,  -168,   151,  -168,  -168,    34,   173,
    -168,  -168,  -168
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,    20,
      19,    14,    15,    16,    17,     9,    10,    11,    12,    13,
       8,     5,     7,     6,     4,    18,     0,     0,     0,     0,
       0,    70,    69,     0,    77,     0,     0,    23,     0,     0,
       0,    24,    25,    26,    22,    21,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    29,    28,     0,
      94,     0,     0,     0,     0,     0,    27,    35,     0,     0,
       0,    71,    72,    66,    94,    77,     0,     0,     0,     0,
       0,    49,    37,     0,     0,     0,    76,    74,     0,    73,
       0,    64,    88,    78,     0,    51,    56,    57,     0,    59,
      58,    60,     0,    96,    61,     0,     0,     0,     0,    45,
      46,    47,    48,    41,     0,     0,     0,     0,     0,     0,
      79,    54,     0,     0,   111,     0,   105,   106,   107,   108,
     109,   110,     0,     0,     0,    95,    94,     0,    37,     0,
       0,    43,     0,    40,     0,     0,    75,     0,    65,     0,
       0,     0,     0,     0,    51,    50,   112,     0,   100,    98,
     101,    99,    96,     0,     0,    38,    36,    44,     0,    42,
       0,     0,     0,     0,     0,    90,    92,     0,    63,    54,
      53,    52,     0,     0,    97,    62,   113,    41,    30,    33,
       0,     0,    96,     0,     0,    89,    83,    86,    55,     0,
     102,   103,    39,    34,    31,    32,    66,    91,    92,    85,
      84,     0,    81,     0,    80,     0,    67,    93,    83,    86,
     104,    82,    87
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,
    -168,  -168,  -168,    -1,  -168,  -168,    54,    93,     4,  -168,
    -168,  -168,  -168,    35,    68,    14,  -104,  -168,  -168,  -168,
      77,    -7,  -168,   152,   155,  -168,   -12,   -16,   -15,  -168,
       9,    -3,   -83,  -167,  -140,  -110,  -168
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,   182,    29,    30,   118,    92,   153,   178,
     123,    93,    31,   133,   105,   163,   112,    32,    33,    34,
      84,   101,    43,    44,    66,   161,   207,   222,   224,   130,
     186,   205,    88,   145,   113,   142,    35
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
     131,   102,   143,     2,   172,   194,   150,     3,     4,   219,
     220,   146,   151,     5,   152,    62,     6,    45,     7,     8,
       9,    10,    11,    39,    47,    40,    12,    13,    14,   134,
      63,   221,    97,   180,   181,   216,    15,    16,   169,   171,
      41,    48,   134,    46,   202,    42,    98,   219,   220,    17,
     135,   136,   137,   138,   139,   140,   141,   192,   189,    36,
      49,    37,    38,   173,   136,   137,   138,   139,   140,   141,
      50,    78,   151,    79,   152,   106,   107,   108,    80,   109,
     110,   111,    51,    52,    53,   106,   107,   168,   210,   109,
     110,   111,   106,   107,   170,    54,   109,   110,   111,   106,
     107,   209,    55,   109,   110,   111,   106,   107,    56,    81,
     109,   110,   111,    57,    82,   119,   120,   121,   122,    58,
      59,    60,    61,    64,    65,    67,    68,    69,    70,    73,
      72,    71,    75,    74,    76,    77,    87,    86,    90,    95,
      83,    89,    96,   114,   100,    99,    91,    94,   104,   116,
     115,   117,   125,   126,   124,   127,   129,   128,   132,   144,
     147,   154,   149,   156,   155,   157,   160,   159,   162,   165,
     176,   167,   179,   166,   174,   177,   183,   188,   190,   195,
     187,   196,   184,   193,   185,   197,   198,   199,   203,   200,
     201,   214,   204,   181,   215,   223,   206,   225,   213,   191,
     164,   212,   175,   208,   211,   158,   217,   228,   230,   226,
     148,   229,   231,   218,   232,   227,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     103
};

static const yytype_int16 yycheck[] =
{
     104,    84,   112,     0,   144,   172,    20,     4,     5,    13,
      14,   115,    26,    10,    28,    20,    13,    57,    15,    16,
      17,    18,    19,     6,     3,     8,    23,    24,    25,    27,
      35,    35,    21,    21,    22,   202,    33,    34,   142,   143,
      57,    36,    27,     7,   184,    62,    35,    13,    14,    46,
      35,    49,    50,    51,    52,    53,    54,   167,   162,     6,
      38,     8,     9,   146,    49,    50,    51,    52,    53,    54,
      57,    55,    26,    57,    28,    55,    56,    57,    62,    59,
      60,    61,     3,     3,     3,    55,    56,    57,   192,    59,
      60,    61,    55,    56,    57,     3,    59,    60,    61,    55,
      56,    57,     3,    59,    60,    61,    55,    56,    47,    57,
      59,    60,    61,    57,    62,    29,    30,    31,    32,    57,
       8,    57,    57,    38,    22,     3,     3,    57,    57,    20,
      48,    44,    57,    45,     3,     3,    39,    37,    60,    45,
      57,    57,    21,     3,    11,    21,    57,    57,    20,    36,
      49,    22,    57,    57,    20,    12,    41,    22,    22,    43,
       6,    57,    21,    21,    20,    57,    40,    42,    22,     3,
       3,    57,    61,    28,    57,    55,    57,     3,    21,     3,
      42,     3,    45,    35,    57,    21,     3,    57,    35,    21,
      21,     3,    22,    22,     3,    22,    57,    35,   199,   164,
     132,   197,   148,   189,    57,   128,    57,    57,    57,   216,
     117,   223,   228,   204,   229,   218,    -1,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    65,     0,     4,     5,    10,    13,    15,    16,    17,
      18,    19,    23,    24,    25,    33,    34,    46,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    78,
      79,    86,    91,    92,    93,   110,     6,     8,     9,     6,
       8,    57,    62,    96,    97,    57,     7,     3,    36,    38,
      57,     3,     3,     3,     3,     3,    47,    57,    57,     8,
      57,    57,    20,    35,    38,    22,    98,     3,     3,    57,
      57,    44,    48,    20,    45,    57,     3,     3,    55,    57,
      62,    57,    62,    57,    94,    97,    37,    39,   106,    57,
      60,    57,    81,    85,    57,    45,    21,    21,    35,    21,
      11,    95,   106,    98,    20,    88,    55,    56,    57,    59,
      60,    61,    90,   108,     3,    49,    36,    22,    80,    29,
      30,    31,    32,    84,    20,    57,    57,    12,    22,    41,
     103,    90,    22,    87,    27,    35,    49,    50,    51,    52,
      53,    54,   109,   109,    43,   107,    90,     6,    81,    21,
      20,    26,    28,    82,    57,    20,    21,    57,    94,    42,
      40,    99,    22,    89,    88,     3,    28,    57,    57,    90,
      57,    90,   108,   106,    57,    80,     3,    55,    83,    61,
      21,    22,    77,    57,    45,    57,   104,    42,     3,    90,
      21,    87,   109,    35,   107,     3,     3,    21,     3,    57,
      21,    21,   108,    35,    22,   105,    57,   100,    89,    57,
      90,    57,    82,    77,     3,     3,   107,    57,   104,    13,
      14,    35,   101,    22,   102,    35,    95,   105,    57,   100,
      57,   101,   102
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    64,    65,    65,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    76,    76,    77,    77,    78,    79,    80,    80,    81,
      81,    82,    82,    82,    83,    84,    84,    84,    84,    85,
      86,    87,    87,    88,    89,    89,    90,    90,    90,    90,
      90,    91,    92,    93,    94,    94,    95,    95,    96,    97,
      97,    97,    97,    97,    97,    97,    97,    98,    98,    99,
      99,   100,   100,   101,   101,   101,   102,   102,   103,   103,
     104,   104,   105,   105,   106,   106,   107,   107,   108,   108,
     108,   108,   108,   108,   108,   109,   109,   109,   109,   109,
     109,   109,   109,   110
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     4,     3,     3,
       9,    10,    10,     2,     3,     4,     8,     0,     3,     6,
       3,     0,     2,     1,     1,     1,     1,     1,     1,     1,
       7,     0,     3,     4,     0,     3,     1,     1,     1,     1,
       1,     5,     8,     8,     2,     4,     0,     7,     2,     1,
       1,     3,     3,     4,     4,     6,     4,     0,     3,     0,
       4,     2,     4,     0,     1,     1,     0,     3,     0,     4,
       1,     3,     0,     3,     0,     3,     0,     3,     3,     3,
       3,     3,     5,     5,     7,     1,     1,     1,     1,     1,
       1,     1,     2,     8
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (scanner, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, void *scanner)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, void *scanner)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, scanner);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, void *scanner)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              , scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, void *scanner)
{
  YYUSE (yyvaluep);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void *scanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 21:
#line 177 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1515 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 182 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1523 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 187 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1531 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 193 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1539 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 199 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1547 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 205 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1555 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 211 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, (yyvsp[-1].string));
    }
#line 1564 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 217 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1572 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 223 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, (yyvsp[-1].string));
    }
#line 1581 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 231 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string), (yyvsp[-2].string), 0);
		}
#line 1590 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 237 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
        if((yyvsp[-2].ve)->len < 32)
            {
                (yyvsp[-2].ve)->data[(yyvsp[-2].ve)->len] = (yyvsp[-3].string);
                (yyvsp[-2].ve)->len++;
            }
        else
                {
                    exit(-1);
                }
        create_multi_index_init(
            &CONTEXT->ssql->sstr.create_multi_index,
            (yyvsp[-7].string), (yyvsp[-5].string), (yyvsp[-2].ve)->data, (yyvsp[-2].ve)->len, 0);
        CONTEXT->ssql->flag = SCF_CREATE_MULTI_INDEX;
    }
#line 1610 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 253 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
            CONTEXT->ssql->flag = SCF_CREATE_INDEX;
            create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string), (yyvsp[-2].string), 1);
        }
#line 1619 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 258 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
             (yyval.ve) = (Vector*) malloc(sizeof(Vector*));
             memset((yyval.ve),0,sizeof(Vector*));
             (yyval.ve)->len = 0;
             (yyval.ve)->data = (char**) malloc(32*sizeof(char*));
             memset((yyval.ve)->data,0,32*sizeof(char*));
             if((yyval.ve)->len < 32)
                 {
                     (yyval.ve)->data[(yyval.ve)->len] = (yyvsp[0].string);
                     (yyval.ve)->len++;
                }
                else
                {
                    exit(-1);
                }
    }
#line 1640 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 275 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
                    (yyval.ve) = (yyvsp[0].ve);
                    if((yyval.ve)->len < 32)
                        {
                            (yyval.ve)->data[(yyval.ve)->len] = (yyvsp[-1].string);
                            (yyval.ve)->len++;
                }
                else
                {
                    exit(-1);
                }
                }
#line 1657 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 290 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, (yyvsp[-1].string));
		}
#line 1666 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 297 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			// CONTEXT->ssql->sstr.create_table.attribute_count = CONTEXT->value_length;
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, (yyvsp[-5].string));
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
#line 1678 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 307 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {    }
#line 1684 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 312 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-4].number), (yyvsp[-2].number), (yyvsp[0].number));
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name =(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type = $2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length = $4;
			CONTEXT->value_length++;
		}
#line 1699 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 323 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-1].number), 5, (yyvsp[0].number));
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name=(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type=$2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length=4; // default attribute length
			CONTEXT->value_length++;
		}
#line 1714 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 335 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { (yyval.number) = 0; }
#line 1720 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 336 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { (yyval.number) = 0; }
#line 1726 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 337 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { (yyval.number) = 1; }
#line 1732 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 340 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {(yyval.number) = (yyvsp[0].number);}
#line 1738 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 343 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { (yyval.number)=INTS; }
#line 1744 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 344 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { (yyval.number)=CHARS; }
#line 1750 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 345 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { (yyval.number)=FLOATS; }
#line 1756 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 346 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { (yyval.number)=DATES; }
#line 1762 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 350 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
		char *temp=(yyvsp[0].string); 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
#line 1771 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 360 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
			// CONTEXT->values[CONTEXT->value_length++] = *$6;

			CONTEXT->ssql->flag=SCF_INSERT;//"insert";
			// CONTEXT->ssql->sstr.insertion.relation_name = $3;
			// CONTEXT->ssql->sstr.insertion.value_num = CONTEXT->value_length;
			// for(i = 0; i < CONTEXT->value_length; i++){
			// 	CONTEXT->ssql->sstr.insertion.values[i] = CONTEXT->values[i];
      // }
			// inserts_init(&CONTEXT->ssql->sstr.insertion, $3, CONTEXT->values, CONTEXT->value_length);
            inserts_init(&CONTEXT->ssql->sstr.insertion, (yyvsp[-4].string), CONTEXT->insert_tuples, CONTEXT->insert_tuple_length);

      //临时变量清零
      CONTEXT->insert_tuple_length = 0;
    }
#line 1791 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 378 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {}
#line 1797 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 382 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
        inserts_tuple_append_value(&CONTEXT->insert_tuples[CONTEXT->insert_tuple_length++], CONTEXT->values, CONTEXT->value_length);
        CONTEXT->value_length = 0;
    }
#line 1806 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 389 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { 
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
#line 1814 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 394 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number));
		}
#line 1822 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 397 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
  		value_init_float(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].floats));
		}
#line 1830 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 400 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
        (yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_string(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
		}
#line 1839 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 404 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
        (yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
        value_init_dates(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
    }
#line 1848 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 408 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
        value_init_null(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
    }
#line 1856 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 415 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
			CONTEXT->ssql->flag = SCF_DELETE;//"delete";
			deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-2].string));
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;	
    }
#line 1868 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 425 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
			CONTEXT->ssql->flag = SCF_UPDATE;//"update";
			Value *value = &CONTEXT->values[0];
			updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-6].string), (yyvsp[-4].string), value, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;
		}
#line 1880 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 435 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
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
#line 1899 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 453 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
                    selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-1].string));
                }
#line 1907 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 457 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
                    selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-3].string));
                }
#line 1915 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 464 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {}
#line 1921 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 466 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
                    selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-4].string));
                }
#line 1929 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 476 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {}
#line 1935 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 480 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
                    RelAttr attr;
                    relation_attr_init(&attr, NULL, "*", NULL);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 1945 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 486 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
                    RelAttr attr;
                    relation_attr_init(&attr, NULL, (yyvsp[0].string), NULL);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 1955 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 492 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
                    RelAttr attr;
                    relation_attr_init(&attr, (yyvsp[-2].string), (yyvsp[0].string), NULL);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 1965 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 498 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
                    RelAttr attr;
                    relation_attr_init(&attr, (yyvsp[-2].string), "*", NULL);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 1975 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 504 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
                    RelAttr attr;
                    relation_attr_init(&attr, NULL, "*", (yyvsp[-3].string));
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 1985 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 510 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
                    RelAttr attr;
                    relation_attr_init(&attr, NULL, (yyvsp[-1].string), (yyvsp[-3].string));
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 1995 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 516 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
                    RelAttr attr;
                    relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string));
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 2005 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 522 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
                    RelAttr attr;
                    relation_attr_init(&attr, NULL, "*", (yyvsp[-3].string));
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 2015 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 529 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {}
#line 2021 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 531 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {}
#line 2027 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 543 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {}
#line 2033 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 547 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
        RelAttr attr;
        relation_attr_init(&attr, NULL, (yyvsp[-1].string), NULL);
        selects_append_order_attrs(&CONTEXT->ssql->sstr.selection, &attr, (yyvsp[0].number));
    }
#line 2043 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 553 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
        RelAttr attr;
        relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), NULL);
        selects_append_order_attrs(&CONTEXT->ssql->sstr.selection, &attr, (yyvsp[0].number));
    }
#line 2053 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 560 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { (yyval.number) = 0; }
#line 2059 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 561 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { (yyval.number) = 0; }
#line 2065 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 562 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { (yyval.number) = 1; }
#line 2071 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 566 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {}
#line 2077 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 570 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {}
#line 2083 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 574 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
        RelAttr attr;
        relation_attr_init(&attr, NULL, (yyvsp[0].string), NULL);
        selects_append_group_attrs(&CONTEXT->ssql->sstr.selection, &attr);
    }
#line 2093 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 580 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
        RelAttr attr;
        relation_attr_init(&attr, (yyvsp[-2].string), (yyvsp[0].string), NULL);
        selects_append_group_attrs(&CONTEXT->ssql->sstr.selection, &attr);
    }
#line 2103 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 588 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {}
#line 2109 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 592 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {	
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 2117 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 598 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 2125 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 604 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string), NULL);

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
#line 2150 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 625 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
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
#line 2174 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 645 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string), NULL);
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string), NULL);

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
#line 2198 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 665 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string), NULL);

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
#line 2224 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 687 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string), NULL);
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
#line 2249 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 708 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string), NULL);

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
#line 2274 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 729 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-6].string), (yyvsp[-4].string), NULL);
			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string), NULL);

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
#line 2297 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 761 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { CONTEXT->comp = EQUAL_TO; }
#line 2303 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 762 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { CONTEXT->comp = LESS_THAN; }
#line 2309 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 763 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { CONTEXT->comp = GREAT_THAN; }
#line 2315 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 764 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { CONTEXT->comp = LESS_EQUAL; }
#line 2321 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 765 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { CONTEXT->comp = GREAT_EQUAL; }
#line 2327 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 766 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { CONTEXT->comp = NOT_EQUAL; }
#line 2333 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 767 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { CONTEXT->comp = IS_NULL; }
#line 2339 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 768 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    { CONTEXT->comp = IS_NOT_NULL; }
#line 2345 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 773 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1646  */
    {
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, (yyvsp[-1].string), (yyvsp[-4].string));
		}
#line 2354 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
    break;


#line 2358 "./src/observer/sql/parser/yacc_sql.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (scanner, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (scanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (scanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 778 "./src/observer/sql/parser/yacc_sql.y" /* yacc.c:1906  */

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
