#include "lexer.hpp"

bool is_letter(char ch) {
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_';
}

bool is_digit(char ch) {
    return '0' <= ch && ch <= '9';
}

Lexer::Lexer(const std::string& input_in) : input{input_in}, position{}, read_position{}, ch{} {
    read_char();
}

Lexer::Lexer(Lexer &&other) noexcept: input(std::move(other.input)), position(other.position),
                                      read_position(other.read_position), ch(other.ch) {
    other.input = "";
    other.position = 0;
    other.read_position = 0;
    other.ch = 0;
}

void Lexer::read_char() {
    if (read_position >= static_cast<int>(input.length())) {
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
    if (read_position >= static_cast<int>(input.length())) {
        return 0;
    } else {
        return input[read_position];
    }
}

std::string Lexer::read_string() {
    int start_position = position + 1;
    while (true) {
        read_char();
        if (ch == '"' || ch == 0) {
            break;
        }
    }
    return input.substr(start_position, (position - start_position));
}

Token Lexer::next_token() {
    skip_whitespace();

    // Initially assume ILLEGAL Token unless proven otherwise, and store current char
    auto tok = Token{TokenType::ILLEGAL, std::string{ch}};

    switch (ch) {
        case ('='): {
            if (peek_char() == '=') {
                read_char();
                tok.type = TokenType::EQ;
                tok.literal += std::string{ch}; // Append = char to first = to give ==
            } else {
                tok.type = TokenType::ASSIGN;
            }
        }
            break;

        case ('+'): {
            tok.type = TokenType::PLUS;
        }
            break;

        case ('-'): {
            tok.type = TokenType::MINUS;
        }
            break;

        case ('!'): {
            if (peek_char() == '=') {
                read_char();
                tok.type = TokenType::NOT_EQ;
                tok.literal += std::string{ch}; // Append = char to first ! to give !=
            } else {
                tok.type = TokenType::BANG;
            }
        }
            break;

        case ('*'): {
            tok.type = TokenType::ASTERISK;
        }
            break;

        case ('/'): {
            tok.type = TokenType::SLASH;
        }
            break;

        case ('<'): {
            tok.type = TokenType::LT;
        }
            break;

        case ('>'): {
            tok.type = TokenType::GT;
        }
            break;

        case (';'): {
            tok.type = TokenType::SEMICOLON;
        }
            break;

        case (':'): {
            tok.type = TokenType::COLON;
        }
            break;

        case (','): {
            tok.type = TokenType::COMMA;
        }
            break;

        case ('('): {
            tok.type = TokenType::LPAREN;
        }
            break;

        case (')'): {
            tok.type = TokenType::RPAREN;
        }
            break;

        case ('"'): {
            tok.type = TokenType::STRING;
            tok.literal = read_string(); // Replace " char with enclosing string
        }
            break;

        case ('{'): {
            tok.type = TokenType::LBRACE;
        }
            break;

        case ('}'): {
            tok.type = TokenType::RBRACE;
        }
            break;

        case ('['): {
            tok.type = TokenType::LBRACKET;
        }
            break;

        case (']'): {
            tok.type = TokenType::RBRACKET;
        }
            break;

        case (0): {
            tok.type = TokenType::ENDOFFILE;
            tok.literal = std::string{""}; // Represent EOF literal with blank string
        }
            break;

        default: {
            if (is_letter(ch)) {
                tok.literal = read_identifier();
                tok.type = lookup_ident(tok.literal);
                return tok; // Return early
            } else if (is_digit(ch)) {
                tok.type = TokenType::INT;
                tok.literal = read_number();
                return tok; // Return early
            }
        }
    }

    read_char();

    return tok;
}
