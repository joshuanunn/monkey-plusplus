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

std::shared_ptr<Node> LetStatement::clone() const {
    auto new_copy = LetStatement{*this};
    return std::make_shared<LetStatement>(new_copy);
}

LetStatement::LetStatement(const Token& t) : token{t} {}

LetStatement::LetStatement(const LetStatement& other) : token{other.token} {
    // Call clone methods to force deep copy
    name = std::dynamic_pointer_cast<Identifier>(other.name->clone());
    value = std::dynamic_pointer_cast<Expression>(other.value->clone());
}

LetStatement::LetStatement(LetStatement&& other) noexcept {
    token = std::move(other.token);
    name = std::move(other.name);
    value = std::move(other.value);

    other.name = nullptr;
    other.value = nullptr;
    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
}

LetStatement& LetStatement::operator=(const LetStatement& other) {
    if (this == &other) return *this;

    token = other.token;

    // Call clone methods to force deep copy
    name = std::dynamic_pointer_cast<Identifier>(other.name->clone());
    value = std::dynamic_pointer_cast<Expression>(other.value->clone());

    return *this;
}

LetStatement& LetStatement::operator=(LetStatement&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    name = std::move(other.name);
    value = std::move(other.value);

    other.name = nullptr;
    other.value = nullptr;
    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";

    return *this;
}

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

std::shared_ptr<Node> ReturnStatement::clone() const {
    auto new_copy = ReturnStatement{*this};
    return std::make_shared<ReturnStatement>(new_copy);
}

ReturnStatement::ReturnStatement(const Token& t) : token(t) {}

ReturnStatement::ReturnStatement(const ReturnStatement& other) : token{other.token} {
    // Call clone methods to force deep copy
    return_value = std::dynamic_pointer_cast<Expression>(other.return_value->clone());
}

ReturnStatement::ReturnStatement(ReturnStatement&& other) noexcept {
    token = std::move(other.token);
    return_value = std::move(other.return_value);

    other.return_value = nullptr;
    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
}

ReturnStatement& ReturnStatement::operator=(const ReturnStatement& other) {
    if (this == &other) return *this;

    token = other.token;

    // Call clone methods to force deep copy
    return_value = std::dynamic_pointer_cast<Expression>(other.return_value->clone());

    return *this;
}

ReturnStatement& ReturnStatement::operator=(ReturnStatement&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    return_value = std::move(other.return_value);

    other.return_value = nullptr;
    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";

    return *this;
}

std::string ExpressionStatement::token_literal() const {
    return token.literal;
}

std::string ExpressionStatement::string() const {
    std::string out;

    out.append(expression->string());

    return out;
}

std::shared_ptr<Node> ExpressionStatement::clone() const {
    auto new_copy = ExpressionStatement{*this};
    return std::make_shared<ExpressionStatement>(new_copy);
}

ExpressionStatement::ExpressionStatement(const Token& t) : token{t} {}

ExpressionStatement::ExpressionStatement(const ExpressionStatement& other) : token{other.token} {
    // Call clone methods to force deep copy
    expression = std::dynamic_pointer_cast<Expression>(other.expression->clone());
}

ExpressionStatement::ExpressionStatement(ExpressionStatement&& other) noexcept {
    token = std::move(other.token);
    expression = std::move(other.expression);

    other.expression = nullptr;
    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
}

ExpressionStatement& ExpressionStatement::operator=(const ExpressionStatement& other) {
    if (this == &other) return *this;

    token = other.token;

    // Call clone methods to force deep copy
    expression = std::dynamic_pointer_cast<Expression>(other.expression->clone());

    return *this;
}

ExpressionStatement& ExpressionStatement::operator=(ExpressionStatement&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    expression = std::move(other.expression);

    other.expression = nullptr;
    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";

    return *this;
}

std::string Identifier::token_literal() const {
    return token.literal;
}

std::string Identifier::string() const {
    return value;
}

std::shared_ptr<Node> Identifier::clone() const {
    auto new_copy = Identifier{*this};
    return std::make_shared<Identifier>(new_copy);
}

Identifier::Identifier(const Token& t, const std::string& v) : token{t}, value{v} {}

Identifier::Identifier(const Identifier &other) : token{other.token}, value{other.value} {}

Identifier::Identifier(Identifier&& other) noexcept {
    token = std::move(other.token);
    value = std::move(other.value);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.value = "";
}

