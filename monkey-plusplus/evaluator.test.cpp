// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "environment.hpp"
#include "evaluator.hpp"
#include "lexer.hpp"
#include "object.hpp"
#include "parser.hpp"

std::shared_ptr<Object> test_eval (const std::string &input) {
    auto l = std::make_unique<Lexer>(Lexer(input));
    auto p = Parser(std::move(l));
    auto program = p.parse_program();
    auto env = new_environment();

    return eval(std::move(program), env);
}

bool test_integer_object(std::shared_ptr<Object> obj, int expected) {
    // Cast Object to an Integer, as this is what we are expecting
    auto result = std::dynamic_pointer_cast<Integer>(obj);

    // Check that we have an Integer Object by checking if the dynamic pointer cast fails (returns nullptr)
    if (!result) {
        std::cerr << "obj is not Integer." << std::endl;
        return false;
    }

    if (result->value != expected) {
        std::cerr << "object has wrong value. got=" << result->value << ", want=" << expected << std::endl;
        return false;
    }

    return true;
}

bool test_boolean_object(std::shared_ptr<Object> obj, bool expected) {
    // Cast Object to an Boolean, as this is what we are expecting
    auto result = std::dynamic_pointer_cast<Boolean>(obj);

    // Check that we have a Boolean Object by checking if the dynamic pointer cast fails (returns nullptr)
    if (!result) {
        std::cerr << "obj is not Boolean." << std::endl;
        return false;
    }

    if (result->value != expected) {
        std::cerr << "object has wrong value. got=" << result->value << ", want=" << expected << std::endl;
        return false;
    }

    return true;
}

bool test_null_object(const std::shared_ptr<Object> &obj) {
    // Cast Object to a Null, as this is what we are expecting.
    auto result = std::dynamic_pointer_cast<Null>(obj);

    // Check that we have a Null Object by checking if the dynamic pointer cast fails (returns nullptr)
    if (!result) {
        std::cerr << "obj is not Null." << std::endl;
        return false;
    }

    if (obj != get_null_ref()) {
        std::cerr << "obj is not Null." << std::endl;
        return false;
    }

    return true;
}

TEST_CASE("Test Eval Integer Expression") {
    std::vector<std::tuple<std::string, int>> tests = {
            std::make_tuple("5", 5),
            std::make_tuple("10", 10),
            std::make_tuple("-5", -5),
            std::make_tuple("-10", -10),
            std::make_tuple("5 + 5 + 5 + 5 - 10", 10),
            std::make_tuple("2 * 2 * 2 * 2 * 2", 32),
            std::make_tuple("-50 + 100 + -50", 0),
            std::make_tuple("5 * 2 + 10", 20),
            std::make_tuple("5 + 2 * 10", 25),
            std::make_tuple("20 + 2 * -10", 0),
            std::make_tuple("50 / 2 * 2 + 10", 60),
            std::make_tuple("2 * (5 + 10)", 30),
            std::make_tuple("3 * 3 * 3 + 10", 37),
            std::make_tuple("3 * (3 * 3) + 10", 37),
            std::make_tuple("(5 + 10 * 2 + 15 / 3) * 2 + -10", 50),
            std::make_tuple("40 + 2", 42),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);
        REQUIRE(test_integer_object(evaluated, tt_expected));
    }
}

TEST_CASE("Test Eval Boolean Expression") {
    std::vector<std::tuple<std::string, bool>> tests = {
            std::make_tuple("true", true),
            std::make_tuple("false", false),
            std::make_tuple("1 < 2", true),
            std::make_tuple("1 > 2", false),
            std::make_tuple("1 > 1", false),
            std::make_tuple("1 < 1", false),
            std::make_tuple("1 == 1", true),
            std::make_tuple("1 != 1", false),
            std::make_tuple("1 == 2", false),
            std::make_tuple("1 != 2", true),
            std::make_tuple("true == true", true),
            std::make_tuple("false == false", true),
            std::make_tuple("true == false", false),
            std::make_tuple("true != false", true),
            std::make_tuple("false != true", true),
            std::make_tuple("(1 < 2) == true", true),
            std::make_tuple("(1 < 2) == false", false),
            std::make_tuple("(1 > 2) == true", false),
            std::make_tuple("(1 > 2) == false", true),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);
        REQUIRE(test_boolean_object(evaluated, tt_expected));
    }
}

