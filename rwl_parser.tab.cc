// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "rwl_parser.tab.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "rwl_parser.tab.hh"

// User implementation prologue.

#line 51 "rwl_parser.tab.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 31 "rwl_parser.yy" // lalr1.cc:413

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

#line 72 "rwl_parser.tab.cc" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 5 "rwl_parser.yy" // lalr1.cc:479
namespace  RWL  {
#line 158 "rwl_parser.tab.cc" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
   RWL_Parser ::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
   RWL_Parser :: RWL_Parser  (RWL_Scanner  &scanner_yyarg, RWL_Driver  &driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

   RWL_Parser ::~ RWL_Parser  ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
   RWL_Parser ::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
   RWL_Parser ::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
   RWL_Parser ::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    value = other.value;
  }


  template <typename Base>
  inline
   RWL_Parser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
   RWL_Parser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  inline
   RWL_Parser ::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
   RWL_Parser ::basic_symbol<Base>::clear ()
  {
    Base::clear ();
  }

  template <typename Base>
  inline
  bool
   RWL_Parser ::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
   RWL_Parser ::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  inline
   RWL_Parser ::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
   RWL_Parser ::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
   RWL_Parser ::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
   RWL_Parser ::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
   RWL_Parser ::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
   RWL_Parser ::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
   RWL_Parser ::by_state::by_state ()
    : state (empty_state)
  {}

  inline
   RWL_Parser ::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
   RWL_Parser ::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
   RWL_Parser ::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
   RWL_Parser ::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
   RWL_Parser ::symbol_number_type
   RWL_Parser ::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
   RWL_Parser ::stack_symbol_type::stack_symbol_type ()
  {}


  inline
   RWL_Parser ::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
    value = that.value;
    // that is emptied.
    that.type = empty_symbol;
  }

  inline
   RWL_Parser ::stack_symbol_type&
   RWL_Parser ::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
   RWL_Parser ::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
   RWL_Parser ::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
   RWL_Parser ::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
   RWL_Parser ::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
   RWL_Parser ::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
   RWL_Parser ::debug_stream () const
  {
    return *yycdebug_;
  }

  void
   RWL_Parser ::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


   RWL_Parser ::debug_level_type
   RWL_Parser ::debug_level () const
  {
    return yydebug_;
  }

  void
   RWL_Parser ::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline  RWL_Parser ::state_type
   RWL_Parser ::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
   RWL_Parser ::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
   RWL_Parser ::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
   RWL_Parser ::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 105 "rwl_parser.yy" // lalr1.cc:859
    {
            (yylhs.value.prog) = new pgm((yystack_[0].value.params));
            root = (yylhs.value.prog);
        }
#line 642 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 3:
#line 114 "rwl_parser.yy" // lalr1.cc:859
    {
            (yylhs.value.params) = single_Expressions((yystack_[0].value.expnode));
        }
#line 650 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 120 "rwl_parser.yy" // lalr1.cc:859
    {
            (yylhs.value.params) = append_Expressions((yystack_[1].value.params), single_Expressions((yystack_[0].value.expnode)));
        }
#line 658 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 126 "rwl_parser.yy" // lalr1.cc:859
    {
            // just copy up the stmtlist when an error occurs
            (yylhs.value.params) = (yystack_[1].value.params);
            yyclearin;
        }
#line 668 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 135 "rwl_parser.yy" // lalr1.cc:859
    {
            (yylhs.value.params) = nil_Expressions();
        }
#line 676 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 143 "rwl_parser.yy" // lalr1.cc:859
    {
            (yylhs.value.params) = append_Expressions((yystack_[2].value.params), single_Expressions((yystack_[0].value.expnode)));
        }
#line 684 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 149 "rwl_parser.yy" // lalr1.cc:859
    {
            (yylhs.value.params) = single_Expressions((yystack_[0].value.expnode));
        }
#line 692 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 156 "rwl_parser.yy" // lalr1.cc:859
    {
            (yylhs.value.params) = nil_Expressions();
        }
#line 700 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 163 "rwl_parser.yy" // lalr1.cc:859
    {
            (yylhs.value.formal_) = new formal_node((yystack_[1].value.symbol), (yystack_[0].value.symbol));
        }
#line 708 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 170 "rwl_parser.yy" // lalr1.cc:859
    {
            (yylhs.value.formals_) = append_Formals((yystack_[2].value.formals_), single_Formals((yystack_[0].value.formal_)));
        }
#line 716 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 176 "rwl_parser.yy" // lalr1.cc:859
    {
            (yylhs.value.formals_) = single_Formals((yystack_[0].value.formal_));
        }
#line 724 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 182 "rwl_parser.yy" // lalr1.cc:859
    {
            (yylhs.value.formals_) = nil_Formals();
        }
#line 732 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 189 "rwl_parser.yy" // lalr1.cc:859
    {
            (yylhs.value.expnode) = new print_stmt((yystack_[0].value.expnode));
            std::cout << "PRINT WORD" << std::endl;
        }
