#ifndef __RWLSCANNER_HPP__
#define __RWLSCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "rwl_parser.tab.hh"
#include "location.hh"

namespace RWL{

class RWL_Scanner : public yyFlexLexer{
public:
   
   RWL_Scanner(std::istream *in) : yyFlexLexer(in)
   {
      loc = new RWL::RWL_Parser::location_type();
   };
   virtual ~RWL_Scanner() {
      delete loc;
   };

   //get rid of override virtual function warning
   using FlexLexer::yylex;

   virtual
   int yylex( RWL::RWL_Parser::semantic_type * const lval, 
              RWL::RWL_Parser::location_type *location );
   // YY_DECL defined in mc_lexer.l
   // Method body created by flex in mc_lexer.yy.cc


private:
   /* yyval ptr */
   RWL::RWL_Parser::semantic_type *yylval = nullptr;
   /* location ptr */
   RWL::RWL_Parser::location_type *loc    = nullptr;
};

} /* end namespace RWL */

#endif /* END __RWLSCANNER_HPP__ */
