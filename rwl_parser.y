%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {RWL}
%define parser_class_name {RWL_Parser}

%code requires{
   namespace RWL {
      class RWL_Driver;
      class RWL_Scanner;
   }

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}

%parse-param { RWL_Scanner  &scanner  }
%parse-param { RWL_Driver  &driver  }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   
   /* include for all driver functions */
   #include "rwl_driver.hpp"
   #include "AST/exp.hpp"

#undef yylex
#define yylex scanner.yylex
}



%union {
  float num;
  char *id;
  exp_node *expnode;
  list<statement *> *stmts;
  statement *st;
  pgm *prog;
}

%token CLASS 258 ELSE 259 FI 260 IF 261 IN 262 
    %token INHERITS 263 LET 264 LOOP 265 POOL 266 THEN 267 WHILE 268
    %token CASE 269 ESAC 270 OF 271 DARROW 272 NEW 273 ISVOID 274
    %token ASSIGN 280 NOT 281 LE 282 ERROR 283

%token               END    0     "end of file"
%token               UPPER  284
%token               LOWER 285
%token <std::string> WORD 286
%token               NEWLINE 287
%token               CHAR 288

%locations

%%


program : stmtlist { $$ = new pgm($1); root = $$; }
;

stmtlist : stmtlist NEWLINE    /* empty line */
     { // just copy up the stmtlist when a blank line occurs
             $$ = $1;
           }
         | stmtlist stmt NEWLINE
            { // copy up the list and add the stmt to it
              $$ = $1;
              $1->push_back($2);
            }
         | stmtlist error NEWLINE
     { // just copy up the stmtlist when an error occurs
             $$ = $1;
             yyclearin; } 
         |  
           { $$ = new list<statement *>(); }  /* empty string */
;

stmt: ID EQUALS exp { 
  $$ = new assignment_stmt($1, $3);
     }
       
| PRINT ID {
  $$ = new print_stmt($2);
 }

 ;

exp:  MINUS exp %prec UMINUS {
  $$ = new unary_minus_node($2); }

  | exp PLUS exp {
    $$ = new plus_node($1, $3); }

  | exp MINUS exp {
    $$ = new minus_node($1, $3); }

  | exp TIMES exp {
    $$ = new times_node($1, $3); }

  | LPAREN exp RPAREN  {
          $$ = $2; }

  | NUMBER {
    $$ = new number_node($1); }

|       ID {
  $$ = new id_node($1); }
;



%%


void 
RWL::RWL_Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
