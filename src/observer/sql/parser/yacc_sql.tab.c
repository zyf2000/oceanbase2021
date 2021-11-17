/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "./src/observer/sql/parser/yacc_sql.y"


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


#line 129 "./src/observer/sql/parser/yacc_sql.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
    NULL_T = 284,
    INT_T = 285,
    STRING_T = 286,
    FLOAT_T = 287,
    DATE_T = 288,
    HELP = 289,
    EXIT = 290,
    DOT = 291,
    INTO = 292,
    VALUES = 293,
    FROM = 294,
    WHERE = 295,
    ORDER = 296,
    GROUP = 297,
    BY = 298,
    AND = 299,
    SET = 300,
    ON = 301,
    LOAD = 302,
    DATA = 303,
    INFILE = 304,
    EQ = 305,
    LT = 306,
    GT = 307,
    LE = 308,
    GE = 309,
    NE = 310,
    NUMBER = 311,
    FLOAT = 312,
    ID = 313,
    PATH = 314,
    DATESSS = 315,
    SSS = 316,
    STAR = 317,
    STRING_V = 318
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 120 "./src/observer/sql/parser/yacc_sql.y"

  struct _Attr *attr;
  struct _Condition *condition1;
  struct _Value *value1;
  char *string;
  int number;
  float floats;
  char *position; 

#line 255 "./src/observer/sql/parser/yacc_sql.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (void *scanner);

#endif /* !YY_YY_SRC_OBSERVER_SQL_PARSER_YACC_SQL_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

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
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
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
#define YYLAST   219

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  64
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  110
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  227