Identifier& Identifier::operator=(const Identifier& other) {
    if (this == &other) return *this;

    token = other.token;
    value = other.value;

    return *this;
}

Identifier& Identifier::operator=(Identifier&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    value = std::move(other.value);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.value = "";

    return *this;
}

std::string IntegerLiteral::token_literal() const {
    return token.literal;
}

std::string IntegerLiteral::string() const {
    return std::to_string(value);
}

std::shared_ptr<Node> IntegerLiteral::clone() const {
    auto new_copy = IntegerLiteral{*this};
    return std::make_shared<IntegerLiteral>(new_copy);
}

IntegerLiteral::IntegerLiteral(const Token& t) : token{t}, value{} {}

IntegerLiteral::IntegerLiteral(const IntegerLiteral& other) : token{other.token}, value{other.value} {}

IntegerLiteral::IntegerLiteral(IntegerLiteral&& other) noexcept {
    token = std::move(other.token);
    value = other.value;

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.value = 0;
}

IntegerLiteral& IntegerLiteral::operator=(const IntegerLiteral& other) {
    if (this == &other) return *this;

    token = other.token;
    value = other.value;

    return *this;
}

IntegerLiteral& IntegerLiteral::operator=(IntegerLiteral&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    value = other.value;

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.value = 0;

    return *this;
}

std::string BooleanLiteral::token_literal() const {
    return token.literal;
}

std::string BooleanLiteral::string() const {
    return token.literal;
}

std::shared_ptr<Node> BooleanLiteral::clone() const {
    auto new_copy = BooleanLiteral{*this};
    return std::make_shared<BooleanLiteral>(new_copy);
}

BooleanLiteral::BooleanLiteral(const Token& t, bool v) : token{t}, value{v} {}

BooleanLiteral::BooleanLiteral(const BooleanLiteral& other) : token{other.token}, value{other.value} {}

BooleanLiteral::BooleanLiteral(BooleanLiteral&& other) noexcept {
    token = std::move(other.token);
    value = other.value;

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.value = false;
}

BooleanLiteral& BooleanLiteral::operator=(const BooleanLiteral& other) {
    if (this == &other) return *this;

    token = other.token;
    value = other.value;

    return *this;
}

BooleanLiteral& BooleanLiteral::operator=(BooleanLiteral&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    value = other.value;

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.value = false;

    return *this;
}

std::string StringLiteral::token_literal() const {
    return token.literal;
}

std::string StringLiteral::string() const {
    return token.literal;
}

std::shared_ptr<Node> StringLiteral::clone() const {
    auto new_copy = StringLiteral{*this};
    return std::make_shared<StringLiteral>(new_copy);
}

StringLiteral::StringLiteral(const Token& t, std::string v) : token{t}, value{std::move(v)} {}

StringLiteral::StringLiteral(const StringLiteral& other) : token{other.token}, value{other.value} {}

StringLiteral::StringLiteral(StringLiteral&& other) noexcept {
    token = std::move(other.token);
    value = std::move(other.value);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.value = "";
}

StringLiteral& StringLiteral::operator=(const StringLiteral& other) {
    if (this == &other) return *this;

    token = other.token;
    value = other.value;

    return *this;
}

StringLiteral& StringLiteral::operator=(StringLiteral&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    value = std::move(other.value);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.value = "";

    return *this;
}

std::string PrefixExpression::token_literal() const {
    return token.literal;
}

std::string PrefixExpression::string() const {
    return "(" + op + right->string() + ")";
}

std::shared_ptr<Node> PrefixExpression::clone() const {
    auto new_copy = PrefixExpression{*this};
    return std::make_shared<PrefixExpression>(new_copy);
}

PrefixExpression::PrefixExpression(const Token& t, const std::string& o) : token{t}, op{o} {}

PrefixExpression::PrefixExpression(const PrefixExpression& other) : token{other.token}, op{other.op} {
    // Call clone methods to force deep copy
    right = std::dynamic_pointer_cast<Expression>(other.right->clone());
}

PrefixExpression::PrefixExpression(PrefixExpression&& other) noexcept {
    token = std::move(other.token);
    op = std::move(other.op);
    right = std::move(other.right);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.op = "";
    other.right = nullptr;
}

