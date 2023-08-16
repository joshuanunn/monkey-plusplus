#ifndef MONKEY_PLUSPLUS_LEXER_HPP
#define MONKEY_PLUSPLUS_LEXER_HPP

#include <string>
#include "token.hpp"

bool is_letter(char ch);

struct Lexer {
    explicit Lexer(const std::string &input_in);

    void read_char();

    std::string read_identifier();

    std::string read_number();

    void skip_whitespace();

    char peek_char();

    std::string read_string();

    Token next_token();

private:
    std::string input;
    int position;
    int read_position;
    char ch;
};

#endif //MONKEY_PLUSPLUS_LEXER_HPP
