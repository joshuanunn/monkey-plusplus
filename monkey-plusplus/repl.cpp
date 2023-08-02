#include <iostream>

#include "repl.hpp"
#include "lexer.hpp"
#include "token.hpp"

void start_repl() {
    std::string line;

    while (true) {
        std::cout << PROMPT;

        std::getline(std::cin, line);

        auto l = Lexer(line);

        for (auto tok = l.next_token(); tok.type != TokenType::ENDOFFILE; tok = l.next_token()) {
            std::cout << tok.literal << std::endl;
        }
    }
}