PrefixExpression& PrefixExpression::operator=(const PrefixExpression& other) {
    if (this == &other) return *this;

    token = other.token;
    op = other.op;

    // Call clone methods to force deep copy
    right = std::dynamic_pointer_cast<Expression>(other.right->clone());

    return *this;
}

PrefixExpression& PrefixExpression::operator=(PrefixExpression&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    op = std::move(other.op);
    right = std::move(other.right);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.op = "";
    other.right = nullptr;

    return *this;
}

std::string InfixExpression::token_literal() const {
    return token.literal;
}

std::string InfixExpression::string() const {
    return "(" + left->string() + " " + op + " " + right->string() + ")";
}

std::shared_ptr<Node> InfixExpression::clone() const {
    auto new_copy = InfixExpression{*this};
    return std::make_shared<InfixExpression>(new_copy);
}

InfixExpression::InfixExpression(const Token& t, const std::string &o) : token{t}, op{o} {}

InfixExpression::InfixExpression(const InfixExpression& other) : token{other.token}, op{other.op} {
    // Call clone methods to force deep copy
    left = std::dynamic_pointer_cast<Expression>(other.left->clone());
    right = std::dynamic_pointer_cast<Expression>(other.right->clone());
}

InfixExpression::InfixExpression(InfixExpression&& other) noexcept {
    token = std::move(other.token);
    left = std::move(other.left);
    op = std::move(other.op);
    right = std::move(other.right);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.left = nullptr;
    other.op = "";
    other.right = nullptr;
}

InfixExpression& InfixExpression::operator=(const InfixExpression& other) {
    if (this == &other) return *this;

    token = other.token;
    op = other.op;

    // Call clone methods to force deep copy
    left = std::dynamic_pointer_cast<Expression>(other.left->clone());
    right = std::dynamic_pointer_cast<Expression>(other.right->clone());

    return *this;
}

InfixExpression& InfixExpression::operator=(InfixExpression&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    left = std::move(other.left);
    op = std::move(other.op);
    right = std::move(other.right);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.left = nullptr;
    other.op = "";
    other.right = nullptr;

    return *this;
}

std::string BlockStatement::token_literal() const {
    return token.literal;
}

std::string BlockStatement::string() const {
    std::string msg;

    for (const auto &stmt: statements) {
        msg.append(stmt->string());
    }

    return msg;
}

std::shared_ptr<Node> BlockStatement::clone() const {
    auto new_copy = BlockStatement{*this};
    return std::make_shared<BlockStatement>(new_copy);
}

BlockStatement::BlockStatement(const Token& t) : token{t} {}

BlockStatement::BlockStatement(const BlockStatement& other) : token{other.token}, statements{} {
    for (const auto &s: other.statements) {
        statements.push_back(s->clone());
    }
}

BlockStatement::BlockStatement(BlockStatement &&other) noexcept {
    token = std::move(other.token);
    statements.swap(other.statements);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
}

BlockStatement& BlockStatement::operator=(const BlockStatement& other) {
    if (this == &other) return *this;

    token = other.token;

    for (const auto &s: other.statements) {
        statements.push_back(s->clone());
    }

    return *this;
}

BlockStatement& BlockStatement::operator=(BlockStatement&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    statements.swap(other.statements);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";

    return *this;
}

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

std::shared_ptr<Node> IfExpression::clone() const {
    auto new_copy = IfExpression{*this};
    return std::make_shared<IfExpression>(new_copy);
}

IfExpression::IfExpression(const Token& t) : token{t} {}

IfExpression::IfExpression(const IfExpression& other) : token{other.token} {
    // Call clone methods to force deep copy
    condition = std::dynamic_pointer_cast<Expression>(other.condition->clone());
    consequence = std::dynamic_pointer_cast<BlockStatement>(other.consequence->clone());
    alternative = std::dynamic_pointer_cast<BlockStatement>(other.alternative->clone());
}

IfExpression::IfExpression(IfExpression&& other) noexcept {
    token = std::move(other.token);
    condition = std::move(other.condition);
    consequence = std::move(other.consequence);
    alternative = std::move(other.alternative);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.condition = nullptr;
    other.consequence = nullptr;
    other.alternative = nullptr;
}

IfExpression& IfExpression::operator=(const IfExpression& other) {
    if (this == &other) return *this;

    token = other.token;

    // Call clone methods to force deep copy
    condition = std::dynamic_pointer_cast<Expression>(other.condition->clone());
    consequence = std::dynamic_pointer_cast<BlockStatement>(other.consequence->clone());
    alternative = std::dynamic_pointer_cast<BlockStatement>(other.alternative->clone());

    return *this;
}

