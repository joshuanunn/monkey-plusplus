#include "lexer.hpp"

bool is_letter(char ch) {
    return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_';
}

bool is_digit(char ch) {
    return '0' <= ch && ch <= '9';
}

Lexer::Lexer(const std::string &input_in) : input{input_in}, position{}, read_position{} {
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

std::string Lexer::read_string() {
    int start_position = position + 1;
    while(true) {
        read_char();
        if (ch == '"' || ch == 0) {
            break;
        }
    }
    return input.substr(start_position, (position - start_position));
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
                tok = {TokenType::ASSIGN, std::string{ch}};
            }
        }
            break;
        case ('+'): {
            tok = {TokenType::PLUS, std::string{ch}};
        }
            break;
        case ('-'): {
            tok = {TokenType::MINUS, std::string{ch}};
        }
            break;
        case ('!'): {
            if (peek_char() == '=') {
                char ch1 = ch;
                read_char();
                tok = {TokenType::NOT_EQ, std::string{ch1} + std::string{ch}};
            } else {
                tok = {TokenType::BANG, std::string{ch}};
            }
        }
            break;
        case ('*'): {
            tok = {TokenType::ASTERISK, std::string{ch}};
        }
            break;
        case ('/'): {
            tok = {TokenType::SLASH, std::string{ch}};
        }
            break;
        case ('<'): {
            tok = {TokenType::LT, std::string{ch}};
        }
            break;
        case ('>'): {
            tok = {TokenType::GT, std::string{ch}};
        }
            break;
        case (';'): {
            tok = {TokenType::SEMICOLON, std::string{ch}};
        }
            break;
        case (':'): {
            tok = {TokenType::COLON, std::string{ch}};
        }
            break;
        case (','): {
            tok = {TokenType::COMMA, std::string{ch}};
        }
            break;
        case ('('): {
            tok = {TokenType::LPAREN, std::string{ch}};
        }
            break;
        case (')'): {
            tok = {TokenType::RPAREN, std::string{ch}};
        }
            break;
        case ('"'): {
            tok = {TokenType::STRING, read_string()};
        }
            break;
        case ('{'): {
            tok = {TokenType::LBRACE, std::string{ch}};
        }
            break;
        case ('}'): {
            tok = {TokenType::RBRACE, std::string{ch}};
        }
            break;
        case ('['): {
            tok = {TokenType::LBRACKET, std::string{ch}};
        }
            break;
        case (']'): {
            tok = {TokenType::RBRACKET, std::string{ch}};
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
