#ifndef MONKEY_PLUSPLUS_LEXER_HPP
#define MONKEY_PLUSPLUS_LEXER_HPP

#include <string>
#include "token.hpp"

struct Lexer {
    std::string input;
    int position;
    int read_position;
    char ch;

    Lexer(std::string input_in);

    void read_char();

    Token next_token();
};

#endif //MONKEY_PLUSPLUS_LEXER_HPP
