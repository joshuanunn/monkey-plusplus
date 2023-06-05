#include "lexer.hpp"

bool is_letter(char ch) {
    return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_';
}

bool is_digit(char ch) {
    return '0' <= ch && ch <= '9';
}

Lexer::Lexer(std::string input_in) {
    input = input_in;
    position = 0;
    read_position = 0;
    read_char();
}

void Lexer::read_char() {
    if (read_position >= input.length()) {
        ch = 0;
    } else {
        ch = input[read_position];
    }
    position = read_position++;
}

void Lexer::skip_whitespace() {
    while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
        read_char();
    }
}

std::string Lexer::read_identifier() {
    int starting_position = position;
    do {
        read_char();
    } while (is_letter(ch));
    return input.substr(starting_position, position - starting_position);
}

std::string Lexer::read_number() {
    int starting_position = position;
    do {
        read_char();
    } while (is_digit(ch));
    return input.substr(starting_position, position - starting_position);
}

char Lexer::peek_char() {
    if (read_position >= input.length()) {
        return 0;
    } else {
        return input[read_position];
    }
}

Token Lexer::next_token() {
    Token tok;

    skip_whitespace();

    switch (ch) {
        case ('='): {
            if (peek_char() == '=') {
                char ch1 = ch;
                read_char();
                tok = {TokenType::EQ, std::string{ch1} + std::string{ch}};
            } else {
                tok = {TokenType::ASSIGN, "="};
            }
        }
            break;
        case ('+'): {
            tok = {TokenType::PLUS, "+"};
        }
            break;
        case ('-'): {
            tok = {TokenType::MINUS, "-"};
        }
            break;
        case ('!'): {
            if (peek_char() == '=') {
                char ch1 = ch;
                read_char();
                tok = {TokenType::NOT_EQ, std::string{ch1} + std::string{ch}};
            } else {
                tok = {TokenType::BANG, "!"};
            }
        }
            break;
        case ('*'): {
            tok = {TokenType::ASTERISK, "*"};
        }
            break;
        case ('/'): {
            tok = {TokenType::SLASH, "/"};
        }
            break;
        case ('<'): {
            tok = {TokenType::LT, "<"};
        }
            break;
        case ('>'): {
            tok = {TokenType::GT, ">"};
        }
            break;
        case (';'): {
            tok = {TokenType::SEMICOLON, ";"};
        }
            break;
        case (','): {
            tok = {TokenType::COMMA, ","};
        }
            break;
        case ('('): {
            tok = {TokenType::LPAREN, "("};
        }
            break;
        case (')'): {
            tok = {TokenType::RPAREN, ")"};
        }
            break;
        case ('{'): {
            tok = {TokenType::LBRACE, "{"};
        }
            break;
        case ('}'): {
            tok = {TokenType::RBRACE, "}"};
        }
            break;
        case (0): {
            tok = {TokenType::ENDOFFILE, ""};
        }
            break;
        default: {
            if (is_letter(ch)) {
                tok.literal = read_identifier();
                tok.type = lookup_ident(tok.literal);
                return tok;
            } else if (is_digit(ch)) {
                tok.type = TokenType::INT;
                tok.literal = read_number();
                return tok;
            } else {
                tok = {TokenType::ILLEGAL, std::string{ch}};
            }
        }
    }

    read_char();

    return tok;
}
