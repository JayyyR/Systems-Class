/*Timothy Cohen and Joseph Acosta
*PA1
*tacohen@email.wm.edu
*jracosta@email.wm.edu
*/

%{
#include "parser-defs.h"
#include "parser.tab.h"



/*
 * You may want to use this string to assemble the lexeme
 * of string constants by adding the current matched character from yytext to it.
 * You can do this as buffer += yytext;
 */
string buffer;


/*
 * You may want to track the comment desting depth with this variable.
 */
int comment_depth = 0;

%}


%option noyywrap

LETTER  [a-zA-Z]

WHITESPACE [ \t\f\r\v]+
NEWLINE [\n]
COMMA [,]
DOT [.]
HEAD [!] 
TAIL [#]
CONS [@]
PLUS [+]
MINUS [-]
IDENTIFIER [=]
TIMES [*]
DIVIDE [/]
LPAREN [(]
RPAREN [)]
EQ [=]
NEQ [<>]
GT [>]
GEQ [>=]
LT [<]
LEQ [<=]
NIL ["Nil"]
READSTRING []
READINT ["readInt"]
PRINT ["print"]
WITH ["with"]
LET ["let"]
INT [0-9]+
AND [&]
OR [|]
IF ["if"]
THEN ["then"]
ELSE ["else"]
LAMBDA ["lambda"]
FUN ["fun"]
IN ["in"]
QUOTE ["\""]
BEGINCOM ["(*"]
ENDCOM ["*)"]

 
%x STRING
%s STRING_END


%x COMMENT 
%s COMMENT_END

%%


"*)" { 
  SET_LEXEME("The comment wasn't closed");
  return TOKEN_ERROR;
}

"(*" { 
  BEGIN COMMENT; 
}



<COMMENT>"(*" { 
  buffer += yytext;
  comment_depth++; 
}

<COMMENT>"*)" { 
  if(comment_depth == 0 ) {
      /*BEGIN INITIAL;*/
      BEGIN COMMENT_END;
  }
  else{
    comment_depth--;
  }
}

<COMMENT>.|\n { 
  buffer += yytext; 
}


<COMMENT><<EOF>> {
  SET_LEXEME("The comment wasn't closed");
  return TOKEN_ERROR;
}

{QUOTE} {
  BEGIN(STRING);
}

<STRING>{NEWLINE} {
  curr_lineno++;
}

<STRING>[^"\""]* {
  SET_LEXEME(yytext);
  
}

<STRING>["\""] {
  BEGIN(STRING_END);
  return TOKEN_STRING;
}

<STRING><<EOF>> {
  SET_LEXEME("The quotation wasn't closed");
  return TOKEN_ERROR;
}

"\"\"" {
  SET_LEXEME("");
  return TOKEN_STRING;
}

{WHITESPACE} {
  /* Do nothing */
}

{NEWLINE} {
/* Do nothing, but increment line numbers */
 curr_lineno++;
}

{COMMA} {
  SET_LEXEME(yytext);
  return TOKEN_COMMA;
}
{HEAD} {
  SET_LEXEME(yytext);
  return TOKEN_HD;
}
{TAIL} {
  SET_LEXEME(yytext);
  return TOKEN_TL;
}
{CONS} {
  SET_LEXEME(yytext);
  return TOKEN_CONS;
}

"Nil"|"nil" {
  return TOKEN_NIL;
}

"readInt" {
  return TOKEN_READINT;
}
"print" {
  return TOKEN_PRINT;
}
"isNil" {
  return TOKEN_ISNIL;
}
{DOT} {
  SET_LEXEME(yytext);
  return TOKEN_DOT;
}
"with" {
  return TOKEN_WITH;
}
"let" {
  return TOKEN_LET;
}
{PLUS} {
  SET_LEXEME(yytext);
  return TOKEN_PLUS;
}
{MINUS} {
  SET_LEXEME(yytext);
  return TOKEN_MINUS;
}
{TIMES} {
  SET_LEXEME(yytext);
  return TOKEN_TIMES;
}
{DIVIDE} {
  SET_LEXEME(yytext);
  return TOKEN_DIVIDE;
}
{INT} {
  SET_LEXEME(yytext);
  return TOKEN_INT;
}
{LPAREN} {
  SET_LEXEME(yytext);
  return TOKEN_LPAREN;
}
{RPAREN} {
  SET_LEXEME(yytext);
  return TOKEN_RPAREN;
}
{AND} {
  SET_LEXEME(yytext);
  return TOKEN_AND;
}
{OR} {
  SET_LEXEME(yytext);
  return TOKEN_OR;
}
{EQ} {
  SET_LEXEME(yytext);
  return TOKEN_EQ;
}
"<>" {
  return TOKEN_NEQ;
}
">" {
  return TOKEN_GT;
}
">=" {
  return TOKEN_GEQ;
}
"<" {
  return TOKEN_LT;
}
"<=" {
  return TOKEN_LEQ;
}
"if" {
  return TOKEN_IF;
}
"then" {
  return TOKEN_THEN;
}
"else" {
  return TOKEN_ELSE;
}
"lambda" {
  return TOKEN_LAMBDA;
}
"fun" { 
  return TOKEN_FUN;
}
{COMMA} {
  SET_LEXEME(yytext);
  return TOKEN_COMMA;
}
"in" {
  return TOKEN_IN;
}
"readString" {
   return TOKEN_READSTRING;
}

{LETTER}+({INT}*{LETTER}*)* {
  SET_LEXEME(yytext);
  return TOKEN_IDENTIFIER; 
}

. {
  /* Leave this rule at the end of our lexer to "eat up" all illegal strings */
  SET_LEXEME("Unexpected character in input");
  return TOKEN_ERROR;
}

