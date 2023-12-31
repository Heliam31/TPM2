/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.yy"

	#include <iostream>
	#include <vector>
	#include <algorithm>
	#include "AST.hpp"
	using namespace std;

	extern int lexer_line;
	extern const char *lexer_file;

	int yylex();
	void yyerror(char const *msg) {
		throw ParseException(Position(), msg);
	}

	vector<State *> states;
	vector<When *> whens;

	Declaration *checkLoc(string name, int line) {
		auto d = Declaration::getSymbol(name);
		if(d == nullptr)
			throw ParseException(Position(line), name + " does not exist.");
		if(d->type() != Declaration::REG
		&& d->type() != Declaration::VAR)
			throw ParseException(Position(line), name + " should be a register or a variable.");
		return d;
	}

	Declaration *checkMem(string name, int line) {
		auto d = Declaration::getSymbol(name);
		if(d == nullptr)
			throw ParseException(Position(line), name + " does not exist.");
		if(d->type() != Declaration::REG
		&& d->type() != Declaration::VAR
		&& d->type() != Declaration::CST)
			throw ParseException(Position(line), name + " should be a register, a variable or a constant.");
		return d;
	}

#line 111 "parser.cpp"

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

#include "parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_INT = 4,                        /* INT  */
  YYSYMBOL_DOTDOT = 5,                     /* DOTDOT  */
  YYSYMBOL_GE = 6,                         /* GE  */
  YYSYMBOL_LE = 7,                         /* LE  */
  YYSYMBOL_NE = 8,                         /* NE  */
  YYSYMBOL_AND = 9,                        /* AND  */
  YYSYMBOL_AUTO = 10,                      /* AUTO  */
  YYSYMBOL_CONST = 11,                     /* CONST  */
  YYSYMBOL_ELSE = 12,                      /* ELSE  */
  YYSYMBOL_ENDIF = 13,                     /* ENDIF  */
  YYSYMBOL_GOTO = 14,                      /* GOTO  */
  YYSYMBOL_GT2 = 15,                       /* GT2  */
  YYSYMBOL_GT3 = 16,                       /* GT3  */
  YYSYMBOL_IF = 17,                        /* IF  */
  YYSYMBOL_LT2 = 18,                       /* LT2  */
  YYSYMBOL_LT3 = 19,                       /* LT3  */
  YYSYMBOL_NOT = 20,                       /* NOT  */
  YYSYMBOL_OR = 21,                        /* OR  */
  YYSYMBOL_REG = 22,                       /* REG  */
  YYSYMBOL_SIG = 23,                       /* SIG  */
  YYSYMBOL_STATE = 24,                     /* STATE  */
  YYSYMBOL_STOP = 25,                      /* STOP  */
  YYSYMBOL_THEN = 26,                      /* THEN  */
  YYSYMBOL_VAR = 27,                       /* VAR  */
  YYSYMBOL_WHEN = 28,                      /* WHEN  */
  YYSYMBOL_29_ = 29,                       /* '~'  */
  YYSYMBOL_30_ = 30,                       /* '['  */
  YYSYMBOL_31_ = 31,                       /* '+'  */
  YYSYMBOL_32_ = 32,                       /* '-'  */
  YYSYMBOL_33_ = 33,                       /* '*'  */
  YYSYMBOL_34_ = 34,                       /* '/'  */
  YYSYMBOL_35_ = 35,                       /* '%'  */
  YYSYMBOL_36_ = 36,                       /* '&'  */
  YYSYMBOL_37_ = 37,                       /* '|'  */
  YYSYMBOL_38_ = 38,                       /* '^'  */
  YYSYMBOL_39_ = 39,                       /* '='  */
  YYSYMBOL_40_ = 40,                       /* '@'  */
  YYSYMBOL_41_ = 41,                       /* ']'  */
  YYSYMBOL_42_ = 42,                       /* ':'  */
  YYSYMBOL_43_ = 43,                       /* '!'  */
  YYSYMBOL_44_ = 44,                       /* '<'  */
  YYSYMBOL_45_ = 45,                       /* '>'  */
  YYSYMBOL_46_ = 46,                       /* '('  */
  YYSYMBOL_47_ = 47,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 48,                  /* $accept  */
  YYSYMBOL_top = 49,                       /* top  */
  YYSYMBOL_line = 50,                      /* line  */
  YYSYMBOL_opt_decls = 51,                 /* opt_decls  */
  YYSYMBOL_decls = 52,                     /* decls  */
  YYSYMBOL_decl = 53,                      /* decl  */
  YYSYMBOL_states = 54,                    /* states  */
  YYSYMBOL_state = 55,                     /* state  */
  YYSYMBOL_opt_whens = 56,                 /* opt_whens  */
  YYSYMBOL_whens = 57,                     /* whens  */
  YYSYMBOL_when = 58,                      /* when  */
  YYSYMBOL_opt_not = 59,                   /* opt_not  */
  YYSYMBOL_opt_stmts = 60,                 /* opt_stmts  */
  YYSYMBOL_stmts = 61,                     /* stmts  */
  YYSYMBOL_stmt = 62,                      /* stmt  */
  YYSYMBOL_cond = 63,                      /* cond  */
  YYSYMBOL_expr = 64,                      /* expr  */
  YYSYMBOL_atom = 65                       /* atom  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   368

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  172

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   283


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    43,     2,     2,     2,    35,    36,     2,
      46,    47,    33,    31,     2,    32,     2,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    42,     2,
      44,    39,    45,     2,    40,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    30,     2,    41,    38,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    37,     2,    29,     2,     2,     2,
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
      25,    26,    27,    28
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    93,    93,    98,   102,   104,   109,   111,   117,   127,
     130,   140,   158,   170,   172,   177,   191,   193,   198,   200,
     205,   219,   221,   226,   228,   233,   235,   241,   249,   256,
     263,   265,   267,   269,   274,   276,   278,   280,   282,   284,
     286,   288,   290,   292,   297,   299,   301,   303,   305,   307,
     309,   311,   313,   315,   317,   319,   321,   326,   328,   336,
     339,   342,   345,   348,   350
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "INT", "DOTDOT",
  "GE", "LE", "NE", "AND", "AUTO", "CONST", "ELSE", "ENDIF", "GOTO", "GT2",
  "GT3", "IF", "LT2", "LT3", "NOT", "OR", "REG", "SIG", "STATE", "STOP",
  "THEN", "VAR", "WHEN", "'~'", "'['", "'+'", "'-'", "'*'", "'/'", "'%'",
  "'&'", "'|'", "'^'", "'='", "'@'", "']'", "':'", "'!'", "'<'", "'>'",
  "'('", "')'", "$accept", "top", "line", "opt_decls", "decls", "decl",
  "states", "state", "opt_whens", "whens", "when", "opt_not", "opt_stmts",
  "stmts", "stmt", "cond", "expr", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-120)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     340,  -120,  -120,  -120,  -120,  -120,    11,  -120,   340,  -120,
      13,    24,    33,    42,    53,  -120,  -120,     9,   -25,    18,
      40,  -120,  -120,  -120,  -120,  -120,    66,     9,  -120,    28,
      28,    89,   -22,    90,    15,  -120,    66,  -120,  -120,  -120,
    -120,  -120,  -120,  -120,    28,   305,    65,   305,    84,    28,
      28,  -120,  -120,    15,     4,   165,   112,  -120,  -120,  -120,
      28,    28,    28,   235,  -120,  -120,  -120,  -120,  -120,  -120,
    -120,  -120,  -120,  -120,  -120,  -120,    28,    28,   200,   305,
      15,   104,   131,  -120,  -120,     9,  -120,  -120,  -120,  -120,
    -120,  -120,    74,    65,  -120,  -120,  -120,    28,    28,    28,
      28,    28,    28,    28,    28,    28,    28,    28,    28,   224,
     259,    28,    85,    70,  -120,    15,    15,   341,    28,    28,
      28,    28,    28,    28,     9,  -120,  -120,  -120,  -120,    93,
      93,   170,   170,   170,   175,    -9,    -9,    28,  -120,  -120,
     270,    28,  -120,   114,     9,  -120,   305,   305,   305,   305,
     305,   305,   106,   294,    96,   305,   343,  -120,  -120,   106,
    -120,  -120,    28,  -120,    98,  -120,   305,  -120,   129,   100,
       9,  -120
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     3,     3,     3,     3,     3,     0,     2,     5,     6,
       0,     0,     0,     0,     0,     1,     7,    23,     0,     0,
       0,     9,     3,     3,     3,    33,     0,    24,    25,     0,
       0,     0,     0,     0,     0,     3,    12,    13,    26,     3,
       3,     3,     3,     3,     0,     8,    44,    10,     0,     0,
       0,    32,     3,     0,     0,     0,     0,    14,    58,    57,
       0,     0,     0,     0,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     0,     0,     0,    27,
       0,     0,     0,     3,     3,     0,     3,     3,     3,     3,
       3,     3,     0,    61,    63,    62,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,    54,    56,    53,    55,    45,
      46,    47,    48,    49,    50,    51,    52,     0,    59,    11,
       0,     0,    41,    42,     0,    30,    39,    37,    35,    34,
      36,    38,    16,     0,     0,    28,     0,     3,    15,    17,
      18,    60,     0,    31,    21,    19,    29,    22,     0,     0,
      23,    20
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -120,  -120,    -2,  -120,  -120,   135,  -120,   108,  -120,  -120,
     -14,  -120,  -119,   -61,   -23,   -38,    -1,    -7
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     6,    10,     7,     8,     9,    36,    37,   158,   159,
     160,   168,    26,    27,    28,    54,    55,    46
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      11,    12,    13,    14,    38,   152,    64,    65,    49,    66,
      67,    15,    22,    83,    29,    81,    17,    50,    39,    40,
      32,    33,    34,    23,   117,    84,    24,    18,    45,    47,
      85,    39,    40,    56,    25,    52,    19,    58,    59,    60,
      61,    62,   113,    63,    41,    20,    42,    43,    78,    79,
      80,   171,    82,    93,    94,    95,    21,    41,    30,    42,
      43,    53,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,    44,   109,   110,   142,   143,    83,
      31,   115,   116,   156,   118,   119,   120,   121,   122,   123,
      35,    84,    48,    51,    38,    76,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,    64,    65,
     140,    66,    67,    83,    77,    92,   124,   146,   147,   148,
     149,   150,   151,    83,   141,    84,    70,    71,    72,    73,
      74,    75,   169,    38,   157,   162,   153,    86,    87,    88,
     155,   167,   170,    16,    57,   165,    64,    65,     0,    66,
      67,   114,     0,     0,     0,   164,     0,     0,     0,     0,
       0,   166,    68,    69,    70,    71,    72,    73,    74,    75,
      89,    86,    87,    88,     0,    90,    91,     0,    96,     0,
      64,    65,     0,    66,    67,    64,    65,     0,    66,    67,
      64,    65,     0,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,    75,    89,   111,    73,    74,    75,    90,
      91,     0,    74,    75,     0,    64,    65,     0,    66,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   137,
       0,    68,    69,    70,    71,    72,    73,    74,    75,    64,
      65,   112,    66,    67,     0,     0,     0,     0,     0,     0,
      64,    65,     0,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,     0,     0,   138,    68,    69,    70,    71,
      72,    73,    74,    75,    64,    65,     0,    66,    67,     0,
       0,     0,    96,     0,     0,    64,    65,     0,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,     0,     0,
     139,    68,    69,    70,    71,    72,    73,    74,    75,    64,
      65,   154,    66,    67,     0,     0,     0,     0,     0,     0,
      64,    65,     0,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,     0,     0,   161,    68,    69,    70,    71,
      72,    73,    74,    75,    22,     0,    22,     0,     0,     0,
       1,     2,     0,   144,   145,    23,   163,    23,    24,     0,
      24,     0,     3,     4,     0,     0,    25,     5,    25
};

static const yytype_int16 yycheck[] =
{
       2,     3,     4,     5,    27,   124,    15,    16,    30,    18,
      19,     0,     3,     9,    39,    53,     3,    39,     3,     4,
      22,    23,    24,    14,    85,    21,    17,     3,    29,    30,
      26,     3,     4,    35,    25,    20,     3,    39,    40,    41,
      42,    43,    80,    44,    29,     3,    31,    32,    49,    50,
      52,   170,    53,    60,    61,    62,     3,    29,    40,    31,
      32,    46,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    46,    76,    77,   115,   116,     9,
      40,    83,    84,   144,    86,    87,    88,    89,    90,    91,
      24,    21,     3,     3,   117,    30,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,    15,    16,
     111,    18,    19,     9,    30,     3,    42,   118,   119,   120,
     121,   122,   123,     9,    39,    21,    33,    34,    35,    36,
      37,    38,     3,   156,    28,    39,   137,     6,     7,     8,
     141,    43,    42,     8,    36,   159,    15,    16,    -1,    18,
      19,    47,    -1,    -1,    -1,   157,    -1,    -1,    -1,    -1,
      -1,   162,    31,    32,    33,    34,    35,    36,    37,    38,
      39,     6,     7,     8,    -1,    44,    45,    -1,    47,    -1,
      15,    16,    -1,    18,    19,    15,    16,    -1,    18,    19,
      15,    16,    -1,    18,    19,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,     5,    36,    37,    38,    44,
      45,    -1,    37,    38,    -1,    15,    16,    -1,    18,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    15,
      16,    41,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    16,    -1,    18,    19,    31,    32,    33,    34,    35,
      36,    37,    38,    -1,    -1,    41,    31,    32,    33,    34,
      35,    36,    37,    38,    15,    16,    -1,    18,    19,    -1,
      -1,    -1,    47,    -1,    -1,    15,    16,    -1,    18,    19,
      31,    32,    33,    34,    35,    36,    37,    38,    -1,    -1,
      41,    31,    32,    33,    34,    35,    36,    37,    38,    15,
      16,    41,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    16,    -1,    18,    19,    31,    32,    33,    34,    35,
      36,    37,    38,    -1,    -1,    41,    31,    32,    33,    34,
      35,    36,    37,    38,     3,    -1,     3,    -1,    -1,    -1,
      10,    11,    -1,    12,    13,    14,    13,    14,    17,    -1,
      17,    -1,    22,    23,    -1,    -1,    25,    27,    25
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    10,    11,    22,    23,    27,    49,    51,    52,    53,
      50,    50,    50,    50,    50,     0,    53,     3,     3,     3,
       3,     3,     3,    14,    17,    25,    60,    61,    62,    39,
      40,    40,    50,    50,    50,    24,    54,    55,    62,     3,
       4,    29,    31,    32,    46,    64,    65,    64,     3,    30,
      39,     3,    20,    46,    63,    64,    50,    55,    50,    50,
      50,    50,    50,    64,    15,    16,    18,    19,    31,    32,
      33,    34,    35,    36,    37,    38,    30,    30,    64,    64,
      50,    63,    64,     9,    21,    26,     6,     7,     8,    39,
      44,    45,     3,    65,    65,    65,    47,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    64,
      64,     5,    41,    63,    47,    50,    50,    61,    50,    50,
      50,    50,    50,    50,    42,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,     5,    41,    41,
      64,    39,    63,    63,    12,    13,    64,    64,    64,    64,
      64,    64,    60,    64,    41,    64,    61,    28,    56,    57,
      58,    41,    39,    13,    50,    58,    64,    43,    59,     3,
      42,    60
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    48,    49,    50,    51,    51,    52,    52,    53,    53,
      53,    53,    53,    54,    54,    55,    56,    56,    57,    57,
      58,    59,    59,    60,    60,    61,    61,    62,    62,    62,
      62,    62,    62,    62,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    65,    65,    65,
      65,    65,    65,    65,    65
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     0,     1,     1,     2,     5,     3,
       5,     8,     5,     1,     2,     6,     0,     1,     1,     2,
       6,     0,     1,     0,     1,     1,     2,     4,     7,     9,
       6,     8,     3,     1,     4,     4,     4,     4,     4,     4,
       3,     4,     4,     3,     1,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     2,     2,     4,
       6,     3,     3,     3,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  case 2: /* top: opt_decls  */
