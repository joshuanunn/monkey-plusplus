#include <iostream>

#include "repl.hpp"
#include "lexer.hpp"
#include "parser.hpp"

void start_repl() {
    std::string line;

    while (true) {
        std::cout << PROMPT;

        std::getline(std::cin, line);

        auto l = std::make_unique<Lexer>(Lexer(line));
        auto p = Parser(std::move(l));

        auto program = p.parse_program();
        if (!p.errors().empty()) {
            print_parser_errors(p.errors());
            continue;
        }

        std::cout << program->string() << std::endl;
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
