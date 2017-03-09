CC    ?= clang
CXX   ?= clang++

EXE = rwl_phaseone

CDEBUG = -g -Wall

CXXDEBUG = -g -Wall

CSTD = -std=c99
CXXSTD = -std=c++14

CFLAGS = -Wno-deprecated-register -O0  $(CDEBUG) $(CSTD) 
CXXFLAGS = -Wno-deprecated-register -O0  $(CXXDEBUG) $(CXXSTD)


CPPOBJ = main rwl_driver
SOBJ =  parser lexer

FILES = $(addsuffix .cpp, $(CPPOBJ))

OBJS  = $(addsuffix .o, $(CPPOBJ))

CLEANLIST =  $(addsuffix .o, $(OBJ)) $(OBJS) \
				 rwl_parser.tab.cc rwl_parser.tab.hh \
				 location.hh position.hh \
			    stack.hh rwl_parser.output parser.o \
				 lexer.o rwl_lexer.yy.cc $(EXE)\


# CLEANLIST =  rwl_lexer.yy.cc rwl_lexer $(EXE)

.PHONY: all
all: rwl

rwl: $(FILES)
	$(MAKE) $(SOBJ)
	$(MAKE) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS) parser.o lexer.o $(LIBS)


# lexer: rwl_cpp.l
# 	flex --outfile=rwl_lexer.yy.cc  $<
# 	$(CXX)  $(CXXFLAGS) rwl_lexer.yy.cc -o rwl_lexer

parser: rwl_parser.yy
	bison -d -v rwl_parser.yy
	$(CXX) $(CXXFLAGS) -c -o parser.o rwl_parser.tab.cc

lexer: rwl_lexer.l
	flex --outfile=rwl_lexer.yy.cc  $<
	$(CXX)  $(CXXFLAGS) -c rwl_lexer.yy.cc -o lexer.o


.PHONY: test
test:
	echo "22 33l if IF ELSE 44 btrne_77" | ./rwl_lexer

.PHONY: clean
clean:
	rm -rf $(CLEANLIST)


# CCC = g++
# CCFLAGS= -O2
# LEX = flex
# LFLAGS= -8     
# YACC= bison 
# YFLAGS= -d -v -t -y

# RM = /bin/rm -f

# exp: y.tab.o lex.yy.o exp.o
# 	${CCC} ${CCFLAGS} lex.yy.o y.tab.o exp.o -o exp -lfl

# exp.o: exp.cpp exp.h
# 	${CCC} -c exp.cpp
# y.tab.o: exp.yacc
# 	${YACC} ${YFLAGS} exp.yacc
# 	${CCC} ${CCFLAGS} y.tab.c -c 

# lex.yy.o: exp.lex
# 	${LEX} $(LFLAGS) exp.lex
# 	${CCC} ${CCFLAGS} lex.yy.c -c 

# clean:
# 	/bin/rm -f lex.yy.* y.tab.* *.o exp