#line 94 "parser.yy"
        { }
#line 1301 "parser.cpp"
    break;

  case 3: /* line: %empty  */
#line 99 "parser.yy"
                { (yyval.line) = lexer_line; }
#line 1307 "parser.cpp"
    break;

  case 4: /* opt_decls: %empty  */
#line 103 "parser.yy"
                { }
#line 1313 "parser.cpp"
    break;

  case 5: /* opt_decls: decls  */
#line 105 "parser.yy"
                { }
#line 1319 "parser.cpp"
    break;

  case 6: /* decls: decl  */
#line 110 "parser.yy"
                { }
#line 1325 "parser.cpp"
    break;

  case 7: /* decls: decls decl  */
#line 112 "parser.yy"
                { }
#line 1331 "parser.cpp"
    break;

  case 8: /* decl: CONST line ID '=' expr  */
#line 118 "parser.yy"
                {
			auto x = (yyvsp[0].expr)->eval();
			if(!x)
				throw ParseException((yyvsp[0].expr)->pos, "should be a constant!");
			delete (yyvsp[0].expr);
			(new ConstDecl((yyvsp[-2].ID), *x))->setLine((yyvsp[-3].line));
			free((yyvsp[-2].ID));
		}
#line 1344 "parser.cpp"
    break;

  case 9: /* decl: VAR line ID  */
