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

TBC

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

It is based on the design of the Go implementations in the books [Writing An Interpreter In Go](https://interpreterbook.com/) and [Writing A Compiler In Go](https://compilerbook.com/), written by Thorsten Ball.
