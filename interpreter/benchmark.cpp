#include <chrono>
#include <iostream>

#include "evaluator.hpp"
#include "parser.hpp"

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

    auto env = new_environment();

    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();

    // Execute benchmark run
    auto result = eval(program, env);

    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> benchmark_time = t2 - t1;

    std::cout << "Benchmark time: " << benchmark_time.count() / 1000 << "s" << std::endl;

    return EXIT_SUCCESS;
}
