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

The Lexer, Parser, AST, String Tables 
(one each for IDs, strings and integers), 
symbol tables (on for variables, one for functions) 
are all implemented.

## Setup

You will need to add the include paths for clang / llvm 5.0

This can be done by amending the CPATH shell variable.

You can use the provided script:
```$xslt
source ./path_setup.sh <llvm include dir> <build include dir>
```

In my case:
```$xslt
source ./path_setup.sh /Users/kieranrimmer/packages/llvm/include /Users/kieranrimmer/packages/build/include
```

To check which paths the clang++ is including:

```$xslt
echo | clang++  -Wp,-v -stdlib=libc++ -x c++ - -fsyntax-only
```

### Lexer

### Parser

#### TODO:

- remove shift-reduce conflicts

### AST

http://web.eecs.utk.edu/~bvz/teaching/cs461Sp11/notes/parse_tree/

### Semantic analysis

The language is rather simple, pretty much everything is an expression.
Some expressions return a trivial type.

#### TODO:

- complete implementation of various semant() functions.
- implement and tune error recovery.  I'm thinking here we
go with casting all non-assignables and erroneous assignments
to "no_type", a special type symbol.

### LLVM integration

llvm version 5.0.0

Sorta scaffolded.

http://llvm.org/docs/tutorial/index.html

http://www.aosabook.org/en/llvm.html

http://gnuu.org/2009/09/18/writing-your-own-toy-compiler/4/

#### TODO:

- generate IR code DONE
- optimisation DONE
- basic code generation for X86_64 DONE

### CMAKE integration

#### TODO:

- get it working

https://cmake.org/pipermail/cmake/2002-September/003028.html

it kinda does work now...

```$xslt
make clean && make
./rwl_phaseone ./test/TREE_PROGRAM.rwl 
clang output.o -o d1
./d1
```

...or if you're feeling lazy...

```
make clean && make && make test

```

the output demonstates that we can link against RWL code :)

## Manually calling the linker

### on Mac OS X

```$xslt
ld -demangle -lto_library /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/libLTO.dylib -dynamic -arch x86_64 -macosx_version_min 10.12.0 -syslibroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk -o d2 output.o -lSystem /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/../lib/clang/8.1.0/lib/darwin/libclang_rt.osx.a
```


### Documentation

- doxygen?
- readthedocs?
- HLDD?


## Copyright

Copyright acknowledgements are provided in uc_copyright.h

## Contact

kieranrimmer@gmail.com