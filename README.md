# monkey-plusplus

A cheeky simian-inspired interpreter and compiler for the [Monkey](https://monkeylang.org/) programming language, written in `C++`.

These are based on the books [Writing An Interpreter In Go](https://interpreterbook.com/) and [Writing A Compiler In Go](https://compilerbook.com/), written by Thorsten Ball.

## Monkey Language

TBC

## Building

The [interpreter](./interpreter/) and [compiler](./compiler) implementations are each fully standalone, although they each share an overlapping similar core set of source files. The standard Makefile builds provide a REPL for both the interpreter and compiler to be able to enter and execute Monkey code, which are generated in the repository root as `monkey-interpreter` and `monkey-compiler`.

The interpreter and compiler can be built under a Linux environment with a C++ compiler by issuing the command `make` in the repository root. Alternatively, each can be built individually using `make interpreter` or `make compiler`.

## Benchmarks

TBC

## Testing

TBC

## License

This software is released under the MIT license [MIT](LICENSE).

It is based on the design of the Go implementations in the books [Writing An Interpreter In Go](https://interpreterbook.com/) and [Writing A Compiler In Go](https://compilerbook.com/), written by Thorsten Ball.