#define YYUNDEFTOK  2
#define YYMAXUTOK   318


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
static const yytype_int16 yyrline[] =
{
       0,   150,   150,   152,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   176,   181,   186,   192,   198,   204,   210,   216,   222,
     229,   234,   242,   249,   258,   260,   264,   275,   288,   289,
     290,   293,   296,   297,   298,   299,   302,   312,   328,   330,
     334,   340,   342,   347,   350,   353,   357,   361,   367,   377,
     387,   405,   409,   417,   418,   428,   432,   438,   444,   450,
     456,   462,   468,   474,   482,   483,   493,   495,   498,   504,
     512,   513,   514,   516,   518,   520,   522,   525,   531,   538,
     540,   542,   544,   548,   550,   555,   576,   596,   616,   638,
     659,   680,   702,   703,   704,   705,   706,   707,   708,   709,
     713
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
  "NOT", "NULL_T", "INT_T", "STRING_T", "FLOAT_T", "DATE_T", "HELP",
  "EXIT", "DOT", "INTO", "VALUES", "FROM", "WHERE", "ORDER", "GROUP", "BY",
  "AND", "SET", "ON", "LOAD", "DATA", "INFILE", "EQ", "LT", "GT", "LE",
  "GE", "NE", "NUMBER", "FLOAT", "ID", "PATH", "DATESSS", "SSS", "STAR",
  "STRING_V", "$accept", "commands", "command", "exit", "help", "sync",
  "begin", "commit", "rollback", "drop_table", "show_tables", "desc_table",
  "create_index", "drop_index", "create_table", "attr_def_list",
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
static const yytype_int16 yytoknum[] =
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

#define YYPACT_NINF (-169)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -169,     8,  -169,   106,    95,    -6,   -43,    22,    33,     0,
      12,     1,    47,    51,    57,    74,    80,    54,  -169,  -169,
    -169,  -169,  -169,  -169,  -169,  -169,  -169,  -169,  -169,  -169,
    -169,  -169,  -169,  -169,  -169,  -169,    48,    55,   109,    61,
      62,    -1,  -169,    82,   100,   120,   121,  -169,    67,    68,
      83,  -169,  -169,  -169,  -169,  -169,    81,   107,    85,    71,
     129,   130,   -42,    42,    76,    -6,  -169,  -169,  -169,    97,
      96,    84,    77,    86,    87,    91,  -169,  -169,   118,   -14,
     119,  -169,  -169,   132,    96,   100,   126,   -12,   138,    98,
     110,  -169,   127,    78,   131,    92,  -169,  -169,    94,  -169,
     141,   133,   112,  -169,     5,   134,  -169,  -169,  -169,    37,
    -169,  -169,    43,   113,  -169,     5,   152,    86,   139,  -169,
    -169,  -169,  -169,    79,   101,   142,   140,   105,    76,   122,
     123,   144,   126,   164,   143,   111,  -169,  -169,  -169,  -169,
    -169,  -169,    11,    18,   -12,  -169,    96,   114,   127,   165,
     117,  -169,   145,  -169,   149,   124,  -169,   135,  -169,   125,
     136,   172,     5,   155,   134,  -169,  -169,    43,  -169,  -169,
     148,  -169,   113,   174,   175,  -169,  -169,  -169,   159,  -169,
     182,   166,   -12,   150,   167,   137,  -169,   144,  -169,  -169,
      24,   146,  -169,  -169,  -169,    90,  -169,   185,   113,   147,
     125,  -169,    -8,   168,  -169,   156,  -169,  -169,  -169,  -169,
     132,  -169,   167,  -169,  -169,   151,  -169,   137,  -169,   153,
    -169,  -169,    -4,   168,  -169,  -169,  -169
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,    20,
      19,    14,    15,    16,    17,     9,    10,    11,    12,    13,
       8,     5,     7,     6,     4,    18,     0,     0,     0,     0,
       0,    67,    66,     0,    74,     0,     0,    23,     0,     0,
       0,    24,    25,    26,    22,    21,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    29,    28,     0,
      91,     0,     0,     0,     0,     0,    27,    32,     0,     0,
       0,    68,    69,    63,    91,    74,     0,     0,     0,     0,
       0,    46,    34,     0,     0,     0,    73,    71,     0,    70,
       0,    61,    85,    75,     0,    48,    57,    53,    54,     0,
      56,    55,     0,    93,    58,     0,     0,     0,     0,    42,
      43,    44,    45,    38,     0,     0,     0,     0,     0,     0,
      76,    51,     0,     0,   108,     0,   102,   103,   104,   105,
     106,   107,     0,     0,     0,    92,    91,     0,    34,     0,
       0,    40,     0,    37,     0,     0,    72,     0,    62,     0,
       0,     0,     0,     0,    48,    47,   109,     0,    97,    95,
      98,    96,    93,     0,     0,    35,    33,    41,     0,    39,
       0,     0,     0,    87,    89,     0,    60,    51,    50,    49,
       0,     0,    94,    59,   110,    38,    30,     0,    93,     0,
       0,    86,    80,    83,    52,     0,    99,   100,    36,    31,
      63,    88,    89,    82,    81,     0,    78,     0,    77,     0,
      64,    90,    80,    83,   101,    79,    84
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -169,  -169,  -169,  -169,  -169,  -169,  -169,  -169,  -169,  -169,
    -169,  -169,  -169,  -169,  -169,    45,    89,     2,  -169,  -169,
    -169,  -169,    27,    64,     7,  -104,  -169,  -169,  -169,    70,
     -11,  -169,   154,   115,  -169,   -16,   -20,   -15,  -169,     3,
      -5,   -83,  -168,  -141,  -110,  -169
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,   118,    92,   153,   178,   123,
      93,    31,   133,   105,   163,   112,    32,    33,    34,    84,
     101,    43,    44,    66,   161,   203,   216,   218,   130,   184,
     201,    88,   145,   113,   142,    35
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
     131,   102,   143,   172,   192,   213,   214,    97,     2,   213,
     214,   146,     3,     4,    78,    45,    79,   106,     5,    62,
      80,     6,    98,     7,     8,     9,    10,    11,   215,    46,
     210,    12,    13,    14,   106,    63,    47,    48,   169,   171,
     106,   198,    15,    16,   107,   108,   109,   106,   110,   111,
      51,    49,    41,   106,    52,    17,    42,   190,   187,    50,
      53,   107,   108,   173,   134,   110,   111,   107,   108,   168,
     134,   110,   111,   135,   107,   108,   170,    54,   110,   111,
     107,   108,   205,    55,   110,   111,   206,   136,   137,   138,
     139,   140,   141,   136,   137,   138,   139,   140,   141,   150,
      81,    39,    56,    40,    82,   151,    57,   152,   119,   120,
     121,   122,    36,    58,    37,    38,   151,    59,   152,    60,
      61,    64,    65,    67,    68,    69,    70,    73,    71,    75,
      72,    74,    76,    77,    83,    86,    87,    95,    90,    96,
      99,   114,    89,   100,    91,    94,   104,   116,   115,   117,
     125,   124,   126,   127,   129,   128,   132,   144,   147,   154,
     149,   156,   155,   157,   160,   159,   162,   165,   176,   167,
     180,   166,   174,   177,   179,   186,   188,   193,   194,   185,
     195,   182,   181,   183,   191,   196,   199,   197,   209,   200,
     217,   189,   219,   175,   204,   202,   164,   208,   158,   220,
     103,   223,   225,   212,   207,   211,   148,   221,   226,   222,
       0,   224,     0,     0,     0,     0,     0,     0,     0,    85
};