#line 741 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 196 "rwl_parser.yy" // lalr1.cc:859
    {
            (yylhs.value.expnode) = block((yystack_[1].value.params));
        }
#line 749 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 202 "rwl_parser.yy" // lalr1.cc:859
    {
            (yylhs.value.expnode) = cond((yystack_[6].value.expnode), (yystack_[3].value.expnode), (yystack_[1].value.expnode));
        }
#line 757 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 208 "rwl_parser.yy" // lalr1.cc:859
    {
            (yylhs.value.expnode) = loop((yystack_[4].value.expnode), (yystack_[1].value.expnode));
        }
#line 765 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 214 "rwl_parser.yy" // lalr1.cc:859
    {
            std::cout << "descending!!!" << std::endl;
            (yylhs.value.expnode) = new assignment_stmt((yystack_[2].value.symbol), (yystack_[0].value.expnode));
        }
#line 774 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 221 "rwl_parser.yy" // lalr1.cc:859
    {
            std::cout << "found variable definition!!!" << std::endl;
            (yylhs.value.expnode) = new declaration_node((yystack_[3].value.symbol), (yystack_[2].value.symbol), (yystack_[0].value.expnode));
        }
#line 783 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 228 "rwl_parser.yy" // lalr1.cc:859
    {
            std::cout << red << "found function declaration!!!" << norm << std::endl;
            (yylhs.value.expnode) = new function_node((yystack_[7].value.symbol), (yystack_[6].value.symbol), (yystack_[4].value.formals_), (yystack_[1].value.params));
        }
#line 792 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 235 "rwl_parser.yy" // lalr1.cc:859
    {
            std::cout << "INTEGER expression detected: "; (yystack_[0].value.symbol)->print(std::cout); std::cout << std::endl;
            (yylhs.value.expnode) = new integer_node((yystack_[0].value.symbol));
        }
#line 801 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 242 "rwl_parser.yy" // lalr1.cc:859
    {
            std::cout << "function expression detected: "; (yystack_[3].value.symbol)->print(std::cout); std::cout << std::endl;
            (yylhs.value.expnode) = new dispatch_node((yystack_[3].value.symbol), (yystack_[1].value.params));
            std::cout << "function expression detection complete " << std::endl;
        }
#line 811 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 250 "rwl_parser.yy" // lalr1.cc:859
    {
            std::cout << "WORD expression detected: "; (yystack_[0].value.symbol)->print(std::cout); std::cout << std::endl;
            (yylhs.value.expnode) = new id_node((yystack_[0].value.symbol));
            std::cout << "WORD expression detection complete " << std::endl;
        }
#line 821 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 258 "rwl_parser.yy" // lalr1.cc:859
    {
            std::cout << "STRING expression detected: "; (yystack_[0].value.symbol)->print(std::cout); std::cout << std::endl;
            (yylhs.value.expnode) = new string_node((yystack_[0].value.symbol));
            std::cout << "STRING expression detection complete " << std::endl;
        }
#line 831 "rwl_parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 266 "rwl_parser.yy" // lalr1.cc:859
    {
            std::cout << "plus expression detected: "; (yystack_[2].value.expnode)->print(); std::cout << " + "; (yystack_[0].value.expnode)->print();  std::cout << std::endl;
            (yylhs.value.expnode) = new plus_node((yystack_[2].value.expnode), (yystack_[0].value.expnode));
            std::cout << "STRING expression detection complete " << std::endl;
        }
#line 841 "rwl_parser.tab.cc" // lalr1.cc:859
    break;


