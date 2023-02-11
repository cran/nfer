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

#ifndef YY_YY_GENSRC_PARSER_DSL_TAB_H_INCLUDED
# define YY_YY_GENSRC_PARSER_DSL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */

/*
 * dsl.y
 *
 *  Created on: Apr 20, 2017
 *      Author: skauffma
 *
 *    nfer - a system for inferring abstractions of event streams
 *   Copyright (C) 2017  Sean Kauffman
 *
 *   This file is part of nfer.
 *   nfer is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "types.h"
#include "dict.h"
#include "log.h"
#include "ast.h"
#include "semantic.h"
#line 46 "src/dsl/dsl.y"

    #define YYLTYPE YYLTYPE
    typedef location_type YYLTYPE;

#line 85 "gensrc/parser/dsl.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INTLITERAL = 258,              /* INTLITERAL  */
    FLOATLITERAL = 259,            /* FLOATLITERAL  */
    IDENTIFIER = 260,              /* IDENTIFIER  */
    STRINGLITERAL = 261,           /* STRINGLITERAL  */
    LPAREN = 262,                  /* LPAREN  */
    RPAREN = 263,                  /* RPAREN  */
    LBRACE = 264,                  /* LBRACE  */
    RBRACE = 265,                  /* RBRACE  */
    LISTSEP = 266,                 /* LISTSEP  */
    MAPSTO = 267,                  /* MAPSTO  */
    LABELS = 268,                  /* LABELS  */
    MODULE = 269,                  /* MODULE  */
    SILENT = 270,                  /* SILENT  */
    LOUD = 271,                    /* LOUD  */
    IMPORT = 272,                  /* IMPORT  */
    WHERE = 273,                   /* WHERE  */
    MAP = 274,                     /* MAP  */
    BEGINTOKEN = 275,              /* BEGINTOKEN  */
    ENDTOKEN = 276,                /* ENDTOKEN  */
    FIELD = 277,                   /* FIELD  */
    YIELDS = 278,                  /* YIELDS  */
    TRUE = 279,                    /* TRUE  */
    FALSE = 280,                   /* FALSE  */
    EOL = 281,                     /* EOL  */
    UNLESS = 282,                  /* UNLESS  */
    ALSO = 283,                    /* ALSO  */
    BEFORE = 284,                  /* BEFORE  */
    MEET = 285,                    /* MEET  */
    DURING = 286,                  /* DURING  */
    START = 287,                   /* START  */
    FINISH = 288,                  /* FINISH  */
    OVERLAP = 289,                 /* OVERLAP  */
    SLICE = 290,                   /* SLICE  */
    COINCIDE = 291,                /* COINCIDE  */
    AFTER = 292,                   /* AFTER  */
    FOLLOW = 293,                  /* FOLLOW  */
    CONTAIN = 294,                 /* CONTAIN  */
    AND = 295,                     /* AND  */
    OR = 296,                      /* OR  */
    GE = 297,                      /* GE  */
    LE = 298,                      /* LE  */
    EQ = 299,                      /* EQ  */
    NE = 300,                      /* NE  */
    GT = 301,                      /* GT  */
    LT = 302,                      /* LT  */
    PLUS = 303,                    /* PLUS  */
    MINUS = 304,                   /* MINUS  */
    MUL = 305,                     /* MUL  */
    DIV = 306,                     /* DIV  */
    MOD = 307,                     /* MOD  */
    UMINUS = 308,                  /* UMINUS  */
    BANG = 309                     /* BANG  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 39 "src/dsl/dsl.y"

    int64_t int_value;
    double float_value;
    word_id string_value;
    ast_node *node;

#line 163 "gensrc/parser/dsl.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (void * scanner, dictionary *parser_dict, ast_node **ast_root);

/* "%code provides" blocks.  */
#line 51 "src/dsl/dsl.y"

int yylex(YYSTYPE * yylval_param, YYLTYPE * llocp, void * yyscanner, dictionary *parser_dict);
void yyerror(YYLTYPE * yylloc, void * scanner, dictionary *parser_dict, ast_node **ast_root, const char* message);

#line 196 "gensrc/parser/dsl.tab.h"

#endif /* !YY_YY_GENSRC_PARSER_DSL_TAB_H_INCLUDED  */
