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

#undef yylex
#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert


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

list_option : END | list END;

list
  : item
  | list item
  ;

item
  : UPPER   { driver.add_upper(); }
  | LOWER   { driver.add_lower(); }
  | WORD    { driver.add_word( $1 ); }
  | NEWLINE { driver.add_newline(); }
  | CHAR    { driver.add_char(); }
  ;

%%


void 
RWL::RWL_Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
