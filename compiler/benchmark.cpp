#include <chrono>
#include <iostream>

#include "compiler.hpp"
#include "parser.hpp"
#include "vm.hpp"

int main() {
    std::string input = R"(
let fibonacci = fn(x) {
    if (x == 0) {
        return 0;
    } else {
        if (x == 1) {
            return 1;
        } else {
            fibonacci(x - 1) + fibonacci(x - 2);
        }
    }
};
fibonacci(35);
)";

    auto l = Lexer(input);
    auto p = Parser(std::move(l));
    auto program = p.parse_program();

    auto comp = new_compiler();

    auto err = comp->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
        return EXIT_FAILURE;
    }

    auto machine = VM(comp->bytecode());

    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();

    // Execute benchmark run
    err = machine.run();
    if (err) {
        std::cerr << "vm error: " << err->message << std::endl;
        return EXIT_FAILURE;
    }

    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> benchmark_time = t2 - t1;

    auto result = machine.last_popped_stack_elem();

    std::cout << "engine=vm, result=" << result->inspect() << ", duration=" << benchmark_time.count() / 1000 << std::endl;

    return EXIT_SUCCESS;
}