#line 128 "parser.yy"
                { (new VarDecl((yyvsp[0].ID)))->setLine((yyvsp[-1].line)); free((yyvsp[0].ID)); }
#line 1350 "parser.cpp"
    break;

  case 10: /* decl: REG line ID '@' expr  */
#line 131 "parser.yy"
                {
			auto x = (yyvsp[0].expr)->eval();
			if(!x)
				throw ParseException((yyvsp[0].expr)->pos, "should be a constant!");
			delete (yyvsp[0].expr);
			(new RegDecl((yyvsp[-2].ID), *x))->setLine((yyvsp[-3].line));
			free((yyvsp[-2].ID));
		}
#line 1363 "parser.cpp"
    break;

  case 11: /* decl: SIG line ID '@' ID '[' expr ']'  */
#line 141 "parser.yy"
                {
			auto dec = Declaration::getSymbol((yyvsp[-3].ID));
			if(dec == nullptr)
				throw ParseException(Position((yyvsp[-6].line)), string((yyvsp[-3].ID)) + " does not exist!");
			if(dec->type() != Declaration::REG)
				throw ParseException(Position((yyvsp[-6].line)), string((yyvsp[-3].ID)) + " must be a register.");
			auto x = (yyvsp[-1].expr)->eval();
			if(!x)
				throw ParseException((yyvsp[-1].expr)->pos, "bit number should be a constant!");
			if(*x >= 32)
				throw ParseException((yyvsp[-1].expr)->pos, "bit number must be less than 32!");
			(new SigDecl((yyvsp[-5].ID), static_cast<RegDecl *>(dec), *x))->setLine((yyvsp[-6].line));
			delete (yyvsp[-1].expr);
			free((yyvsp[-5].ID));
			free((yyvsp[-3].ID));
		}
