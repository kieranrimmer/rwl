%{
/* C++ string header, for string ops below */
#include <string>

/* Implementation of yyFlexScanner */ 
#include "rwl_scanner.hpp"
#include "string_table/string_table.hpp"
#undef  YY_DECL
#define YY_DECL int RWL::RWL_Scanner::yylex( RWL::RWL_Parser::semantic_type * const lval, RWL::RWL_Parser::location_type *loc )

/* typedef to make the returns for the tokens shorter */
using token = RWL::RWL_Parser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option debug
%option nodefault
%option yyclass="RWL::RWL_Scanner"
%option noyywrap
%option c++

IF  (?i:if)
CLASS  (?i:class)
INHERITS (?i:inherits)
NEW (?i:new)
DELETE (?i:delete)
IMPORT (i?:import)
SC ";"

GT >
GTE >=
LT <
LTE <=
EQ ==
assign =
INC_OP ++
DEC_OP --




string  \"[^\n"]+\"

ws      [ \t]+




alpha   [A-Za-z]
dig     [0-9]
name    ({alpha}|{dig}|\$)({alpha}|{dig}|[_.\-/$])*
num1    [-+]?{dig}+\.?([eE][-+]?{dig}+)?
num2    [-+]?{dig}*\.{dig}+([eE][-+]?{dig}+)?
number  {num1}|{num2}
ELSE    (?i:else)
print (?i:print)


%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

{print}     {


                return(token::PRINT);
            }

{assign} {
  return(token::ASSIGN);
}

{string}    {
  yylval->string_const = strdup(yytext);
  return(token::STRING);
}

[a-z]       {
               return( token::LOWER );
            }

[A-Z]       {
               return( token::UPPER );
            }



{name}      {
                yylval->symbol = stringtable.add_string(yytext);
                return( token::WORD );
            }

[a-zA-Z]+   {
               /**
                * Section 10.1.5.1 of the 3.0.2 Bison Manual says the 
                * following should work:
                * yylval.build( yytext );
                * but it doesn't.
                * ref: http://goo.gl/KLn0w2
                */
               yylval->id = strdup(yytext);;
               return( token::WORD );
            }

{ws}   {}

\n          {
               // Update line number
               loc->lines();
               return( token::NEWLINE );
            }

%%


