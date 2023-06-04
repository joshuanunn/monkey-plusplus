#include "lexer.hpp"

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


Token Lexer::next_token() {
    Token tok;

    switch(ch) {
        case('='): {
            tok = {TokenType::ASSIGN, "="};
        } break;
        case(';'): {
            tok = {TokenType::SEMICOLON, ";"};
        } break;
        case('('): {
            tok = {TokenType::LPAREN, "("};
        } break;
        case(')'): {
            tok = {TokenType::RPAREN, ")"};
        } break;
        case(','): {
            tok = {TokenType::COMMA, ","};
        } break;
        case('+'): {
            tok = {TokenType::PLUS, "+"};
        } break;
        case('{'): {
            tok = {TokenType::LBRACE, "{"};
        } break;
        case('}'): {
            tok = {TokenType::RBRACE, "}"};
        } break;
        case(0): {
            tok = {TokenType::ENDOFFILE, ""};
        } break;
        default: {
            // TODO: handle undefined cases.
        }
    }

    read_char();

    return tok;
}