#line 1384 "parser.cpp"
    break;

  case 12: /* decl: AUTO line ID opt_stmts states  */
#line 159 "parser.yy"
                {
			(yyvsp[-1].opt_stmts)->fix(states);
			for(auto s: states)
				s->fix(states);
			(new AutoDecl((yyvsp[-2].ID), (yyvsp[-1].opt_stmts), states))->setLine((yyvsp[-3].line));
			free((yyvsp[-2].ID));
			states.clear();
		}
#line 1397 "parser.cpp"
    break;

  case 13: /* states: state  */
#line 171 "parser.yy"
                { }
#line 1403 "parser.cpp"
    break;

  case 14: /* states: states state  */
#line 173 "parser.yy"
                { }
#line 1409 "parser.cpp"
    break;

  case 15: /* state: STATE line ID ':' opt_stmts opt_whens  */
#line 178 "parser.yy"
                {
			for(auto s: states)
				if(s->name() == (yyvsp[-3].ID))
					throw ParseException(Position((yyvsp[-4].line)), string(" state ") + (yyvsp[-3].ID) + " already exists.");
			auto s = new State((yyvsp[-3].ID), (yyvsp[-1].opt_stmts), whens);
			s->setLine((yyvsp[-4].line));
			states.push_back(s);
			whens.clear();
			free((yyvsp[-3].ID));
		}