#line 845 "rwl_parser.tab.cc" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
   RWL_Parser ::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
   RWL_Parser ::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char  RWL_Parser ::yypact_ninf_ = -32;

  const signed char  RWL_Parser ::yytable_ninf_ = -3;

  const signed char
   RWL_Parser ::yypact_[] =
  {
      67,   -16,   -15,   -17,    67,   -32,   -32,    -1,    -9,    67,
      29,    27,   -10,    67,    67,    67,    67,   -10,    11,     9,
      38,   -32,   -32,   -10,    67,   -25,   -21,   -10,   -31,   -10,
      67,     4,   -32,   -10,    31,    35,    67,   -32,   -10,    15,
      67,    67,   -10,    23,   -32,   -22,    -2,    -6,   -32,    15,
      14,    67,   -32,   -32,    67,     1,    53,   -32,   -32
  };

  const unsigned char
   RWL_Parser ::yydefact_[] =
  {
       6,     0,     0,    23,     0,    24,    21,     0,     0,     6,
       0,     0,     3,     0,     0,     0,     9,    14,     0,     0,
       0,     1,     5,     4,     0,     0,     0,    18,     0,     8,
       0,     0,    15,    25,     0,     0,     0,    22,    19,    13,
       0,     0,     7,     0,    12,     0,     0,     0,    10,     0,
       0,     0,    17,    11,     6,     0,     0,    16,    20
  };

  const signed char
   RWL_Parser ::yypgoto_[] =
  {
     -32,   -32,    -8,   -32,     6,   -32,    -4
  };

  const signed char
   RWL_Parser ::yydefgoto_[] =
  {
      -1,    10,    11,    28,    44,    45,    12
  };

  const signed char
   RWL_Parser ::yytable_[] =
  {
      17,    20,    51,    15,    36,    52,    57,    23,    37,    25,
      26,    27,    29,    49,    34,    24,    23,    50,    35,    24,
      33,    16,    13,    14,    19,    18,    38,    -2,    22,    21,
      24,    30,    42,     1,    24,    31,    46,    47,    24,    22,
       2,    24,    39,    40,     1,    41,    56,    55,    43,    48,
      54,     2,    23,     3,    22,    53,     4,     5,     6,     1,
       7,     8,     0,     9,     3,     0,     2,     4,     5,     6,
       0,     7,     8,     1,     9,    32,     0,     0,     0,     3,
       2,     0,     4,     5,     6,     0,     7,     8,     0,     9,
      58,     0,     0,     3,     0,     0,     4,     5,     6,     0,
       7,     8,     0,     9
  };

  const signed char
   RWL_Parser ::yycheck_[] =
  {
       4,     9,     4,    20,    35,    11,     5,    11,    39,    13,
      14,    15,    16,    35,    39,    40,    20,    39,    39,    40,
      24,    38,    38,    38,    33,    26,    30,     0,     1,     0,
      40,    20,    36,     6,    40,    26,    40,    41,    40,     1,
      13,    40,    38,    12,     6,    10,    54,    51,    33,    26,
      36,    13,    56,    26,     1,    49,    29,    30,    31,     6,
      33,    34,    -1,    36,    26,    -1,    13,    29,    30,    31,
      -1,    33,    34,     6,    36,    37,    -1,    -1,    -1,    26,
      13,    -1,    29,    30,    31,    -1,    33,    34,    -1,    36,
      37,    -1,    -1,    26,    -1,    -1,    29,    30,    31,    -1,
      33,    34,    -1,    36
  };

  const unsigned char
   RWL_Parser ::yystos_[] =
  {
       0,     6,    13,    26,    29,    30,    31,    33,    34,    36,
      42,    43,    47,    38,    38,    20,    38,    47,    26,    33,
      43,     0,     1,    47,    40,    47,    47,    47,    44,    47,
      20,    26,    37,    47,    39,    39,    35,    39,    47,    38,
      12,    10,    47,    33,    45,    46,    47,    47,    26,    35,
      39,     4,    11,    45,    36,    47,    43,     5,    37
  };

  const unsigned char
   RWL_Parser ::yyr1_[] =
  {
       0,    41,    42,    43,    43,    43,    43,    44,    44,    44,
      45,    46,    46,    46,    47,    47,    47,    47,    47,    47,
      47,    47,    47,    47,    47,    47
  };

  const unsigned char
   RWL_Parser ::yyr2_[] =
  {
       0,     2,     1,     1,     2,     2,     0,     3,     1,     0,
       2,     3,     1,     0,     2,     3,     9,     7,     3,     4,
       9,     1,     4,     1,     1,     3
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const  RWL_Parser ::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "CLASS", "ELSE", "FI", "IF",
  "IN", "INHERITS", "LET", "LOOP", "POOL", "THEN", "WHILE", "CASE", "ESAC",
  "OF", "DARROW", "NEW", "ISVOID", "ASSIGN", "NOT", "LE", "ERROR", "UPPER",
  "LOWER", "WORD", "NEWLINE", "CHAR", "PRINT", "STRING", "INTEGER_CONST",
  "BOOL_CONST", "TYPE_DECL", "DEF", "','", "'{'", "'}'", "'('", "')'",
  "'+'", "$accept", "program", "explist", "explist_params", "formal",
  "formals", "exp", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
   RWL_Parser ::yyrline_[] =
  {
       0,   105,   105,   114,   120,   126,   135,   143,   149,   156,
     163,   170,   176,   182,   189,   196,   202,   208,   214,   221,
     228,   235,   242,   250,   258,   266
  };

  // Print the state stack on the debug stream.
  void
   RWL_Parser ::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
   RWL_Parser ::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
   RWL_Parser ::token_number_type
   RWL_Parser ::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      38,    39,     2,    40,    35,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    36,     2,    37,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,     2,     2,     2,     2,     2,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,     2
    };
    const unsigned int user_token_number_max_ = 295;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 5 "rwl_parser.yy" // lalr1.cc:1167
} //  RWL 
#line 1304 "rwl_parser.tab.cc" // lalr1.cc:1167
#line 276 "rwl_parser.yy" // lalr1.cc:1168



    void RWL::RWL_Parser::error( const location_type &l, const std::string &err_message ) {
       std::cerr << "Error: " << err_message << " at " << l  << "counted line number = " << std::endl;
    }