TEST_CASE("Test Bang Operator") {
    std::vector<std::tuple<std::string, bool>> tests = {
            std::make_tuple("!true", false),
            std::make_tuple("!false", true),
            std::make_tuple("!5", false),
            std::make_tuple("!!true", true),
            std::make_tuple("!!false", false),
            std::make_tuple("!!5", true),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);
        REQUIRE(test_boolean_object(evaluated, tt_expected));
    }
}

TEST_CASE("Test If Else Expressions") {
    std::vector<std::tuple<std::string, int>> tests = {
            std::make_tuple("if (true) { 10 }", 10),
            std::make_tuple("if (1) { 10 }", 10),
            std::make_tuple("if (1 < 2) { 10 }", 10),
            std::make_tuple("if (1 > 2) { 10 } else { 20 }", 20),
            std::make_tuple("if (1 < 2) { 10 } else { 20 }", 10),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);

        REQUIRE(test_integer_object(evaluated, tt_expected));
    }
}

TEST_CASE("Test If Else Expressions With Null Return") {
    std::vector<std::string> tests = {
            "if (false) { 10 }",
            "if (1 > 2) { 10 }",
    };

    for (const auto &tt_input: tests) {
        auto evaluated = test_eval(tt_input);

        REQUIRE(test_null_object(evaluated));
    }
}

TEST_CASE("Test Return Statements") {
    std::vector<std::tuple<std::string, int>> tests = {
            std::make_tuple("return 10;", 10),
            std::make_tuple("return 10; 9;", 10),
            std::make_tuple("return 2 * 5; 9;", 10),
            std::make_tuple("9; return 2 * 5; 9;", 10),
            std::make_tuple(R"(
if (10 > 1) {
  if (10 > 1) {
    return 10;
  }

  return 1;
})", 10),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);

        REQUIRE(test_integer_object(evaluated, tt_expected));
    }
}

TEST_CASE("Test Error Handling") {
    std::vector<std::tuple<std::string, std::string>> tests = {
            std::make_tuple("5 + true;", "type mismatch: INTEGER + BOOLEAN"),
            std::make_tuple("5 + true; 5;", "type mismatch: INTEGER + BOOLEAN"),
            std::make_tuple("-true", "unknown operator: -BOOLEAN"),
            std::make_tuple("true + false;", "unknown operator: BOOLEAN + BOOLEAN"),
            std::make_tuple("5; true + false; 5", "unknown operator: BOOLEAN + BOOLEAN"),
            std::make_tuple("if (10 > 1) { true + false; }", "unknown operator: BOOLEAN + BOOLEAN"),
            std::make_tuple(R"(
if (10 > 1) {
  if (10 > 1) {
    return true + false;
  }

  return 1;
})", "unknown operator: BOOLEAN + BOOLEAN"),
            std::make_tuple("foobar", "identifier not found: foobar"),
            std::make_tuple(R"("Hello" - "World")", "unknown operator: STRING - STRING"),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected_message] = tt;

        auto evaluated = test_eval(tt_input);

        auto error_obj = std::dynamic_pointer_cast<Error>(evaluated);

        // Check that we have an Error Object by checking if the dynamic pointer cast fails (returns nullptr)
        if (!error_obj) {
            std::cerr << "no error object returned." << std::endl;
        }
        REQUIRE(error_obj);

        if (error_obj->message != tt_expected_message) {
            std::cerr << "wrong error message. expected=" << tt_expected_message << ", got=" << error_obj->message << std::endl;
        }
        REQUIRE(error_obj->message == tt_expected_message);
    }
}

