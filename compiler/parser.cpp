#include <iostream>
#include <memory>
#include "parser.hpp"

std::map<TokenType, Precedence> precedences = {
        {TokenType::EQ, Precedence::EQUALS},
        {TokenType::NOT_EQ, Precedence::EQUALS},
        {TokenType::LT, Precedence::LESSGREATER},
        {TokenType::GT, Precedence::LESSGREATER},
        {TokenType::PLUS, Precedence::SUM},
        {TokenType::MINUS, Precedence::SUM},
        {TokenType::SLASH, Precedence::PRODUCT},
        {TokenType::ASTERISK, Precedence::PRODUCT},
        {TokenType::LPAREN, Precedence::CALL},
        {TokenType::LBRACKET, Precedence::INDEX},
};

Parser::Parser(Lexer&& lexer) : cur_token{}, peek_token{}, l{std::move(lexer)} {
    // Register prefix method pointers lookups for each token type
    register_prefix(TokenType::IDENT, std::mem_fn(&Parser::parse_identifier));
    register_prefix(TokenType::INT, std::mem_fn(&Parser::parse_integer_literal));
    register_prefix(TokenType::BANG, std::mem_fn(&Parser::parse_prefix_expression));
    register_prefix(TokenType::MINUS, std::mem_fn(&Parser::parse_prefix_expression));
    register_prefix(TokenType::TRUE, std::mem_fn(&Parser::parse_boolean));
    register_prefix(TokenType::FALSE, std::mem_fn(&Parser::parse_boolean));
    register_prefix(TokenType::LPAREN, std::mem_fn(&Parser::parse_grouped_expression));
    register_prefix(TokenType::IF, std::mem_fn(&Parser::parse_if_expression));
    register_prefix(TokenType::FUNCTION, std::mem_fn(&Parser::parse_function_literal));
    register_prefix(TokenType::STRING, std::mem_fn(&Parser::parse_string_literal));
    register_prefix(TokenType::LBRACKET, std::mem_fn(&Parser::parse_array_literal));
    register_prefix(TokenType::LBRACE, std::mem_fn(&Parser::parse_hash_literal));

    // Register infix method pointers lookups for each token type
    register_infix(TokenType::PLUS, std::mem_fn(&Parser::parse_infix_expression));
    register_infix(TokenType::MINUS, std::mem_fn(&Parser::parse_infix_expression));
    register_infix(TokenType::SLASH, std::mem_fn(&Parser::parse_infix_expression));
    register_infix(TokenType::ASTERISK, std::mem_fn(&Parser::parse_infix_expression));
    register_infix(TokenType::EQ, std::mem_fn(&Parser::parse_infix_expression));
    register_infix(TokenType::NOT_EQ, std::mem_fn(&Parser::parse_infix_expression));
    register_infix(TokenType::LT, std::mem_fn(&Parser::parse_infix_expression));
    register_infix(TokenType::GT, std::mem_fn(&Parser::parse_infix_expression));
    register_infix(TokenType::LPAREN, std::mem_fn(&Parser::parse_call_expression));
    register_infix(TokenType::LBRACKET, std::mem_fn(&Parser::parse_index_expression));

    // Read two tokens, so cur_token and peek_token are both set
    next_token();
    next_token();
}

void Parser::next_token() {
    cur_token = peek_token;
    peek_token = l.next_token();
}

std::shared_ptr<LetStatement> Parser::parse_let_statement() {
    auto stmt = std::make_shared<LetStatement>(LetStatement{cur_token});

    if (!expect_peek(TokenType::IDENT)) {
        return nullptr;
    }

    stmt->name = std::make_shared<Identifier>(Identifier{cur_token, cur_token.literal});

    if (!expect_peek(TokenType::ASSIGN)) {
        return nullptr;
    }

    next_token();

    stmt->value = parse_expression(Precedence::LOWEST);

    if (auto fl = std::dynamic_pointer_cast<FunctionLiteral>(stmt->value)) {
        fl->name = stmt->name->value;
    }

    if (peek_token_is(TokenType::SEMICOLON)) {
        next_token();
    }

    return stmt;
}

std::shared_ptr<ReturnStatement> Parser::parse_return_statement() {
    auto stmt = std::make_shared<ReturnStatement>(ReturnStatement{cur_token});

    next_token();

    stmt->return_value = parse_expression(Precedence::LOWEST);

    if (peek_token_is(TokenType::SEMICOLON)) {
        next_token();
    }

    return stmt;
}

std::shared_ptr<ExpressionStatement> Parser::parse_expression_statement() {
    auto stmt = std::make_shared<ExpressionStatement>(ExpressionStatement{cur_token});

    stmt->expression = parse_expression(Precedence::LOWEST);

    if (peek_token_is(TokenType::SEMICOLON)) {
        next_token();
    }

    return stmt;
}