static const yytype_int16 yycheck[] =
{
     104,    84,   112,   144,   172,    13,    14,    21,     0,    13,
      14,   115,     4,     5,    56,    58,    58,    29,    10,    20,
      62,    13,    36,    15,    16,    17,    18,    19,    36,     7,
     198,    23,    24,    25,    29,    36,     3,    37,   142,   143,
      29,   182,    34,    35,    56,    57,    58,    29,    60,    61,
       3,    39,    58,    29,     3,    47,    62,   167,   162,    58,
       3,    56,    57,   146,    27,    60,    61,    56,    57,    58,
      27,    60,    61,    36,    56,    57,    58,     3,    60,    61,
      56,    57,    58,     3,    60,    61,   190,    50,    51,    52,
      53,    54,    55,    50,    51,    52,    53,    54,    55,    20,
      58,     6,    48,     8,    62,    26,    58,    28,    30,    31,
      32,    33,     6,    58,     8,     9,    26,     8,    28,    58,
      58,    39,    22,     3,     3,    58,    58,    20,    45,    58,
      49,    46,     3,     3,    58,    38,    40,    46,    61,    21,
      21,     3,    58,    11,    58,    58,    20,    37,    50,    22,
      58,    20,    58,    12,    42,    22,    22,    44,     6,    58,
      21,    21,    20,    58,    41,    43,    22,     3,     3,    58,
      21,    28,    58,    56,    29,     3,    21,     3,     3,    43,
      21,    46,    58,    58,    36,     3,    36,    21,     3,    22,
      22,   164,    36,   148,   187,    58,   132,   195,   128,   210,
      85,   217,   222,   200,    58,    58,   117,   212,   223,    58,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    65,     0,     4,     5,    10,    13,    15,    16,    17,
      18,    19,    23,    24,    25,    34,    35,    47,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    85,    90,    91,    92,   109,     6,     8,     9,     6,
       8,    58,    62,    95,    96,    58,     7,     3,    37,    39,
      58,     3,     3,     3,     3,     3,    48,    58,    58,     8,
      58,    58,    20,    36,    39,    22,    97,     3,     3,    58,
      58,    45,    49,    20,    46,    58,     3,     3,    56,    58,
      62,    58,    62,    58,    93,    96,    38,    40,   105,    58,
      61,    58,    80,    84,    58,    46,    21,    21,    36,    21,
      11,    94,   105,    97,    20,    87,    29,    56,    57,    58,
      60,    61,    89,   107,     3,    50,    37,    22,    79,    30,
      31,    32,    33,    83,    20,    58,    58,    12,    22,    42,
     102,    89,    22,    86,    27,    36,    50,    51,    52,    53,
      54,    55,   108,   108,    44,   106,    89,     6,    80,    21,
      20,    26,    28,    81,    58,    20,    21,    58,    93,    43,
      41,    98,    22,    88,    87,     3,    28,    58,    58,    89,
      58,    89,   107,   105,    58,    79,     3,    56,    82,    29,
      21,    58,    46,    58,   103,    43,     3,    89,    21,    86,
     108,    36,   106,     3,     3,    21,     3,    21,   107,    36,
      22,   104,    58,    99,    88,    58,    89,    58,    81,     3,
     106,    58,   103,    13,    14,    36,   100,    22,   101,    36,
      94,   104,    58,    99,    58,   100,   101
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    64,    65,    65,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    76,    77,    78,    79,    79,    80,    80,    81,    81,
      81,    82,    83,    83,    83,    83,    84,    85,    86,    86,
      87,    88,    88,    89,    89,    89,    89,    89,    90,    91,
      92,    93,    93,    94,    94,    95,    96,    96,    96,    96,
      96,    96,    96,    96,    97,    97,    98,    98,    99,    99,
     100,   100,   100,   101,   101,   102,   102,   103,   103,   104,
     104,   105,   105,   106,   106,   107,   107,   107,   107,   107,
     107,   107,   108,   108,   108,   108,   108,   108,   108,   108,
     109
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     4,     3,     3,
       9,    10,     4,     8,     0,     3,     6,     3,     0,     2,
       1,     1,     1,     1,     1,     1,     1,     7,     0,     3,
       4,     0,     3,     1,     1,     1,     1,     1,     5,     8,
       8,     2,     4,     0,     7,     2,     1,     1,     3,     3,
       4,     4,     6,     4,     0,     3,     0,     4,     2,     4,
       0,     1,     1,     0,     3,     0,     4,     1,     3,     0,
       3,     0,     3,     0,     3,     3,     3,     3,     3,     5,
       5,     7,     1,     1,     1,     1,     1,     1,     1,     2,
       8
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, void *scanner)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, void *scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule, void *scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
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
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
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
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

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
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
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
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
          ++yyp;
          ++yyformat;
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

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

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
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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
#line 176 "./src/observer/sql/parser/yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1601 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 22:
#line 181 "./src/observer/sql/parser/yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1609 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 23:
#line 186 "./src/observer/sql/parser/yacc_sql.y"
                   {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1617 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 24:
#line 192 "./src/observer/sql/parser/yacc_sql.y"
                        {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1625 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 25:
#line 198 "./src/observer/sql/parser/yacc_sql.y"
                         {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1633 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 26:
#line 204 "./src/observer/sql/parser/yacc_sql.y"
                           {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1641 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 27:
#line 210 "./src/observer/sql/parser/yacc_sql.y"
                            {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, (yyvsp[-1].string));
    }
#line 1650 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 28:
#line 216 "./src/observer/sql/parser/yacc_sql.y"
                          {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1658 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 29:
#line 222 "./src/observer/sql/parser/yacc_sql.y"
                      {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, (yyvsp[-1].string));
    }
#line 1667 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 30:
#line 230 "./src/observer/sql/parser/yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string), (yyvsp[-2].string), 0);
		}
#line 1676 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 31:
#line 235 "./src/observer/sql/parser/yacc_sql.y"
        {
            CONTEXT->ssql->flag = SCF_CREATE_INDEX;
            create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string), (yyvsp[-2].string), 1);
        }
