/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ID = 258,                      /* ID  */
    INT = 259,                     /* INT  */
    DOTDOT = 260,                  /* DOTDOT  */
    GE = 261,                      /* GE  */
    LE = 262,                      /* LE  */
    NE = 263,                      /* NE  */
    AND = 264,                     /* AND  */
    AUTO = 265,                    /* AUTO  */
    CONST = 266,                   /* CONST  */
    ELSE = 267,                    /* ELSE  */
    ENDIF = 268,                   /* ENDIF  */
    GOTO = 269,                    /* GOTO  */
    GT2 = 270,                     /* GT2  */
    GT3 = 271,                     /* GT3  */
    IF = 272,                      /* IF  */
    LT2 = 273,                     /* LT2  */
    LT3 = 274,                     /* LT3  */
    NOT = 275,                     /* NOT  */
    OR = 276,                      /* OR  */
    REG = 277,                     /* REG  */
    SIG = 278,                     /* SIG  */
    STATE = 279,                   /* STATE  */
    STOP = 280,                    /* STOP  */
    THEN = 281,                    /* THEN  */
    VAR = 282,                     /* VAR  */
    WHEN = 283                     /* WHEN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
  Condition * cond;                        /* cond  */
  Expression * expr;                       /* expr  */
  Statement * opt_stmts;                   /* opt_stmts  */
  Statement * stmts;                       /* stmts  */
  Statement * stmt;                        /* stmt  */
  bool opt_not;                            /* opt_not  */
  char * ID;                               /* ID  */
  int line;                                /* line  */
  long int INT;                            /* INT  */

#line 102 "parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