std::shared_ptr<BlockStatement> Parser::parse_block_statement() {
    auto block = std::make_shared<BlockStatement>(BlockStatement{cur_token});

    next_token();

    while(!cur_token_is(TokenType::RBRACE) && !cur_token_is(TokenType::ENDOFFILE)) {
        auto stmt = parse_statement();

        if (stmt) {
            block->statements.push_back(std::move(stmt));
        }

        next_token();
    }

    return block;
}

std::shared_ptr<Statement> Parser::parse_statement() {
    switch (cur_token.type) {
        case (TokenType::LET):
            return parse_let_statement();
        case (TokenType::RETURN):
            return parse_return_statement();
        default:
            return parse_expression_statement();
    }
}

std::shared_ptr<Expression> Parser::parse_expression(Precedence precedence) {
    // Retrieve method pointer for prefix parsing based on token type and log an error if not defined
    auto pcontains = prefix_parse_fns.find(cur_token.type);
    if (pcontains == prefix_parse_fns.end()) {
        no_prefix_parse_fn_error(cur_token.type);
        return nullptr;
    }
    auto prefix = prefix_parse_fns[cur_token.type];

    // Generate an expression using the current token and prefix parsing method
    auto left_exp = prefix(this);

    while (!peek_token_is(TokenType::SEMICOLON) && precedence < peek_precedence()) {
        // Retrieve method pointer for infix parsing based on token type and log an error if not defined
        auto icontains = infix_parse_fns.find(peek_token.type);
        if (icontains == infix_parse_fns.end()) {
            //no_infix_parse_fn_error(peek_token.type);
            return left_exp;
        }
        auto infix = infix_parse_fns[peek_token.type];

        next_token();

        left_exp = infix(this, left_exp);
    }

    return left_exp;
}

std::vector<std::shared_ptr<Expression>> Parser::parse_expression_list(TokenType end) {
    std::vector<std::shared_ptr<Expression>> list;

    if (peek_token_is(end)) {
        next_token();
        return list;
    }

    next_token();
    list.push_back(parse_expression(Precedence::LOWEST));

    while(peek_token_is(TokenType::COMMA)) {
        next_token();
        next_token();
        list.push_back(parse_expression(Precedence::LOWEST));
    }

    if (!expect_peek(end)) {
        return std::vector<std::shared_ptr<Expression>>{};
    }

    return list;
}

std::shared_ptr<Expression> Parser::parse_identifier() const {
    return std::make_shared<Identifier>(Identifier{cur_token, cur_token.literal});
}

std::shared_ptr<Expression> Parser::parse_integer_literal() {
    auto lit = std::make_shared<IntegerLiteral>(IntegerLiteral{cur_token});

    // Try and parse integer literal token, and log error and return nullptr on failure
    std::size_t pos{};
    try {
        lit->value = std::stoi(cur_token.literal, &pos);
    } catch (std::invalid_argument const &ex) {
        std::string msg = "invalid argument parsing " + std::string(ex.what()) + " as int.";
        e.push_back(msg);
        return nullptr;
    } catch (std::out_of_range const &ex) {
        std::string msg = "integer literal " + std::string(ex.what()) + " to be parsed out of range.";
        e.push_back(msg);
        return nullptr;
    }

    return lit;
}

std::vector<std::shared_ptr<Identifier>> Parser::parse_function_parameters() {
    std::vector<std::shared_ptr<Identifier>> identifiers;

    if (peek_token_is(TokenType::RPAREN)) {
        next_token();
        return identifiers;
    }

    next_token();

    auto ident = std::make_shared<Identifier>(Identifier{cur_token, cur_token.literal});
    identifiers.push_back(ident);

    while (peek_token_is(TokenType::COMMA)) {
        next_token();
        next_token();
        ident = std::make_shared<Identifier>(Identifier{cur_token, cur_token.literal});
        identifiers.push_back(ident);
    }

    if (!expect_peek(TokenType::RPAREN)) {
        return std::vector<std::shared_ptr<Identifier>>{};
    }

    return identifiers;
}

std::shared_ptr<Expression> Parser::parse_function_literal() {
    auto lit = std::make_shared<FunctionLiteral>(FunctionLiteral{cur_token});

    if (!expect_peek(TokenType::LPAREN)) {
        return nullptr;
    }

    lit->parameters = parse_function_parameters();

    if (!expect_peek(TokenType::LBRACE)) {
        return nullptr;
    }

    lit->body = parse_block_statement();

    return lit;
}

std::shared_ptr<Expression> Parser::parse_boolean() const {
    return std::make_shared<BooleanLiteral>(BooleanLiteral{cur_token, cur_token_is(TokenType::TRUE)});
}

std::shared_ptr<Expression> Parser::parse_string_literal() const {
    return std::make_shared<StringLiteral>(StringLiteral{cur_token, cur_token.literal});
}

std::shared_ptr<Expression> Parser::parse_array_literal() {
    auto array = std::make_shared<ArrayLiteral>(ArrayLiteral{cur_token});

    array->elements = parse_expression_list(TokenType::RBRACKET);

    return array;
}