#line 1685 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 32:
#line 243 "./src/observer/sql/parser/yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, (yyvsp[-1].string));
		}
#line 1694 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 33:
#line 250 "./src/observer/sql/parser/yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			// CONTEXT->ssql->sstr.create_table.attribute_count = CONTEXT->value_length;
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, (yyvsp[-5].string));
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
#line 1706 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 35:
#line 260 "./src/observer/sql/parser/yacc_sql.y"
                                   {    }
#line 1712 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 36:
#line 265 "./src/observer/sql/parser/yacc_sql.y"
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
#line 1727 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 37:
#line 276 "./src/observer/sql/parser/yacc_sql.y"
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
#line 1742 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 38:
#line 288 "./src/observer/sql/parser/yacc_sql.y"
                { (yyval.number) = 0; }
#line 1748 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 39:
#line 289 "./src/observer/sql/parser/yacc_sql.y"
                 { (yyval.number) = 0; }
#line 1754 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 40:
#line 290 "./src/observer/sql/parser/yacc_sql.y"
               { (yyval.number) = 1; }
#line 1760 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 41:
#line 293 "./src/observer/sql/parser/yacc_sql.y"
                       {(yyval.number) = (yyvsp[0].number);}
#line 1766 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 42:
#line 296 "./src/observer/sql/parser/yacc_sql.y"
              { (yyval.number)=INTS; }
#line 1772 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 43:
#line 297 "./src/observer/sql/parser/yacc_sql.y"
                  { (yyval.number)=CHARS; }
#line 1778 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 44:
#line 298 "./src/observer/sql/parser/yacc_sql.y"
                 { (yyval.number)=FLOATS; }
#line 1784 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 45:
#line 299 "./src/observer/sql/parser/yacc_sql.y"
                { (yyval.number)=DATES; }
#line 1790 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 46:
#line 303 "./src/observer/sql/parser/yacc_sql.y"
        {
		char *temp=(yyvsp[0].string); 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
#line 1799 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 47:
#line 313 "./src/observer/sql/parser/yacc_sql.y"
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
#line 1819 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 49:
#line 331 "./src/observer/sql/parser/yacc_sql.y"
        {}
#line 1825 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 50:
#line 335 "./src/observer/sql/parser/yacc_sql.y"
    {
        inserts_tuple_append_value(&CONTEXT->insert_tuples[CONTEXT->insert_tuple_length++], CONTEXT->values, CONTEXT->value_length);
        CONTEXT->value_length = 0;
    }
#line 1834 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 52:
#line 342 "./src/observer/sql/parser/yacc_sql.y"
                              { 
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
#line 1842 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 53:
#line 347 "./src/observer/sql/parser/yacc_sql.y"
          {	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number));
		}
