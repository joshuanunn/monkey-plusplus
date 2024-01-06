# monkey-plusplus

A cheeky simian-inspired interpreter and compiler for the [Monkey](https://monkeylang.org/) programming language, written in `C++`.

These are based on the books [Writing An Interpreter In Go](https://interpreterbook.com/) and [Writing A Compiler In Go](https://compilerbook.com/), written by Thorsten Ball.

## Monkey Language

TBC

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

To run all interpreter tests:

```sh
$ cd test/interpreter
$ make
```

To run all compiler tests:

```sh
$ cd test/compiler
$ make
```

To run only a specific set of test (e.g. only the lexer within the compiler), then these can be run as `make test-lexer` from within the `test/compiler` directory.

## License

This software is released under the MIT license [MIT](LICENSE).

It is based on the design of the Go implementations in the books [Writing An Interpreter In Go](https://interpreterbook.com/) and [Writing A Compiler In Go](https://compilerbook.com/), written by Thorsten Ball.