#line 1424 "parser.cpp"
    break;

  case 16: /* opt_whens: %empty  */
#line 192 "parser.yy"
                { }
#line 1430 "parser.cpp"
    break;

  case 17: /* opt_whens: whens  */
#line 194 "parser.yy"
                { }
#line 1436 "parser.cpp"
    break;

  case 18: /* whens: when  */
#line 199 "parser.yy"
                { }
#line 1442 "parser.cpp"
    break;

  case 19: /* whens: whens when  */
#line 201 "parser.yy"
                { }
#line 1448 "parser.cpp"
    break;

  case 20: /* when: WHEN line opt_not ID ':' opt_stmts  */
#line 206 "parser.yy"
                {
			auto s = Declaration::getSymbol((yyvsp[-2].ID));
			if(s == nullptr)
				throw ParseException(Position((yyvsp[-4].line)), string((yyvsp[-2].ID)) + " does not exist.");
			if(s->type() != Declaration::SIG)
				throw ParseException(Position((yyvsp[-4].line)), string((yyvsp[-2].ID)) + " should be a singal!");
			auto w = new When((yyvsp[-3].opt_not), static_cast<SigDecl *>(s), (yyvsp[0].opt_stmts));
			w->setLine((yyvsp[-4].line));
			whens.push_back(w);
		}