#line 1850 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 54:
#line 350 "./src/observer/sql/parser/yacc_sql.y"
          {
  		value_init_float(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].floats));
		}
#line 1858 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 55:
#line 353 "./src/observer/sql/parser/yacc_sql.y"
         {
        (yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_string(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
		}
#line 1867 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 56:
#line 357 "./src/observer/sql/parser/yacc_sql.y"
             {
        (yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
        value_init_dates(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
    }
#line 1876 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 57:
#line 361 "./src/observer/sql/parser/yacc_sql.y"
            {
        value_init_null(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
    }
#line 1884 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 58:
#line 368 "./src/observer/sql/parser/yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_DELETE;//"delete";
			deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-2].string));
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;	
    }
#line 1896 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 59:
#line 378 "./src/observer/sql/parser/yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_UPDATE;//"update";
			Value *value = &CONTEXT->values[0];
			updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-6].string), (yyvsp[-4].string), value, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;
		}
#line 1908 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 60:
#line 388 "./src/observer/sql/parser/yacc_sql.y"
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
#line 1927 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 61:
#line 406 "./src/observer/sql/parser/yacc_sql.y"
                {
                    selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-1].string));
                }
#line 1935 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 62:
#line 410 "./src/observer/sql/parser/yacc_sql.y"
                {
                    selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-3].string));
                }
#line 1943 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 63:
#line 417 "./src/observer/sql/parser/yacc_sql.y"
                {}
#line 1949 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 64:
#line 419 "./src/observer/sql/parser/yacc_sql.y"
                {
                    selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-4].string));
                }
#line 1957 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 65:
#line 429 "./src/observer/sql/parser/yacc_sql.y"
                {}
