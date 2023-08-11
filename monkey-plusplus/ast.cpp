#include "ast.hpp"

std::string Statement::token_literal() const {
    return "<Statement base class>";
}

std::string Statement::string() const {
    return "<Statement base class>";
}

std::string Expression::token_literal() const {
    return "<Expression base class>";
}

std::string Expression::string() const {
    return "<Expression base class>";
}

std::string LetStatement::token_literal() const {
    return token.literal;
}

std::string LetStatement::string() const {
    std::string out;

    out.append(token_literal() + " ");
    out.append(name->string());
    out.append(" = ");
    out.append(value->string());
    out.append(";");

    return out;
}

LetStatement::LetStatement(const Token &t) : token(t) {}

std::string ReturnStatement::token_literal() const {
    return token.literal;
}

std::string ReturnStatement::string() const {
    std::string out;

    out.append(token_literal() + " ");
    out.append(return_value->string());
    out.append(";");

    return out;
}

ReturnStatement::ReturnStatement(const Token &t) : token(t) {}

std::string ExpressionStatement::token_literal() const {
    return token.literal;
}

std::string ExpressionStatement::string() const {
    std::string out;

    out.append(expression->string());

    return out;
}

ExpressionStatement::ExpressionStatement(const Token &t) : token{t} {}

std::string Identifier::token_literal() const {
    return token.literal;
}

std::string Identifier::string() const {
    return value;
}

Identifier::Identifier(const Token &t, const std::string &v) : token{t}, value{v} {}

std::string IntegerLiteral::token_literal() const {
    return token.literal;
}

std::string IntegerLiteral::string() const {
    return std::to_string(value);
}

IntegerLiteral::IntegerLiteral(const Token &t) : token{t}, value{} {}

std::string Boolean::token_literal() const {
    return token.literal;
}

std::string Boolean::string() const {
    return token.literal;
}

Boolean::Boolean(const Token &t, bool v) : token{t}, value{v} {}

std::string PrefixExpression::token_literal() const {
    return token.literal;
}

std::string PrefixExpression::string() const {
    return "(" + op + right->string() + ")";
}

PrefixExpression::PrefixExpression(const Token &t, const std::string &o) : token{t}, op{o} {}

std::string InfixExpression::token_literal() const {
    return token.literal;
}

std::string InfixExpression::string() const {
    return "(" + left->string() + " " + op + " " + right->string() + ")";
}

InfixExpression::InfixExpression(const Token &t, const std::string &o) : token{t}, op{o} {}

std::string BlockStatement::token_literal() const {
    return token.literal;
}

std::string BlockStatement::string() const {
    std::string msg;

    for (const auto &stmt: statements) {
        msg += stmt->string();
    }

    return msg;
}

BlockStatement::BlockStatement(const Token &t) : token(t) {}

std::string IfExpression::token_literal() const {
    return token.literal;
}

std::string IfExpression::string() const {
    std::string msg = "if" + condition->string() + " " + consequence->string();

    if (alternative) {
        msg += "else ";
        msg += alternative->string();
    }

    return msg;
}

IfExpression::IfExpression(const Token &t) : token(t) {}

std::string FunctionLiteral::token_literal() const {
    return token.literal;
}

std::string FunctionLiteral::string() const {
    std::string msg = token.literal + "(";

    int counter = 0;

    for (const auto &p: parameters) {
        if (counter != 0) {
            msg += ", ";
        }
        msg += p->string();
        counter++;
    }

    msg += ") ";
    msg += body->string();

    return msg;
}

FunctionLiteral::FunctionLiteral(const Token &t) : token(t) {}

std::string CallExpression::token_literal() const {
    return token.literal;
}

std::string CallExpression::string() const {
    std::string msg = function->string() + "(";

    int counter = 0;

    for (const auto &a: arguments) {
        if (counter != 0) {
            msg += ", ";
        }
        msg += a->string();
        counter++;
    }

    msg += ")";

    return msg;
}

CallExpression::CallExpression(const Token &t, std::shared_ptr<Expression> f) : token(t), function(f) {}

std::string Program::token_literal() const {
    if (!statements.empty()) {
        return statements.at(0)->token_literal();
    } else {
        return "";
    }
}

std::string Program::string() const {
    std::string out;

    for (const auto &s: statements) {
        out.append(s->string());
    }

    return out;
}
