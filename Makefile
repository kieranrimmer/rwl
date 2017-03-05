CC    ?= clang
CXX   ?= clang++

EXE = rwl_phaseone

CDEBUG = -g -Wall

CXXDEBUG = -g -Wall

CSTD = -std=c99
CXXSTD = -std=c++14

CFLAGS = -Wno-deprecated-register -O0  $(CDEBUG) $(CSTD) 
CXXFLAGS = -Wno-deprecated-register -O0  $(CXXDEBUG) $(CXXSTD)


CPPOBJ = main mc_driver
SOBJ =  parser lexer

FILES = $(addsuffix .cpp, $(CPPOBJ))

OBJS  = $(addsuffix .o, $(CPPOBJ))

CLEANLIST =  rwl_lexer.yy.cc rwl_lexer $(EXE)


.PHONY: all
all: lexer

lexer: rwl_cpp.l
	flex --outfile=rwl_lexer.yy.cc  $<
	$(CXX)  $(CXXFLAGS) rwl_lexer.yy.cc -o rwl_lexer


.PHONY: test
test:
	echo "22 33l if IF ELSE 44 btrne_77" | ./rwl_lexer

.PHONY: clean
clean:
	rm -rf $(CLEANLIST)

