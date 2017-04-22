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
  extern int node_lineno;

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
    exp_node *expnode;
  char *error;
  std::list<RWL::Expression> *stmts;
  RWL::Expression st;
  RWL::Expressions params;
  RWL::pgm *prog;
  RWL::function_node *function;
}

%error-verbose

%token CLASS 258 ELSE 259 FI 260 IF 261 IN 262 INHERITS 263 LET 264 LOOP 265 POOL 266 THEN 267 WHILE 268
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
%token <symbol> TYPE_DECL 293
%token DEF 294

%type <expnode> exp
%type <expnode> formal
%type <params> formals
%type <params> explist
%type <params> explist_params
%type <prog> program




%locations

%%


program : explist { $$ = new pgm($1); root = $$; }
;

explist : exp  { $$ = single_Expressions($1); }
         | explist exp
            { $$ = append_Expressions($1, single_Expressions($2)); }
            | explist error
                 { // just copy up the stmtlist when an error occurs
                         $$ = $1;
                         yyclearin; }
         | { $$ = nil_Expressions(); }  /* empty string */
;

explist_params :
          explist_params ',' exp
            { $$ = append_Expressions($1, single_Expressions($3)); }

         | exp { $$ = single_Expressions($1); }
         | { $$ = nil_Expressions(); }
;


formal :
    TYPE_DECL WORD {
        $$ = new formal_node($1, $2);
    }
    ;

formals :

    formals ',' formal { $$ = append_Expressions($1, single_Expressions($3)); }

                                | formal { $$ = single_Expressions($1); }
                                | { $$ = nil_Expressions(); }
;







  exp:

  PRINT exp {
      $$ = new print_stmt($2);
      std::cout << "PRINT WORD" << std::endl;
         }


         | '{' explist '}' {
            $$ = block($2);

         }

         | IF '(' exp ')' THEN exp ELSE exp FI
             { $$ = cond($3, $6, $8); }
             | WHILE '(' exp ')' LOOP exp POOL
             { $$ = loop($3, $6); }




         | WORD ASSIGN exp

          {
          std::cout << "descending!!!" << std::endl;
          $$ = new assignment_stmt($1, $3);
          }

          |

          TYPE_DECL WORD ASSIGN exp {
          std::cout << "found variable definition!!!" << std::endl;
          $$ = new declaration_node($1, $2, $4);
          }
          |

          DEF TYPE_DECL WORD '(' formals ')' '{' exp '}' {
                std::cout << red << "found function declaration!!!" << norm << std::endl;
                $$ = new function_node($2, $3, $5, $8);
          }
          |

  INTEGER_CONST {
      std::cout << "INTEGER expression detected: "; $1->print(std::cout); std::cout << std::endl;
      $$ = new integer_node($1);
    }

    |

    WORD '(' explist_params ')' {

    std::cout << "function expression detected: "; $1->print(std::cout); std::cout << std::endl;

            $$ = new dispatch_node($1, $3);
            std::cout << "function expression detection complete " << std::endl;
    }



    |

    WORD {
        std::cout << "WORD expression detected: "; $1->print(std::cout); std::cout << std::endl;

        $$ = new id_node($1);
        std::cout << "WORD expression detection complete " << std::endl;
    }

    |

    STRING {
        std::cout << "STRING expression detected: "; $1->print(std::cout); std::cout << std::endl;

        $$ = new string_node($1);
        std::cout << "STRING expression detection complete " << std::endl;
    }




;



%%


void 
RWL::RWL_Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l  << "counted line number = " << std::endl;
}
