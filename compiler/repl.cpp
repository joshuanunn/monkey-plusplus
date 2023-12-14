#include <iostream>

#include "repl.hpp"
#include "compiler.hpp"
#include "lexer.hpp"
#include "object.hpp"
#include "parser.hpp"
#include "vm.hpp"

void start_repl() {
    std::string line;
    std::shared_ptr<Error> err;

    while (true) {
        std::cout << PROMPT;

        std::getline(std::cin, line);

        auto l = Lexer(line);
        auto p = Parser(std::move(l));

        auto program = p.parse_program();
        if (!p.errors().empty()) {
            print_parser_errors(p.errors());
            continue;
        }

        auto comp = new_compiler();
        err = comp->compile(program);
        if (err) {
            std::cout << "Woops! Compilation failed:\n " << err->message << std::endl;
            continue;
        }

        auto machine = VM(comp->bytecode());
        err = machine.run();
        if (err) {
            std::cout << "Woops! Executing bytecode failed:\n " << err->message << std::endl;
            continue;
        }

        auto stack_top = machine.stack_top();
        std::cout << stack_top->inspect() << std::endl;
    }
}

void print_parser_errors(std::vector<std::string> errors) {
    std::cout << MONKEY_FACE << std::endl;
    std::cout << "Woops! We ran into some monkey business here!" << std::endl;
    std::cout << " parser errors:" << std::endl;

    for (const auto &msg: errors) {
        std::cout << "\t" + msg << std::endl;
    }
}
