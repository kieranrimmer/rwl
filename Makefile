CC    ?= clang
CXX   ?= clang++

EXE = rwl_phaseone

CDEBUG = -g -Wall

CXXDEBUG = -g -Wall

CSTD = -std=c99
CXXSTD = -std=c++14


LLVM_COMPILE_ONLY = `llvm-config --cppflags --ldflags`

LLVM_FLAGS = `llvm-config --cppflags --ldflags --system-libs --libs core`
LLVM_FLAGS_OBJECT = `llvm-config --cppflags --ldflags --system-libs --libs all`

CFLAGS = -Wno-deprecated-register -O0 $(CDEBUG) $(CSTD)
CXXFLAGS = -Wno-deprecated-register -O0  $(CXXDEBUG) $(CXXSTD) $(LLVM_FLAGS_OBJECT)
CXXFLAGS_COMPILE_ONLY = -Wno-deprecated-register -O0  $(CXXDEBUG) $(CXXSTD) $(LLVM_COMPILE_ONLY)


CPPOBJ = main rwl_driver
SOBJ =  parser lexer

FILES = $(addsuffix .cpp, $(CPPOBJ))

OBJS  = $(addsuffix .o, $(CPPOBJ))

CLEANLIST =  $(addsuffix .o, $(OBJ)) $(OBJS) \
				 rwl_parser.tab.cc rwl_parser.tab.hh \
				 location.hh position.hh \
			    stack.hh rwl_parser.output parser.o \
			    util.o tree.o string_table.o \
			    main.o rwl_driver.o \
			    codegen_llvm.o \
			    semant.o \
			    main.o driver.o \
				 lexer.o rwl_lexer.yy.cc $(EXE)\

.PHONY: all
all: rwl

rwl: $(FILES)
	$(MAKE) $(SOBJ)
	$(MAKE) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS)  util.o string_table.o parser.o tree.o codegen_llvm.o semant.o lexer.o $(LIBS)

parser: rwl_parser.yy
	${CXX} -c string_table/string_table.cpp -o string_table.o
	${CXX} -c util/util.cpp -o util.o
	${CXX} $(CXXFLAGS_COMPILE_ONLY) -c AST/tree.cpp -o tree.o
	${CXX} $(CXXFLAGS_COMPILE_ONLY) -c semant.cpp -o semant.o
	${CXX} $(CXXFLAGS_COMPILE_ONLY) -c codegen_llvm/codegen_llvm.cpp -o codegen_llvm.o
	${CXX} $(CXXFLAGS_COMPILE_ONLY) -c AST/tree.cpp -o tree.o
	bison -d -v rwl_parser.yy
	$(CXX) $(CXXFLAGS_COMPILE_ONLY) -c -o parser.o rwl_parser.tab.cc

lexer: rwl_lexer.l
	flex --outfile=rwl_lexer.yy.cc  $<
	$(CXX)  $(CXXFLAGS_COMPILE_ONLY) -c rwl_lexer.yy.cc -o lexer.o


.PHONY: test
test:
	echo "22 33l if IF ELSE 44 btrne_77" | ./rwl_lexer

.PHONY: clean
clean:
	rm -rf $(CLEANLIST)



