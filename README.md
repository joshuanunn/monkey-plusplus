# monkey-plusplus

A cheeky simian-inspired interpreter and compiler for the [Monkey](https://monkeylang.org/) programming language, written in `C++`.

These are based on the books [Writing An Interpreter In Go](https://interpreterbook.com/) and [Writing A Compiler In Go](https://compilerbook.com/), written by Thorsten Ball.

## Monkey Language

TBC

## Building

All development was completed within a Linux envionment, so builds should be fairly straightforward on most POSIX systems. Builds on Windows may require a little more work, but it would be easier to do this within WSL (Windows Subsystem for Linux). You'll need build tools for use of `make` (e.g. `build-essental` under Linux) and a modern C++ compiler supporting the C++17 standard, as the implementations make use of some C++17 features.

The [interpreter](./interpreter/) and [compiler](./compiler) implementations are each fully standalone, although they share a common core comprising the ast, builtins, lexer, object, parser and token source files (with minor differences between the two implementations). The standard Makefile builds provide a REPL for both the interpreter and compiler to be able to enter and execute Monkey code, which are generated in the repository root as `monkey-interpreter` and `monkey-compiler`.

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

TBC

## License

This software is released under the MIT license [MIT](LICENSE).

It is based on the design of the Go implementations in the books [Writing An Interpreter In Go](https://interpreterbook.com/) and [Writing A Compiler In Go](https://compilerbook.com/), written by Thorsten Ball.
