/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUMBER = 258,
     ID = 259,
     LD = 260,
     LA = 261,
     LOAD = 262,
     LOADI = 263,
     ADD = 264,
     SUB = 265,
     MUL = 266,
     DIV = 267,
     END = 268,
     ENDP = 269,
     PROCESS = 270,
     AND = 271,
     OR = 272,
     NOT = 273,
     LE_OP = 274,
     GE_OP = 275,
     LT_OP = 276,
     GT_OP = 277,
     EQ_OP = 278,
     NE_OP = 279,
     STOP = 280,
     STOR = 281,
     ST = 282,
     DATA = 283,
     IDATA = 284,
     COMMA = 285,
     COLON = 286,
     LOCK = 287,
     UNLOCK = 288,
     OPEN = 289,
     READ = 290,
     WRITE = 291,
     CLOSE = 292,
     SEEK = 293,
     POPD = 294,
     POP = 295,
     DOT = 296,
     HALT = 297,
     JFALSE = 298,
     JMP = 299,
     LPREC = 300,
     RPREC = 301
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 132 "assembler.y"

  int intval;
  char *sptr;



/* Line 2068 of yacc.c  */
#line 103 "assembler.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