#line 1963 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 66:
#line 433 "./src/observer/sql/parser/yacc_sql.y"
                {
                    RelAttr attr;
                    relation_attr_init(&attr, NULL, "*", NULL);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 1973 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 67:
#line 439 "./src/observer/sql/parser/yacc_sql.y"
                {
                    RelAttr attr;
                    relation_attr_init(&attr, NULL, (yyvsp[0].string), NULL);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 1983 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 68:
#line 445 "./src/observer/sql/parser/yacc_sql.y"
                {
                    RelAttr attr;
                    relation_attr_init(&attr, (yyvsp[-2].string), (yyvsp[0].string), NULL);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 1993 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 69:
#line 451 "./src/observer/sql/parser/yacc_sql.y"
                {
                    RelAttr attr;
                    relation_attr_init(&attr, (yyvsp[-2].string), "*", NULL);
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 2003 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 70:
#line 457 "./src/observer/sql/parser/yacc_sql.y"
                {
                    RelAttr attr;
                    relation_attr_init(&attr, NULL, "*", (yyvsp[-3].string));
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 2013 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 71:
#line 463 "./src/observer/sql/parser/yacc_sql.y"
                {
                    RelAttr attr;
                    relation_attr_init(&attr, NULL, (yyvsp[-1].string), (yyvsp[-3].string));
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 2023 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 72:
#line 469 "./src/observer/sql/parser/yacc_sql.y"
                {
                    RelAttr attr;
                    relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string));
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 2033 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 73:
#line 475 "./src/observer/sql/parser/yacc_sql.y"
                {
                    RelAttr attr;
                    relation_attr_init(&attr, NULL, "*", (yyvsp[-3].string));
                    selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
                }
#line 2043 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 74:
#line 482 "./src/observer/sql/parser/yacc_sql.y"
                {}
#line 2049 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 75:
#line 484 "./src/observer/sql/parser/yacc_sql.y"
                {}
#line 2055 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 77:
#line 495 "./src/observer/sql/parser/yacc_sql.y"
                                               {}
#line 2061 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 78:
#line 499 "./src/observer/sql/parser/yacc_sql.y"
    {
        RelAttr attr;
        relation_attr_init(&attr, NULL, (yyvsp[-1].string), NULL);
        selects_append_order_attrs(&CONTEXT->ssql->sstr.selection, &attr, (yyvsp[0].number));
    }
#line 2071 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 79:
#line 505 "./src/observer/sql/parser/yacc_sql.y"
    {
        RelAttr attr;
        relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), NULL);
        selects_append_order_attrs(&CONTEXT->ssql->sstr.selection, &attr, (yyvsp[0].number));
    }
#line 2081 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 80:
#line 512 "./src/observer/sql/parser/yacc_sql.y"
                { (yyval.number) = 0; }
#line 2087 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 81:
#line 513 "./src/observer/sql/parser/yacc_sql.y"
          { (yyval.number) = 0; }
#line 2093 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 82:
#line 514 "./src/observer/sql/parser/yacc_sql.y"
           { (yyval.number) = 1; }
#line 2099 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 84:
#line 518 "./src/observer/sql/parser/yacc_sql.y"
                                            {}
#line 2105 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 86:
#line 522 "./src/observer/sql/parser/yacc_sql.y"
                                               {}
#line 2111 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 87:
#line 526 "./src/observer/sql/parser/yacc_sql.y"
    {
        RelAttr attr;
        relation_attr_init(&attr, NULL, (yyvsp[0].string), NULL);
        selects_append_group_attrs(&CONTEXT->ssql->sstr.selection, &attr);
    }
#line 2121 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 88:
#line 532 "./src/observer/sql/parser/yacc_sql.y"
    {
        RelAttr attr;
        relation_attr_init(&attr, (yyvsp[-2].string), (yyvsp[0].string), NULL);
        selects_append_group_attrs(&CONTEXT->ssql->sstr.selection, &attr);
    }
#line 2131 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 90:
#line 540 "./src/observer/sql/parser/yacc_sql.y"
                                            {}
#line 2137 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 92:
#line 544 "./src/observer/sql/parser/yacc_sql.y"
                                     {	
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 2145 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 94:
#line 550 "./src/observer/sql/parser/yacc_sql.y"
                                   {
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 2153 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 95:
#line 556 "./src/observer/sql/parser/yacc_sql.y"
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
#line 2178 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 96:
#line 577 "./src/observer/sql/parser/yacc_sql.y"
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
#line 2202 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 97:
#line 597 "./src/observer/sql/parser/yacc_sql.y"
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
#line 2226 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 98:
#line 617 "./src/observer/sql/parser/yacc_sql.y"
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
#line 2252 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 99:
#line 639 "./src/observer/sql/parser/yacc_sql.y"
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
#line 2277 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 100:
#line 660 "./src/observer/sql/parser/yacc_sql.y"
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
#line 2302 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 101:
#line 681 "./src/observer/sql/parser/yacc_sql.y"
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
#line 2325 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 102:
#line 702 "./src/observer/sql/parser/yacc_sql.y"
             { CONTEXT->comp = EQUAL_TO; }
#line 2331 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 103:
#line 703 "./src/observer/sql/parser/yacc_sql.y"
         { CONTEXT->comp = LESS_THAN; }
#line 2337 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 104:
#line 704 "./src/observer/sql/parser/yacc_sql.y"
         { CONTEXT->comp = GREAT_THAN; }
#line 2343 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 105:
#line 705 "./src/observer/sql/parser/yacc_sql.y"
         { CONTEXT->comp = LESS_EQUAL; }
#line 2349 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 106:
#line 706 "./src/observer/sql/parser/yacc_sql.y"
         { CONTEXT->comp = GREAT_EQUAL; }
#line 2355 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 107:
#line 707 "./src/observer/sql/parser/yacc_sql.y"
         { CONTEXT->comp = NOT_EQUAL; }
#line 2361 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 108:
#line 708 "./src/observer/sql/parser/yacc_sql.y"
         { CONTEXT->comp = IS_NULL; }
#line 2367 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 109:
#line 709 "./src/observer/sql/parser/yacc_sql.y"
             { CONTEXT->comp = IS_NOT_NULL; }
#line 2373 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;

  case 110:
#line 714 "./src/observer/sql/parser/yacc_sql.y"
                {
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, (yyvsp[-1].string), (yyvsp[-4].string));
		}
#line 2382 "./src/observer/sql/parser/yacc_sql.tab.c"
    break;


#line 2386 "./src/observer/sql/parser/yacc_sql.tab.c"

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
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

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
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
                  yystos[+*yyssp], yyvsp, scanner);
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
#line 719 "./src/observer/sql/parser/yacc_sql.y"

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