std::shared_ptr<Expression> Parser::parse_hash_literal() {
    auto hash = std::make_shared<HashLiteral>(HashLiteral{cur_token});

    while(!peek_token_is(TokenType::RBRACE)) {
        next_token();
        auto key = parse_expression(Precedence::LOWEST);

        if(!expect_peek(TokenType::COLON)) {
            return nullptr;
        }

        next_token();
        auto value = parse_expression(Precedence::LOWEST);

        hash->pairs[std::move(key)] = std::move(value);

        if (!peek_token_is(TokenType::RBRACE) && !expect_peek(TokenType::COMMA)) {
            return nullptr;
        }
    }

    if (!expect_peek(TokenType::RBRACE)) {
        return nullptr;
    }

    return hash;
}

std::shared_ptr<Expression> Parser::parse_prefix_expression() {
    auto expression = std::make_shared<PrefixExpression>(PrefixExpression{cur_token, cur_token.literal});

    next_token();

    expression->right = parse_expression(Precedence::PREFIX);

    return expression;
}

std::shared_ptr<Expression> Parser::parse_index_expression(std::shared_ptr<Expression> left) {
    auto exp = std::make_shared<IndexExpression>(IndexExpression{cur_token});

    exp->left = std::move(left);

    next_token();

    exp->index = parse_expression(Precedence::LOWEST);

    if (!expect_peek(TokenType::RBRACKET)) {
        return nullptr;
    }

    return exp;
}

std::shared_ptr<Expression> Parser::parse_infix_expression(std::shared_ptr<Expression> left) {
    auto expression = std::make_shared<InfixExpression>(InfixExpression{cur_token, cur_token.literal});

    expression->left = std::move(left);

    auto precedence = cur_precedence();
    next_token();

    expression->right = parse_expression(precedence);

    return expression;
}

std::shared_ptr<Expression> Parser::parse_call_expression(std::shared_ptr<Expression> function) {
    auto exp = std::make_shared<CallExpression>(CallExpression{cur_token, function});

    exp->arguments = parse_expression_list(TokenType::RPAREN);

    return exp;
}

std::shared_ptr<Expression> Parser::parse_grouped_expression() {
    next_token();

    auto exp = parse_expression(Precedence::LOWEST);

    if (!expect_peek(TokenType::RPAREN)) {
        return nullptr;
    }

    return exp;
}

std::shared_ptr<Expression> Parser::parse_if_expression() {
    auto expression = std::make_shared<IfExpression>(IfExpression{cur_token});

    if (!expect_peek(TokenType::LPAREN)) {
        return nullptr;
    }

    next_token();
    expression->condition = parse_expression(Precedence::LOWEST);

    if (!expect_peek(TokenType::RPAREN)) {
        return nullptr;
    }

    if (!expect_peek(TokenType::LBRACE)) {
        return nullptr;
    }

    expression->consequence = parse_block_statement();

    if (peek_token_is(TokenType::ELSE)) {
        next_token();

        if (!expect_peek(TokenType::LBRACE)) {
            return nullptr;
        }

        expression->alternative = parse_block_statement();
    }

    return expression;
}

std::shared_ptr<Program> Parser::parse_program() {
    auto program = std::make_shared<Program>(Program{});

    while (cur_token.type != TokenType::ENDOFFILE) {
        auto stmt = parse_statement();

        if (stmt) {
            program->statements.push_back(std::move(stmt));
        }

        next_token();
    }

    return program;
}

std::vector<std::string> Parser::errors() const {
    return e;
}

bool Parser::cur_token_is(TokenType t) const {
    return cur_token.type == t;
}

bool Parser::peek_token_is(TokenType t) const {
    return peek_token.type == t;
}

bool Parser::expect_peek(TokenType t) {
    if (peek_token_is(t)) {
        next_token();
        return true;
    } else {
        peek_error(t);
        return false;
    }
}

void Parser::peek_error(TokenType t) {
    std::string msg =
            "expected next token to be " + tokentype_literal(t) + ", got " + tokentype_literal(peek_token.type) +
            " instead";
    e.push_back(msg);
}

Precedence Parser::peek_precedence() const {
    auto precedence = precedences.find(peek_token.type);

    // If peek token type does not have a defined precedence, then return LOWEST
    if (precedence == precedences.end()) {
        return Precedence::LOWEST;
    }
    return precedences[peek_token.type];
}

Precedence Parser::cur_precedence() const {
    auto precedence = precedences.find(cur_token.type);

    // If peek token type does not have a defined precedence, then return LOWEST
    if (precedence == precedences.end()) {
        return Precedence::LOWEST;
    }
    return precedences[cur_token.type];
}

void Parser::no_prefix_parse_fn_error(TokenType t) {
    std::string msg = "no prefix parse function for " + tokentype_literal(t) + " found";
    e.push_back(msg);
}

void Parser::register_prefix(TokenType token_type, prefix_parse_fn fn) {
    prefix_parse_fns[token_type] = std::move(fn);
}

void Parser::register_infix(TokenType token_type, infix_parse_fn fn) {
    infix_parse_fns[token_type] = std::move(fn);
}