IfExpression& IfExpression::operator=(IfExpression&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    condition = std::move(other.condition);
    consequence = std::move(other.consequence);
    alternative = std::move(other.alternative);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.condition = nullptr;
    other.consequence = nullptr;
    other.alternative = nullptr;

    return *this;
}

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

std::shared_ptr<Node> FunctionLiteral::clone() const {
    auto new_copy = FunctionLiteral{*this};
    return std::make_shared<FunctionLiteral>(new_copy);
}

FunctionLiteral::FunctionLiteral(const Token& t) : token{t} {}

FunctionLiteral::FunctionLiteral(const FunctionLiteral& other) : token{other.token}, parameters{} {
    for (const auto &s: other.parameters) {
        parameters.push_back(std::dynamic_pointer_cast<Identifier>(s->clone()));
    }

    body = std::dynamic_pointer_cast<BlockStatement>(other.body->clone());
}

FunctionLiteral::FunctionLiteral(FunctionLiteral&& other) noexcept {
    token = std::move(other.token);
    parameters.swap(other.parameters);
    body = std::move(body);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.body = nullptr;
}

FunctionLiteral& FunctionLiteral::operator=(const FunctionLiteral& other) {
    if (this == &other) return *this;

    token = other.token;

    for (const auto &s: other.parameters) {
        parameters.push_back(std::dynamic_pointer_cast<Identifier>(s->clone()));
    }

    body = std::dynamic_pointer_cast<BlockStatement>(other.body->clone());

    return *this;
}

FunctionLiteral& FunctionLiteral::operator=(FunctionLiteral&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    parameters.swap(other.parameters);
    body = std::move(body);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.body = nullptr;

    return *this;
}

std::string ArrayLiteral::token_literal() const {
    return token.literal;
}

std::string ArrayLiteral::string() const {
    std::string out = "[";

    int counter = 0;

    for (const auto &e: elements) {
        if (counter != 0) {
            out += ", ";
        }
        out += e->string();
        counter++;
    }

    out += "]";

    return out;
}

std::shared_ptr<Node> ArrayLiteral::clone() const {
    auto new_copy = ArrayLiteral{*this};
    return std::make_shared<ArrayLiteral>(new_copy);
}

ArrayLiteral::ArrayLiteral(const Token& t) : token{t} {}

ArrayLiteral::ArrayLiteral(const ArrayLiteral& other) : token{other.token}, elements{} {
    for (const auto &s: other.elements) {
        elements.push_back(std::dynamic_pointer_cast<Expression>(s->clone()));
    }
}

ArrayLiteral::ArrayLiteral(ArrayLiteral&& other) noexcept {
    token = std::move(other.token);
    elements.swap(other.elements);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
}

ArrayLiteral& ArrayLiteral::operator=(const ArrayLiteral& other) {
    if (this == &other) return *this;

    token = other.token;

    for (const auto &s: other.elements) {
        elements.push_back(std::dynamic_pointer_cast<Expression>(s->clone()));
    }

    return *this;
}

ArrayLiteral& ArrayLiteral::operator=(ArrayLiteral&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    elements.swap(other.elements);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";

    return *this;
}

std::string HashLiteral::token_literal() const {
    return token.literal;
}

std::string HashLiteral::string() const {
    std::string out = "{";

    int counter = 0;

    for (const auto &p: pairs) {
        if (counter++ != 0) {
            out += ", ";
        }

        const auto[key, value] = p;
        out += key->string() + ":" + value->string();
    }

    out += "}";

    return out;
}

std::shared_ptr<Node> HashLiteral::clone() const {
    auto new_copy = HashLiteral{*this};
    return std::make_shared<HashLiteral>(new_copy);
}

HashLiteral::HashLiteral(const Token& t) : token{t} {}

HashLiteral::HashLiteral(const HashLiteral& other) : token{other.token}, pairs{} {
    for (const auto &kv: other.pairs) {
        const auto[key, value] = kv;
        auto key_copy = std::dynamic_pointer_cast<Expression>(key->clone());
        auto value_copy = std::dynamic_pointer_cast<Expression>(value->clone());
        pairs[key_copy] = value_copy;
    }
}