TEST_CASE("Test Let Statements") {
    std::vector<std::tuple<std::string, int>> tests = {
            std::make_tuple("let a = 5; a;", 5),
            std::make_tuple("let a = 5 * 5; a;", 25),
            std::make_tuple("let a = 5; let b = a; b;", 5),
            std::make_tuple("let a = 5; let b = a; let c = a + b + 5; c;", 15),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);

        REQUIRE(test_integer_object(evaluated, tt_expected));
    }
}

TEST_CASE("Test Function Object") {
    std::string input = "fn(x) { x + 2; };";

    auto evaluated = test_eval(input);

    auto fn = std::dynamic_pointer_cast<Function>(evaluated);

    // Check that we have a Function Object by checking if the dynamic pointer cast fails (returns nullptr)
    if (!fn) {
        std::cerr << "object is not Function." << std::endl;
    }
    REQUIRE(fn);

    if (fn->parameters.size() != 1) {
        std::cerr << "function has wrong parameters." << std::endl;
    }
    REQUIRE(fn->parameters.size() == 1);

    if (fn->parameters.at(0)->string() != "x") {
        std::cerr << "parameter is not 'x'. got=" << fn->parameters.at(0)->string() << std::endl;
    }
    REQUIRE(fn->parameters.at(0)->string() == "x");

    if (fn->body->string() != "(x + 2)") {
        std::cerr << "body is not (x + 2). got=" << fn->body->string() << std::endl;
    }
    REQUIRE(fn->body->string() == "(x + 2)");
}

TEST_CASE("Test Function Application") {
    std::vector<std::tuple<std::string, int>> tests = {
            std::make_tuple("let identity = fn(x) { x; }; identity(5);", 5),
            std::make_tuple("let identity = fn(x) { return x; }; identity(5);", 5),
            std::make_tuple("let double = fn(x) { x * 2; }; double(5);", 10),
            std::make_tuple("let add = fn(x, y) { x + y; }; add(5, 5);", 10),
            std::make_tuple("let add = fn(x, y) { x + y; }; add(5 + 5, add(5, 5));", 20),
            std::make_tuple("fn(x) { x; }(5)", 5),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);

        REQUIRE(test_integer_object(evaluated, tt_expected));
    }
}

TEST_CASE("Test Closures") {
    std::string input = R"(
let newAdder = fn(x) {
  fn(y) { x + y };
};

let addTwo = newAdder(2);
addTwo(2);)";

    auto evaluated = test_eval(input);

    REQUIRE(test_integer_object(evaluated, 4));
}

TEST_CASE("Test String Literal") {
    std::string input = R"("Hello World!")";

    auto evaluated = test_eval(input);

    auto str = std::dynamic_pointer_cast<String>(evaluated);

    // Check that we have a String Object by checking if the dynamic pointer cast fails (returns nullptr)
    if (!str) {
        std::cerr << "object is not String." << std::endl;
    }
    REQUIRE(str);

    if (str->value != "Hello World!") {
        std::cerr << "String has wrong value. got=" << str->value << std::endl;
    }
    REQUIRE(str->value == "Hello World!");
}

TEST_CASE("Test String Concatenation") {
    std::string input = R"("Hello" + " " + "World!")";

    auto evaluated = test_eval(input);

    auto str = std::dynamic_pointer_cast<String>(evaluated);

    // Check that we have a String Object by checking if the dynamic pointer cast fails (returns nullptr)
    if (!str) {
        std::cerr << "object is not String." << std::endl;
    }
    REQUIRE(str);

    if (str->value != "Hello World!") {
        std::cerr << "String has wrong value. got=" << str->value << std::endl;
    }
    REQUIRE(str->value == "Hello World!");
}