#line 1463 "parser.cpp"
    break;

  case 21: /* opt_not: %empty  */
#line 220 "parser.yy"
                { (yyval.opt_not) = false; }
#line 1469 "parser.cpp"
    break;

  case 22: /* opt_not: '!'  */
#line 222 "parser.yy"
                { (yyval.opt_not) = true; }
#line 1475 "parser.cpp"
    break;

  case 23: /* opt_stmts: %empty  */
#line 227 "parser.yy"
                { (yyval.opt_stmts) = new NOPStatement(); }
#line 1481 "parser.cpp"
    break;

  case 24: /* opt_stmts: stmts  */
#line 229 "parser.yy"
                { (yyval.opt_stmts) = (yyvsp[0].stmts); }
#line 1487 "parser.cpp"
    break;

  case 25: /* stmts: stmt  */
#line 234 "parser.yy"
                { (yyval.stmts) = (yyvsp[0].stmt); }
#line 1493 "parser.cpp"
    break;

  case 26: /* stmts: stmts stmt  */
#line 236 "parser.yy"
                { (yyval.stmts) = new SeqStatement((yyvsp[-1].stmts), (yyvsp[0].stmt)); }
#line 1499 "parser.cpp"
    break;

  case 27: /* stmt: ID line '=' expr  */
#line 242 "parser.yy"
                {
			auto d = checkLoc((yyvsp[-3].ID), (yyvsp[-2].line));
			(yyval.stmt) = new SetStatement(d, (yyvsp[0].expr));
			(yyval.stmt)->setLine((yyvsp[-2].line));
			free((yyvsp[-3].ID));
		}
#line 1510 "parser.cpp"
    break;

  case 28: /* stmt: ID line '[' expr ']' '=' expr  */
#line 250 "parser.yy"
                {
			auto d = checkLoc((yyvsp[-6].ID), (yyvsp[-5].line));
			(yyval.stmt) = new SetFieldStatement(d, (yyvsp[-3].expr), (yyvsp[-3].expr), (yyvsp[0].expr));
			(yyval.stmt)->setLine((yyvsp[-5].line));
			free((yyvsp[-6].ID));
		}
#line 1521 "parser.cpp"
    break;

  case 29: /* stmt: ID line '[' expr DOTDOT expr ']' '=' expr  */
#line 257 "parser.yy"
                {
			auto d = checkLoc((yyvsp[-8].ID), (yyvsp[-7].line));
			(yyval.stmt) = new SetFieldStatement(d, (yyvsp[-5].expr), (yyvsp[-3].expr), (yyvsp[0].expr));
			(yyval.stmt)->setLine((yyvsp[-7].line));
			free((yyvsp[-8].ID));
		}
#line 1532 "parser.cpp"
    break;

  case 30: /* stmt: IF line cond THEN stmts ENDIF  */
#line 264 "parser.yy"
                { (yyval.stmt) = new IfStatement((yyvsp[-3].cond), (yyvsp[-1].stmts), new NOPStatement()); (yyval.stmt)->setLine((yyvsp[-4].line)); }
#line 1538 "parser.cpp"
    break;

  case 31: /* stmt: IF line cond THEN stmts ELSE stmts ENDIF  */
#line 266 "parser.yy"
                { (yyval.stmt) = new IfStatement((yyvsp[-5].cond), (yyvsp[-3].stmts), (yyvsp[-1].stmts)); (yyval.stmt)->setLine((yyvsp[-6].line)); }
#line 1544 "parser.cpp"
    break;

  case 32: /* stmt: GOTO line ID  */
#line 268 "parser.yy"
                { (yyval.stmt) = new GotoStatement((yyvsp[0].ID)); (yyval.stmt)->setLine((yyvsp[-1].line)); }
#line 1550 "parser.cpp"
    break;

  case 33: /* stmt: STOP  */
