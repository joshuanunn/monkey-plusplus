# Monkey++

A cheeky simian-inspired interpreter and compiler for the [Monkey](https://monkeylang.org/) programming language, written in `C++`. These are based on the books [Writing An Interpreter In Go](https://interpreterbook.com/) and [Writing A Compiler In Go](https://compilerbook.com/), written by Thorsten Ball.

## Monkey Language

Monkey is a programming language defined in the books above, of which this repository includes two implementations: a tree-walking interpreter and a bytecode compiler and virtual machine. More information can be found at [https://monkeylang.org](https://monkeylang.org/).

Monkey defines types for integers, booleans, strings, arrays, hash maps and supports common arithmetic expressions:

```javascript
// Integers & arithemetic expressions
let version = 1 + (50 / 2) - (8 * 3);

// Strings
let name = "The Monkey programming language";

// Booleans
let isMonkeyFastNow = true;

// Arrays & hash maps
let people = [{"name": "Anna", "age": 24}, {"name": "Bob", "age": 99}];
```

It also includes first-class support for functions:

```javascript
// User-defined functions
let getName = fn(person) { person["name"]; } ;
getName(people[0]); // => "Anna"
getName(people[1]); // => "Bob"

// Built-in functions
puts(len(people))  // prints: 2
```

Support for conditionals, implicit and explicit returns and the use of recursive functions allows this:

```javascript
let fibonacci = fn(x) {
  if (x == 0) {
    0
  } else {
    if (x == 1) {
      return 1;
    } else {
      fibonacci(x - 1) + fibonacci(x - 2);
    }
  }
};
```

Monkey even supports closures:

```javascript
// Return a closure with free variables a and b
let newAdder = fn(a, b) {
  fn(c) { a + b + c };
};

// Construct a new 'adder' function
let adder = newAdder(1, 2);

adder(8); // => 11
```

## Building

All development was completed within a Linux envionment, so builds should be fairly straightforward on most POSIX systems. Builds on Windows may require a little more work, but the easiest option is to use WSL (Windows Subsystem for Linux). Build tools are required for use of `make` (e.g. `build-essential` under Linux) and a modern C++ compiler supporting the C++17 standard, as the implementations make use of some C++17 features. The GCC compiler suite was used for development and testing, but this should be mostly compatible with other compilers (some of the compiler flags may need to be changed or removed).

The [interpreter](./interpreter/) and [compiler](./compiler) implementations are each fully standalone, but share a common core comprising the `ast`, `builtins`, `lexer`, `object`, `parser` and `token` source files (with minor differences between the two implementations). The standard Makefile builds provide a REPL for both the interpreter and compiler to be able to enter and execute Monkey code, which are generated in the repository root as `monkey-interpreter` and `monkey-compiler`.

The interpreter and compiler can be built by issuing the following in the repository root:

```sh
$ make
```

Alternatively, each can be built individually:

```sh
$ make interpreter
$ make compiler
```

## Under the Hood

TBC

## Benchmarks

Simple benchmark builds are provided in order to compare the relative performance of the interpreter and compiler implementations. The benchmarks follow those outlined in the books, which are based on the (very inefficient) recursive calculation of the value of the 35th term in the fibonacci sequence (9227465). These work by simply switching out the "front-end" REPL for a hard-coded input string including the definition of a recursive `fibonacci` function followed by a call to `fibonacci(35)`.

The interpreter and compiler benchmark runs can optionally each be modified by first editing the files [compiler/benchmark.cpp](./compiler/benchmark.cpp) or [interpreter/benchmark.cpp](./interpreter/benchmark.cpp), and then built following the example commands below from in the repository root. The benchmark builds will produce the relevant executables `benchmark-interpreter` and `benchmark-compiler`. Finally, the relevant benchmarks can be run by executing these binary builds.

```sh
$ make interpreter-benchmark
$ ./benchmark-interpreter
$ make compiler-benchmark
$ ./benchmark-compiler
```

Reference timing information from benchmark runs on a 13th Gen Intel i5-13500H @ 2.06 GHz is provided below for these C++ implementations of the interpreter and compiler, and also equivalent runs from the final Go implementations on the same machine.

| Implementation | Interpreter time (s) | Compiler time (s) |
| :---: | :---: | :---: |
| Go (book) | 12.8 | 2.9 |
| C++ (this repo) | 126.2 | 19.6 |

## Testing

A test suite is included for each of the interpreter and compiler within the [test/interpreter/](./test/interpreter) and [test/compiler/](./test/compiler) directories, which are based on those provided in the original Go implementations in the books. The [Catch2](https://github.com/catchorg/Catch2) unit testing framework is used to orchestrate tests and the required header-only implementation is included in `test/include`.

To run all interpreter tests (and clean up artifacts):

```sh
$ cd test/interpreter
$ make
$ make clean
```

To run all compiler tests (and clean up test artifacts):

```sh
$ cd test/compiler
$ make
$ make clean
```

To run only a specific test set (e.g. only the lexer within the compiler), then these can be run as for example `make test-lexer` from within the `test/compiler` directory.

## License

This software is released under the MIT license [MIT](LICENSE).

It is based on the design of the Go implementations (also released under an MIT license) in the books [Writing An Interpreter In Go](https://interpreterbook.com/) and [Writing A Compiler In Go](https://compilerbook.com/), written by Thorsten Ball.