TEST_CASE("Test Builtin Functions Returning Integers") {
    std::vector<std::tuple<std::string, int>> tests = {
            std::make_tuple(R"(len(""))", 0),
            std::make_tuple(R"(len("four"))", 4),
            std::make_tuple(R"(len("hello world"))", 11),
            std::make_tuple(R"(len([1, 2, 3]))", 3),
            std::make_tuple(R"(len([]))", 0),
            std::make_tuple(R"(first([1, 2, 3]))", 1),
            std::make_tuple(R"(last([1, 2, 3]))", 3),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);

        REQUIRE(test_integer_object(evaluated, tt_expected));
    }
}

TEST_CASE("Test Builtin Functions Returning Arrays") {
    std::vector<std::tuple<std::string, std::vector<int>>> tests = {
            std::make_tuple(R"(rest([1, 2, 3]))", std::vector<int>{2, 3}),
            std::make_tuple(R"(rest([1, 2]))", std::vector<int>{2}),
            std::make_tuple(R"(rest([1]))", std::vector<int>{}),
            std::make_tuple(R"(push([1, 2], 3))", std::vector<int>{1, 2, 3}),
            std::make_tuple(R"(push([1], 2))", std::vector<int>{1, 2}),
            std::make_tuple(R"(push([], 1))", std::vector<int>{1}),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);

        auto array = std::dynamic_pointer_cast<Array>(evaluated);

        // Check that we have an Array Object by checking if the dynamic pointer cast fails (returns nullptr)
        if (!array) {
            std::cerr << "object is not Array." << std::endl;
        }
        REQUIRE(array);

        if (array->elements.size() != tt_expected.size()) {
            std::cerr << "wrong num of elements. want=" << tt_expected.size()
                      << ", got=" << array->elements.size() << std::endl;
        }
        REQUIRE(array->elements.size() == tt_expected.size());

        for (int i = 0; i < tt_expected.size(); i++) {
            REQUIRE(test_integer_object(array->elements.at(i), tt_expected.at(i)));
        }
    }
}

TEST_CASE("Test Builtin Functions Returning Null") {
    std::vector<std::string> tests = {
            R"(first([]))",
            R"(last([]))",
            R"(rest([]))",
    };

    for (const auto &tt: tests) {

        auto evaluated = test_eval(tt);

        REQUIRE(test_null_object(evaluated));
    }
}

TEST_CASE("Test Builtin Function Returning Errors") {
    std::vector<std::tuple<std::string, std::string>> tests = {
            std::make_tuple("len(1)", "argument to 'len' not supported."),
            std::make_tuple(R"(len("one", "two"))", "wrong number of arguments. got=2, want=1"),
            std::make_tuple(R"(first(1))", "argument to 'first' must be ARRAY, got INTEGER"),
            std::make_tuple(R"(last(1))", "argument to 'last' must be ARRAY, got INTEGER"),
            std::make_tuple(R"(push(1, 1))", "argument to 'push' must be ARRAY, got INTEGER"),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);

        auto err_obj = std::dynamic_pointer_cast<Error>(evaluated);

        // Check that we have an Error Object by checking if the dynamic pointer cast fails (returns nullptr)
        if (!err_obj) {
            std::cerr << "object is not Error." << std::endl;
        }
        REQUIRE(err_obj);

        if (err_obj->message != tt_expected) {
            std::cerr << "wrong error message. expected=" << tt_expected << ", got=" << err_obj->message << std::endl;
        }
        REQUIRE(err_obj->message == tt_expected);
    }
}

TEST_CASE("Test Array Literals") {
    std::string input = "[1, 2 * 2, 3 + 3]";

    auto evaluated = test_eval(input);

    auto result = std::dynamic_pointer_cast<Array>(evaluated);

    // Check that we have a String Object by checking if the dynamic pointer cast fails (returns nullptr)
    if (!result) {
        std::cerr << "object is not Array." << std::endl;
    }
    REQUIRE(result);

    if (result->elements.size() != 3) {
        std::cerr << "array has wrong num of elements. got=" << result->elements.size() << std::endl;
    }
    REQUIRE(result->elements.size() == 3);

    REQUIRE(test_integer_object(result->elements.at(0), 1));
    REQUIRE(test_integer_object(result->elements.at(1), 4));
    REQUIRE(test_integer_object(result->elements.at(2), 6));
}