#line 270 "parser.yy"
                { (yyval.stmt) = new StopStatement(); }
#line 1556 "parser.cpp"
    break;

  case 34: /* cond: expr '=' line expr  */
#line 275 "parser.yy"
                { (yyval.cond) = new CompCond(CompCond::EQ, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.cond)->setLine((yyvsp[-1].line)); }
#line 1562 "parser.cpp"
    break;

  case 35: /* cond: expr NE line expr  */
#line 277 "parser.yy"
                { (yyval.cond) = new CompCond(CompCond::NE, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.cond)->setLine((yyvsp[-1].line)); }
#line 1568 "parser.cpp"
    break;

  case 36: /* cond: expr '<' line expr  */
#line 279 "parser.yy"
                { (yyval.cond) = new CompCond(CompCond::LT, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.cond)->setLine((yyvsp[-1].line)); }
#line 1574 "parser.cpp"
    break;

  case 37: /* cond: expr LE line expr  */
#line 281 "parser.yy"
                { (yyval.cond) = new CompCond(CompCond::LE, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.cond)->setLine((yyvsp[-1].line)); }
#line 1580 "parser.cpp"
    break;

  case 38: /* cond: expr '>' line expr  */
#line 283 "parser.yy"
                { (yyval.cond) = new CompCond(CompCond::GT, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.cond)->setLine((yyvsp[-1].line)); }
#line 1586 "parser.cpp"
    break;

  case 39: /* cond: expr GE line expr  */
#line 285 "parser.yy"
                { (yyval.cond) = new CompCond(CompCond::GE, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.cond)->setLine((yyvsp[-1].line)); }
#line 1592 "parser.cpp"
    break;

  case 40: /* cond: NOT line cond  */
#line 287 "parser.yy"
                { (yyval.cond) = new NotCond((yyvsp[0].cond)); (yyval.cond)->setLine((yyvsp[-1].line)); }
#line 1598 "parser.cpp"
    break;

  case 41: /* cond: cond AND line cond  */
#line 289 "parser.yy"
                { (yyval.cond) = new AndCond((yyvsp[-3].cond), (yyvsp[0].cond)); (yyval.cond)->setLine((yyvsp[-1].line)); }
#line 1604 "parser.cpp"
    break;

  case 42: /* cond: cond OR line cond  */
#line 291 "parser.yy"
                { (yyval.cond) = new OrCond((yyvsp[-3].cond), (yyvsp[0].cond)); (yyval.cond)->setLine((yyvsp[-1].line)); }
#line 1610 "parser.cpp"
    break;

  case 43: /* cond: '(' cond ')'  */
#line 293 "parser.yy"
                { (yyval.cond) = (yyvsp[-1].cond); }
#line 1616 "parser.cpp"
    break;

  case 44: /* expr: atom  */
#line 298 "parser.yy"
                { (yyval.expr) = (yyvsp[0].atom); }
#line 1622 "parser.cpp"
    break;

  case 45: /* expr: expr '+' line expr  */
#line 300 "parser.yy"
                { (yyval.expr) = new BinopExpr(BinopExpr::ADD, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.expr)->setLine((yyvsp[-1].line)); }
#line 1628 "parser.cpp"
    break;

  case 46: /* expr: expr '-' line expr  */
#line 302 "parser.yy"
                { (yyval.expr) = new BinopExpr(BinopExpr::SUB, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.expr)->setLine((yyvsp[-1].line)); }
#line 1634 "parser.cpp"
    break;

  case 47: /* expr: expr '*' line expr  */
#line 304 "parser.yy"
                { (yyval.expr) = new BinopExpr(BinopExpr::MUL, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.expr)->setLine((yyvsp[-1].line)); }
#line 1640 "parser.cpp"
    break;

  case 48: /* expr: expr '/' line expr  */
#line 306 "parser.yy"
                { (yyval.expr) = new BinopExpr(BinopExpr::DIV, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.expr)->setLine((yyvsp[-1].line)); }
#line 1646 "parser.cpp"
    break;

  case 49: /* expr: expr '%' line expr  */
#line 308 "parser.yy"
                { (yyval.expr) = new BinopExpr(BinopExpr::MOD, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.expr)->setLine((yyvsp[-1].line)); }
#line 1652 "parser.cpp"
    break;

  case 50: /* expr: expr '&' line expr  */
