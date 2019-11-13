/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     lex0 = 258,
     lex1 = 259,
     lex2 = 260,
     lex3 = 261,
     lex4 = 262,
     lex5 = 263,
     lex6 = 264,
     lpar = 265,
     rpar = 266,
     lbkt = 267,
     rbkt = 268,
     eol = 269,
     unk = 270,
     com = 271,
     com0 = 272,
     quad = 273,
     asg = 274,
     null = 275,
     dot = 276,
     cln = 277,
     semi = 278,
     comnt = 279,
     tran = 280,
     strng = 281,
     nam = 282,
     numb = 283,
     nfun = 284,
     mfun = 285,
     dfun = 286,
     comexpr = 287,
     comnam = 288,
     comnull = 289,
     comlist = 290,
     dscal = 291,
     mdscal = 292,
     m = 293,
     d = 294,
     md = 295,
     msub = 296,
     mdsub = 297
   };
#endif
/* Tokens.  */
#define lex0 258
#define lex1 259
#define lex2 260
#define lex3 261
#define lex4 262
#define lex5 263
#define lex6 264
#define lpar 265
#define rpar 266
#define lbkt 267
#define rbkt 268
#define eol 269
#define unk 270
#define com 271
#define com0 272
#define quad 273
#define asg 274
#define null 275
#define dot 276
#define cln 277
#define semi 278
#define comnt 279
#define tran 280
#define strng 281
#define nam 282
#define numb 283
#define nfun 284
#define mfun 285
#define dfun 286
#define comexpr 287
#define comnam 288
#define comnull 289
#define comlist 290
#define dscal 291
#define mdscal 292
#define m 293
#define d 294
#define md 295
#define msub 296
#define mdsub 297




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 22 "apl.y"
{
	char	*charptr;
	char	charval;
}
/* Line 1529 of yacc.c.  */
#line 138 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