TEST_CASE("Test Array Index Expressions") {
    std::vector<std::tuple<std::string, int>> tests = {
            std::make_tuple("[1, 2, 3][0]", 1),
            std::make_tuple("[1, 2, 3][1]", 2),
            std::make_tuple("[1, 2, 3][2]", 3),
            std::make_tuple("let i = 0; [1][i];", 1),
            std::make_tuple("[1, 2, 3][1 + 1];", 3),
            std::make_tuple("let myArray = [1, 2, 3]; myArray[2];", 3),
            std::make_tuple("let myArray = [1, 2, 3]; myArray[0] + myArray[1] + myArray[2];", 6),
            std::make_tuple("let myArray = [1, 2, 3]; let i = myArray[0]; myArray[i]", 2),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);

        REQUIRE(test_integer_object(evaluated, tt_expected));
    }
}

TEST_CASE("Test Array Index Expression Errors") {
    std::vector<std::string> tests = {
            "[1, 2, 3][3]",
            "[1, 2, 3][-1]",
    };

    for (const auto &tt_input: tests) {
        auto evaluated = test_eval(tt_input);

        REQUIRE(test_null_object(evaluated));
    }
}

TEST_CASE("Test Builtin Functions Called Within User Functions (Bugfix)") {
    std::vector<std::tuple<std::string, int>> tests = {
            std::make_tuple(R"(
let shortGreeting = fn() {
  len("hello")
};

shortGreeting()
)", 5),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);

        auto val = std::dynamic_pointer_cast<Integer>(evaluated);

        // Check that we have an Integer Object by checking if the dynamic pointer cast fails (returns nullptr)
        if (!val) {
            std::cerr << "object is not Integer." << std::endl;
        }
        REQUIRE(test_integer_object(val, tt_expected));
    }
}

TEST_CASE("Building Map Example from Book") {
    std::string input = R"(
let map = fn(arr, f) {
  let iter = fn(arr, accumulated) {
    if (len(arr) == 0) {
      accumulated
    } else {
      iter(rest(arr), push(accumulated, f(first(arr))));
    }
  };

  iter(arr, []);
};

let a = [1, 2, 3, 4];
let double = fn(x) { x * 2 };
map(a, double);
)";

    std::vector<int> expected = {2, 4, 6, 8};

    auto evaluated = test_eval(input);

    auto array = std::dynamic_pointer_cast<Array>(evaluated);

    // Check that we have an Array Object by checking if the dynamic pointer cast fails (returns nullptr)
    if (!array) {
        std::cerr << "object is not Array." << std::endl;
    }
    REQUIRE(array);

    if (array->elements.size() != expected.size()) {
        std::cerr << "wrong num of elements. want=" << expected.size()
                  << ", got=" << array->elements.size() << std::endl;
    }
    REQUIRE(array->elements.size() == expected.size());

    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(test_integer_object(array->elements.at(i), expected.at(i)));
    }
}

TEST_CASE("Building Sum Reduce Example from Book") {
    std::string input = R"(
let reduce = fn(arr, initial, f) {
  let iter = fn(arr, result) {
    if (len(arr) == 0) {
      result
    } else {
      iter(rest(arr), f(result, first(arr)));
    }
  };

  iter(arr, initial);
};

let sum = fn(arr) {
  reduce(arr, 0, fn(initial, el) { initial + el });
}

sum([1, 2, 3, 4, 5]);
)";

    int expected = 15;

    auto evaluated = test_eval(input);

    auto val = std::dynamic_pointer_cast<Integer>(evaluated);

    // Check that we have an Integer Object by checking if the dynamic pointer cast fails (returns nullptr)
    if (!val) {
        std::cerr << "object is not Integer." << std::endl;
    }
    REQUIRE(val);

    REQUIRE(test_integer_object(val, expected));
}
