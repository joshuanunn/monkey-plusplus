#include <iostream>

#include "repl.hpp"

int main() {
    try {
        std::string line;

        std::cout << "Hello, this is the Monkey programming language!" << std::endl;
        std::cout << "Feel free to type in commands" << std::endl;

        start_repl();
    }
    catch (const std::exception&) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