HashLiteral::HashLiteral(HashLiteral&& other) noexcept {
    token = std::move(other.token);
    pairs.merge(other.pairs);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
}

HashLiteral& HashLiteral::operator=(const HashLiteral& other) {
    if (this == &other) return *this;

    token = other.token;

    for (const auto &kv: other.pairs) {
        const auto[key, value] = kv;
        auto key_copy = std::dynamic_pointer_cast<Expression>(key->clone());
        auto value_copy = std::dynamic_pointer_cast<Expression>(value->clone());
        pairs[key_copy] = value_copy;
    }

    return *this;
}

HashLiteral& HashLiteral::operator=(HashLiteral&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    pairs.merge(other.pairs);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";

    return *this;
}

std::string IndexExpression::token_literal() const {
    return token.literal;
}

std::string IndexExpression::string() const {
    std::string out = "(";
    out += left->string();
    out += "[";
    out += index->string();
    out += "])";

    return out;
}

std::shared_ptr<Node> IndexExpression::clone() const {
    auto new_copy = IndexExpression{*this};
    return std::make_shared<IndexExpression>(new_copy);
}

IndexExpression::IndexExpression(const Token& t) : token{t} {}

IndexExpression::IndexExpression(const IndexExpression& other) : token{other.token} {
    // Call clone methods to force deep copy
    left = std::dynamic_pointer_cast<Expression>(other.left->clone());
    index = std::dynamic_pointer_cast<Expression>(other.index->clone());
}

IndexExpression::IndexExpression(IndexExpression&& other) noexcept {
    token = std::move(other.token);
    left = std::move(other.left);
    index = std::move(other.index);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.left = nullptr;
    other.index = nullptr;
}

IndexExpression& IndexExpression::operator=(const IndexExpression& other) {
    if (this == &other) return *this;

    token = other.token;

    // Call clone methods to force deep copy
    left = std::dynamic_pointer_cast<Expression>(other.left->clone());
    index = std::dynamic_pointer_cast<Expression>(other.index->clone());

    return *this;
}

IndexExpression& IndexExpression::operator=(IndexExpression&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    left = std::move(other.left);
    index = std::move(other.index);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.left = nullptr;
    other.index = nullptr;

    return *this;
}

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

std::shared_ptr<Node> CallExpression::clone() const {
    auto new_copy = CallExpression{*this};
    return std::make_shared<CallExpression>(new_copy);
}

CallExpression::CallExpression(const Token& t, std::shared_ptr<Expression> f) : token{t}, function{f} {}

CallExpression::CallExpression(const CallExpression& other) : token{other.token}, arguments{} {
    function = std::dynamic_pointer_cast<Expression>(other.function->clone());

    for (const auto &s: other.arguments) {
        arguments.push_back(std::dynamic_pointer_cast<Expression>(s->clone()));
    }
}

CallExpression::CallExpression(CallExpression&& other) noexcept {
    token = std::move(other.token);
    function = std::move(function);
    arguments.swap(other.arguments);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.function = nullptr;
}

CallExpression& CallExpression::operator=(const CallExpression& other) {
    if (this == &other) return *this;

    token = other.token;
    function = std::dynamic_pointer_cast<Expression>(other.function->clone());

    for (const auto &s: other.arguments) {
        arguments.push_back(std::dynamic_pointer_cast<Expression>(s->clone()));
    }

    return *this;
}

CallExpression& CallExpression::operator=(CallExpression&& other) noexcept {
    if (this == &other) return *this;

    token = std::move(other.token);
    function = std::move(function);
    arguments.swap(other.arguments);

    other.token.type = TokenType::ILLEGAL;
    other.token.literal = "";
    other.function = nullptr;

    return *this;
}

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

std::shared_ptr<Node> Program::clone() const {
    auto new_copy = Program{*this};
    return std::make_shared<Program>(new_copy);
}

Program::Program(const Program& other) : statements{} {
    for (const auto &s: other.statements) {
        statements.push_back(s->clone());
    }
}

Program::Program(Program&& other) noexcept {
    statements.swap(other.statements);
}

Program& Program::operator=(const Program& other) {
    if (this == &other) return *this;

    for (const auto &s: other.statements) {
        statements.push_back(s->clone());
    }

    return *this;
}

Program& Program::operator=(Program&& other) noexcept {
    if (this == &other) return *this;

    statements.swap(other.statements);

    return *this;
}
