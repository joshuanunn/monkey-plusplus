#include <iostream>

#include "repl.hpp"
#include "compiler.hpp"
#include "lexer.hpp"
#include "object.hpp"
#include "parser.hpp"
#include "vm.hpp"

void start_repl() {
    std::string line;
    std::shared_ptr<Error> err;

    // Initialise global state to exist between REPL iterations
    std::vector<std::shared_ptr<Object>> constants;
    std::array<std::shared_ptr<Object>, GLOBALSSIZE> globals;
    auto symbol_table = new_symbol_table();

    // Define all builtin functions
    for (int i = 0; i < static_cast<int>(builtins_names.size()); i++) {
        symbol_table->define_builtin(i, builtins_names[i]);
    }

    while (true) {
        std::cout << PROMPT;

        std::getline(std::cin, line);

        auto l = Lexer(line);
        auto p = Parser(std::move(l));

        auto program = p.parse_program();
        if (!p.errors().empty()) {
            print_parser_errors(p.errors());
            continue;
        }

        if (program->statements.empty()) {
            continue;
        }

        // Initialise new compiler maintaining state from previous iterations
        auto comp = new_compiler_with_state(symbol_table, constants);
        err = comp->compile(program);
        if (err) {
            std::cout << "Woops! Compilation failed:\n " << err->message << std::endl;
            continue;
        }

        auto code = comp->bytecode();

        // Add any new constants from this compilation cycle to global REPL constants
        auto new_constants = code->constants;
        for (int i = constants.size(); i < static_cast<int>(new_constants.size()); i++) {
            constants.push_back(new_constants.at(i)->clone());
        }

        auto machine = VM(std::move(code), globals);
        err = machine.run();
        if (err) {
            std::cout << "Woops! Executing bytecode failed:\n " << err->message << std::endl;
            continue;
        }

        // Update REPL globals from those after running VM
        std::copy(std::begin(machine.globals), std::end(machine.globals), std::begin(globals));

        auto last_popped = machine.last_popped_stack_elem();
        std::cout << last_popped->inspect() << std::endl;
    }
}

void print_parser_errors(std::vector<std::string> errors) {
    std::cout << MONKEY_FACE << std::endl;
    std::cout << "Woops! We ran into some monkey business here!" << std::endl;
    std::cout << " parser errors:" << std::endl;

    for (const auto &msg: errors) {
        std::cout << "\t" + msg << std::endl;
    }
}
