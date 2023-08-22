#ifndef MONKEY_PLUSPLUS_LEXER_HPP
#define MONKEY_PLUSPLUS_LEXER_HPP

#include <string>
#include "token.hpp"

bool is_letter(char ch);

struct Lexer {
    Lexer() = delete;

    explicit Lexer(const std::string& input_in);

    Lexer(const Lexer& other) = delete;

    Lexer(Lexer&& other) noexcept;

    Lexer& operator=(const Lexer& other) = delete;

    Lexer& operator=(Lexer&& other) noexcept = delete;

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
