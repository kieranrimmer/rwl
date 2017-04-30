# RWL

## What is RWL?
 
RWL (Real World Language) is a simple scripting language.  This project is an RWL compiler.

It is AOT compiled and statically typed.  There are three types:
1. integer
2. string
3. boolean

The Lexer and Parser are implemented using C++ through
 FLEX/BISON.
 
Semantic analysis and error recovery are implemented in C++.

The backend will initially utilise llvm (http://llvm.org/docs/tutorial/index.html).


The starting points for this project are:

1.  the excellent work of Jonathan Beard:
https://github.com/jonathan-beard/simple_wc_example.git
2.  the COOL programming language as taught in Stanford's online "CS1 Compilers"
https://lagunita.stanford.edu/courses/Engineering/Compilers/Fall2014

## Current status

The Lexer, Parser, AST, String Tables (one each for IDs, strings and integers), symbol tables (on for variables, one for functions) are all implemented.


### Lexer

#### TODO:

- reformat

### Parser

#### TODO:

- remove shift-reduce conflicts
- reformat

### AST

http://web.eecs.utk.edu/~bvz/teaching/cs461Sp11/notes/parse_tree/

#### LLVM integration

Sorta scaffolded.

http://llvm.org/docs/tutorial/index.html

http://www.aosabook.org/en/llvm.html

http://gnuu.org/2009/09/18/writing-your-own-toy-compiler/4/

#### TODO:

- optimisation
- basic code generation for X86_64

### CMAKE integration

#### TODO:

- get it working

https://cmake.org/pipermail/cmake/2002-September/003028.html

## Copyright

Copyright acknowledgements are provided in uc_copyright.h

## Contact

kieranrimmer@gmail.com