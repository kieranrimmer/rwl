
    // An example of using the flex C++ scanner class.

%{

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#include <iostream.h>
#endif

using namespace std;

int mylineno = 0;
%}

%option debug
%option nodefault
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
ASS_OP =
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

%%

{ws}    /* skip blanks and tabs */

"/*"    {
        int c;

        while((c = yyinput()) != 0)
            {
            if(c == '\n')
                ++mylineno;

            else if(c == '*')
                {
                if((c = yyinput()) == '/')
                    break;
                else
                    unput(c);
                }
            }
        }

{ELSE}   cout << "else ";
{IF}   cout << "if ";
{NEW}   cout << "new ";
{DELETE}   cout << "delete ";

{number}  cout << "number " << YYText();

\n        mylineno++; cout << '\n';

{name}    cout << "name " << YYText();

{string}  cout << "string " << YYText();



%%


int main( int /* argc */, char** /* argv */ )
    {
    FlexLexer* lexer = new yyFlexLexer;
    while(lexer->yylex() != 0)
        ;
    return 0;
    }
