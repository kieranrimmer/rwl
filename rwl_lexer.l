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
DEF def

type_declarator (int|string|boolean|void)

GT >
GTE >=
LT <
LTE <=
EQ ==
assign =
INC_OP ++
DEC_OP --

cpp_comment ("\/\/".*)
sql_comment ("-- ".*)

string  \"[^\n"]+\"

ws      [ \t]+

TRUE    t(?i:rue)

FALSE   f(?i:alse)




alpha   [A-Za-z]
dig     [0-9]
integer [0-9]+
name    ({alpha}|{dig}|\$)({alpha}|{dig}|[_.\-/$])*
disallowed_name {name}([aeiou]|os)
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

{sql_comment}  {  }


{cpp_comment}  {  }

{DEF} {
    return (token::DEF);
}

{type_declarator} {
                    std::cout << "detected type declarator: " << yytext << std::endl;
                    yylval->symbol = idtable.add_string(yytext);
                    return (token::TYPE_DECL);
                  }

{integer}    {
  std::cout << "detected integer: " << yytext << std::endl;
  yylval->symbol = inttable.add_string(yytext);
  return (token::INTEGER_CONST);
}

{string}    {
  yylval->symbol = stringtable.add_string(yytext);
  return(token::STRING);
}

{TRUE} {
  yylval->boolean = 1;
  return token::BOOL_CONST;
}

{FALSE} {
  yylval->boolean = 0;
  return token::BOOL_CONST;
}



[a-z]       {
               return( token::LOWER );
            }

[A-Z]       {
               return( token::UPPER );
            }



{IF} { return (token::IF); }

{disallowed_name} { yylval->error = strdup(yytext); std::cout << "Disallowed name: " << yytext << "!!!" <<std::endl; return (token::ERROR); }

{name}      {
                yylval->symbol = idtable.add_string(yytext);
                return( token::WORD );
            }

"."	{ return (int)'.'; }
";"	{ return (int)';'; }
","	{ return (int)','; }
")"	{ return (int)')'; }
"("	{ return (int)'('; }
"}"	{ return (int)'}'; }
"{"	{ return (int)'{'; }
"<"	{ return (int)'<'; }
":"	{ return (int)':'; }
"+"	{ return (int)'+'; }
"-"	{ return (int)'-'; }
"*"	{ return (int)'*'; }
"/"	{ return (int)'/'; }
"~"	{ return (int)'~'; }
"@"	{ return (int)'@'; }


{ws}   {}

\n          {
               // Update line number
               loc->lines();
               return( token::NEWLINE );
            }

.    { yylval->error = strdup(yytext); return (token::ERROR); }

%%