#line 310 "parser.yy"
                { (yyval.expr) = new BinopExpr(BinopExpr::BIT_AND, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.expr)->setLine((yyvsp[-1].line)); }
#line 1658 "parser.cpp"
    break;

  case 51: /* expr: expr '|' line expr  */
#line 312 "parser.yy"
                { (yyval.expr) = new BinopExpr(BinopExpr::BIT_OR, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.expr)->setLine((yyvsp[-1].line)); }
#line 1664 "parser.cpp"
    break;

  case 52: /* expr: expr '^' line expr  */
#line 314 "parser.yy"
                { (yyval.expr) = new BinopExpr(BinopExpr::XOR, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.expr)->setLine((yyvsp[-1].line)); }
#line 1670 "parser.cpp"
    break;

  case 53: /* expr: expr LT2 line expr  */
#line 316 "parser.yy"
                { (yyval.expr) = new BinopExpr(BinopExpr::SHL, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.expr)->setLine((yyvsp[-1].line)); }
#line 1676 "parser.cpp"
    break;

  case 54: /* expr: expr GT2 line expr  */
#line 318 "parser.yy"
                { (yyval.expr) = new BinopExpr(BinopExpr::SHR, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.expr)->setLine((yyvsp[-1].line)); }
#line 1682 "parser.cpp"
    break;

  case 55: /* expr: expr LT3 line expr  */
#line 320 "parser.yy"
                { (yyval.expr) = new BinopExpr(BinopExpr::ROL, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.expr)->setLine((yyvsp[-1].line)); }
#line 1688 "parser.cpp"
    break;

  case 56: /* expr: expr GT3 line expr  */
#line 322 "parser.yy"
                { (yyval.expr) = new BinopExpr(BinopExpr::ROR, (yyvsp[-3].expr), (yyvsp[0].expr)); (yyval.expr)->setLine((yyvsp[-1].line)); }
#line 1694 "parser.cpp"
    break;

  case 57: /* atom: INT line  */
#line 327 "parser.yy"
                { (yyval.atom) = new ConstExpr((yyvsp[-1].INT)); (yyval.atom)->setLine((yyvsp[0].line)); }
#line 1700 "parser.cpp"
    break;

  case 58: /* atom: ID line  */
#line 329 "parser.yy"
                {
			auto d = checkMem((yyvsp[-1].ID), (yyvsp[0].line));
			(yyval.atom) = new MemExpr(d);
			(yyval.atom)->setLine((yyvsp[0].line));
			free((yyvsp[-1].ID));
		}
#line 1711 "parser.cpp"
    break;

  case 59: /* atom: atom '[' expr ']'  */
#line 337 "parser.yy"
                { (yyval.atom) = new BitFieldExpr((yyvsp[-3].atom), (yyvsp[-1].expr), (yyvsp[-1].expr)); (yyval.atom)->pos = (yyvsp[-3].atom)->pos;  }
#line 1717 "parser.cpp"
    break;

  case 60: /* atom: atom '[' expr DOTDOT expr ']'  */
#line 340 "parser.yy"
                { (yyval.atom) = new BitFieldExpr((yyvsp[-5].atom), (yyvsp[-3].expr), (yyvsp[-1].expr)); (yyval.atom)->pos = (yyvsp[-5].atom)->pos;  }
#line 1723 "parser.cpp"
    break;

  case 61: /* atom: '~' line atom  */
#line 343 "parser.yy"
                { (yyval.atom) = new UnopExpr(UnopExpr::INV, (yyvsp[0].atom)); (yyval.atom)->setLine((yyvsp[-1].line)); }
#line 1729 "parser.cpp"
    break;

  case 62: /* atom: '-' line atom  */
#line 346 "parser.yy"
                { (yyval.atom) = new UnopExpr(UnopExpr::NEG, (yyvsp[0].atom)); (yyval.atom)->setLine((yyvsp[-1].line)); }
#line 1735 "parser.cpp"
    break;

  case 63: /* atom: '+' line atom  */
#line 349 "parser.yy"
                { (yyval.atom) = (yyvsp[0].atom); }
#line 1741 "parser.cpp"
    break;

  case 64: /* atom: '(' expr ')'  */
#line 351 "parser.yy"
                { (yyval.atom) = (yyvsp[-1].expr); }
#line 1747 "parser.cpp"
    break;


#line 1751 "parser.cpp"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
                      yytoken, &yylval);
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

