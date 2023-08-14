#include <iostream>

#include "repl.hpp"
#include "environment.hpp"
#include "evaluator.hpp"
#include "lexer.hpp"
#include "parser.hpp"

void start_repl() {
    std::string line;
    auto env = new_environment();

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

        auto evaluated = eval(std::move(program), env);
        if (evaluated) {
            std::cout << evaluated->inspect() << std::endl;
        }
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
