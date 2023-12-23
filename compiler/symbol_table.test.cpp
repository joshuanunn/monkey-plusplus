// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "symbol_table.hpp"

TEST_CASE("Test Define") {
    std::map<std::string, Symbol> expected = {
        {"a", Symbol{"a", SymbolScope::GlobalScope, 0}},
        {"b", Symbol{"b", SymbolScope::GlobalScope, 1}},
    };

    auto global = new_symbol_table();

    auto a = global->define("a");
    if (a != expected["a"]) {
        std::cerr << "expected a= " << expected["a"] << ", got=" << a << std::endl;
    }
    REQUIRE(a == expected["a"]);

    auto b = global->define("b");
    if (b != expected["b"]) {
        std::cerr << "expected b= " << expected["b"] << ", got=" << b << std::endl;
    }
    REQUIRE(b == expected["b"]);
}

TEST_CASE("Test Resolve Global") {
    auto global = new_symbol_table();
    global->define("a");
    global->define("b");

    std::vector<Symbol> expected = {
        {Symbol{"a", SymbolScope::GlobalScope, 0}},
        {Symbol{"b", SymbolScope::GlobalScope, 1}},
    };

    for (const Symbol& sym : expected) {
        auto [result, ok] = global->resolve(sym.name);

        if (!ok) {
            std::cerr << "name " << sym.name << " not resolvable" << std::endl;
        }
        REQUIRE(ok);

        if (result != sym) {
            std::cerr << "expected " << sym.name << " to resolve to " << sym << ", got=" << result << std::endl;
        }
        REQUIRE(result == sym);
    }
}
