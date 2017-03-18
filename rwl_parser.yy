%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {RWL}
%define parser_class_name {RWL_Parser}

%code requires{
  #include "AST/tree.hpp"
  #include "string_table/string_table.hpp"
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
   
   


// the root of the abstract syntax tree
 RWL::pgm *RWL::root;

// for keeping track of line numbers in the program we are parsing
  int line_num = 1;

#undef yylex
#define yylex scanner.yylex
}

%start program




%union {
  char *num;
  int boolean;
  Symbol symbol;
  char *id;
  char *string_const;
  exp_node<int> *expnode;
  char *error;
  std::list<RWL::statement *> *stmts;
  RWL::statement *st;
  RWL::pgm *prog;
}

%error-verbose

%token CLASS 258 ELSE 259 FI 260 IF 261 IN 262 
    %token INHERITS 263 LET 264 LOOP 265 POOL 266 THEN 267 WHILE 268
    %token CASE 269 ESAC 270 OF 271 DARROW 272 NEW 273 ISVOID 274
    %token ASSIGN 280 NOT 281 LE 282 ERROR 283

%token               END    0     "end of file"
%token               UPPER  284
%token               LOWER 285
%token <symbol> WORD 286
%token               NEWLINE 287
%token               CHAR 288
%token  PRINT 289
%token <symbol> STRING 290
%token <symbol> INTEGER_CONST 291
%token <boolean> BOOL_CONST 292

%type <expnode> exp 
%type <stmts> stmtlist
%type <st> stmt
%type <prog> program

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
              std::cout << "statement detected: "; $2->print(); std::cout << std::endl;
              $1->push_back($2);
            }
         | stmtlist error NEWLINE
     { // just copy up the stmtlist when an error occurs
             $$ = $1;              
             yyclearin; } 
         |  
           { $$ = new std::list<statement *>(); }  /* empty string */
;

stmt: 




  PRINT WORD { 
    $$ = new print_stmt($2->get_string());
    std::cout << "PRINT WORD" << std::endl;
       }

       |

       PRINT INTEGER_CONST { 
    $$ = new print_stmt($2->get_string());
    std::cout << "PRINT INTEGER" << std::endl;
       }

   
       | WORD ASSIGN exp

        {
        std::cout << "descending!!!" << std::endl;
        $$ = new assignment_stmt($1, $3);
        }

        | exp {std::cout << "expression detected: "; std::cout << std::endl;}
         


   ;

  exp:  INTEGER_CONST {
      std::cout << "INTEGER expression detected: "; $1->print(std::cout); std::cout << std::endl;
      $$ = new integer_node($1);
    }

    |

   WORD {
    std::cout << "WORD expression detected: "; $1->print(std::cout); std::cout << std::endl;
    $$ = new id_node($1); }

    |

    STRING {
    std::cout << "STRING expression detected: "; $1->print(std::cout); std::cout << std::endl;
    $$ = new string_node($1);
    }
 
;



%%


void 
RWL::RWL_Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
