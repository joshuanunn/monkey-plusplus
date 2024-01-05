// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "symbol_table.hpp"

TEST_CASE("Test Define") {
    std::map<std::string, Symbol> expected = {
        {"a", Symbol{"a", SymbolScope::GlobalScope, 0}},
        {"b", Symbol{"b", SymbolScope::GlobalScope, 1}},
        {"c", Symbol{"c", SymbolScope::LocalScope, 0}},
        {"d", Symbol{"d", SymbolScope::LocalScope, 1}},
        {"e", Symbol{"e", SymbolScope::LocalScope, 0}},
        {"f", Symbol{"f", SymbolScope::LocalScope, 1}},
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

    auto first_local = new_enclosed_symbol_table(global);

    auto c = first_local->define("c");
    if (c != expected["c"]) {
        std::cerr << "expected c= " << expected["c"] << ", got=" << c << std::endl;
    }
    REQUIRE(c == expected["c"]);

    auto d = first_local->define("d");
    if (d != expected["d"]) {
        std::cerr << "expected d= " << expected["d"] << ", got=" << d << std::endl;
    }
    REQUIRE(d == expected["d"]);

    auto second_local = new_enclosed_symbol_table(first_local);

    auto e = second_local->define("e");
    if (e != expected["e"]) {
        std::cerr << "expected e= " << expected["e"] << ", got=" << e << std::endl;
    }
    REQUIRE(e == expected["e"]);

    auto f = second_local->define("f");
    if (f != expected["f"]) {
        std::cerr << "expected f= " << expected["f"] << ", got=" << f << std::endl;
    }
    REQUIRE(f == expected["f"]);
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

TEST_CASE("Test Resolve Local") {
    auto global = new_symbol_table();
    global->define("a");
    global->define("b");

    auto local = new_enclosed_symbol_table(global);
    local->define("c");
    local->define("d");

    std::vector<Symbol> expected = {
        {Symbol{"a", SymbolScope::GlobalScope, 0}},
        {Symbol{"b", SymbolScope::GlobalScope, 1}},
        {Symbol{"c", SymbolScope::LocalScope, 0}},
        {Symbol{"d", SymbolScope::LocalScope, 1}},
    };

    for (const Symbol& sym : expected) {
        auto [result, ok] = local->resolve(sym.name);

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

TEST_CASE("Test Resolve Nested Local") {
    auto global = new_symbol_table();
    global->define("a");
    global->define("b");

    auto first_local = new_enclosed_symbol_table(global);
    first_local->define("c");
    first_local->define("d");

    auto second_local = new_enclosed_symbol_table(first_local);
    second_local->define("e");
    second_local->define("f");

    std::vector<Symbol> first_local_expected = {
        {Symbol{"a", SymbolScope::GlobalScope, 0}},
        {Symbol{"b", SymbolScope::GlobalScope, 1}},
        {Symbol{"c", SymbolScope::LocalScope, 0}},
        {Symbol{"d", SymbolScope::LocalScope, 1}},
    };

    for (const Symbol& sym : first_local_expected) {
        auto [result, ok] = first_local->resolve(sym.name);

        if (!ok) {
            std::cerr << "name " << sym.name << " not resolvable" << std::endl;
        }
        REQUIRE(ok);

        if (result != sym) {
            std::cerr << "expected " << sym.name << " to resolve to " << sym << ", got=" << result << std::endl;
        }
        REQUIRE(result == sym);
    }

    std::vector<Symbol> second_local_expected = {
        {Symbol{"a", SymbolScope::GlobalScope, 0}},
        {Symbol{"b", SymbolScope::GlobalScope, 1}},
        {Symbol{"e", SymbolScope::LocalScope, 0}},
        {Symbol{"f", SymbolScope::LocalScope, 1}},
    };

    for (const Symbol& sym : second_local_expected) {
        auto [result, ok] = second_local->resolve(sym.name);

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

TEST_CASE("Test Define Resolve Builtins") {
    auto global = new_symbol_table();
    auto first_local = new_enclosed_symbol_table(global);
    auto second_local = new_enclosed_symbol_table(first_local);

    std::vector<Symbol> expected = {
        {Symbol{"a", SymbolScope::BuiltinScope, 0}},
        {Symbol{"c", SymbolScope::BuiltinScope, 1}},
        {Symbol{"e", SymbolScope::BuiltinScope, 2}},
        {Symbol{"f", SymbolScope::BuiltinScope, 3}},
    };

    global->define_builtin(0, "a");
    global->define_builtin(1, "c");
    global->define_builtin(2, "e");
    global->define_builtin(3, "f");

    // Check builtins are resolvable from global scope
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

    // Check builtins are resolvable from first nested local scope
    for (const Symbol& sym : expected) {
        auto [result, ok] = first_local->resolve(sym.name);

        if (!ok) {
            std::cerr << "name " << sym.name << " not resolvable" << std::endl;
        }
        REQUIRE(ok);

        if (result != sym) {
            std::cerr << "expected " << sym.name << " to resolve to " << sym << ", got=" << result << std::endl;
        }
        REQUIRE(result == sym);
    }

    // Check builtins are resolvable from second nested local scope
    for (const Symbol& sym : expected) {
        auto [result, ok] = second_local->resolve(sym.name);

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

TEST_CASE("Test Resolve Free") {
    auto global = new_symbol_table();
    global->define("a");
    global->define("b");

    auto first_local = new_enclosed_symbol_table(global);
    first_local->define("c");
    first_local->define("d");
    
    auto second_local = new_enclosed_symbol_table(first_local);
    second_local->define("e");
    second_local->define("f");

    std::vector<Symbol> first_local_expected_symbols = {
        {Symbol{"a", SymbolScope::GlobalScope, 0}},
        {Symbol{"b", SymbolScope::GlobalScope, 1}},
        {Symbol{"c", SymbolScope::LocalScope, 0}},
        {Symbol{"d", SymbolScope::LocalScope, 1}},
    };
    std::vector<Symbol> first_local_expected_free_symbols = {};

    std::vector<Symbol> second_local_expected_symbols = {
        {Symbol{"a", SymbolScope::GlobalScope, 0}},
        {Symbol{"b", SymbolScope::GlobalScope, 1}},
        {Symbol{"c", SymbolScope::FreeScope, 0}},
        {Symbol{"d", SymbolScope::FreeScope, 1}},
        {Symbol{"e", SymbolScope::LocalScope, 0}},
        {Symbol{"f", SymbolScope::LocalScope, 1}},
    };
    std::vector<Symbol> second_local_expected_free_symbols = {
        {Symbol{"c", SymbolScope::LocalScope, 0}},
        {Symbol{"d", SymbolScope::LocalScope, 1}},
    };

    // Check symbols and free symbols are resolvable from first nested local scope
    for (const Symbol& sym : first_local_expected_symbols) {
        auto [result, ok] = first_local->resolve(sym.name);

        if (!ok) {
            std::cerr << "name " << sym.name << " not resolvable" << std::endl;
        }
        REQUIRE(ok);

        if (result != sym) {
            std::cerr << "expected " << sym.name << " to resolve to " << sym << ", got=" << result << std::endl;
        }
        REQUIRE(result == sym);
    }

    if (first_local->free_symbols.size() != first_local_expected_free_symbols.size()) {
        std::cerr << "wrong number of free symbols. got=" << first_local->free_symbols.size()
                  << ", want=" << first_local_expected_free_symbols.size() << std::endl;
    }
    REQUIRE(first_local->free_symbols.size() == first_local_expected_free_symbols.size());

    for (int i = 0; i < static_cast<int>(first_local_expected_free_symbols.size()); i++) {
        auto sym = first_local_expected_free_symbols.at(i);
        auto result = first_local->free_symbols.at(i);

        if (result != sym) {
            std::cerr << "wrong free symbol. got=" << result << ", want="
                      << sym.name << std::endl;
        }
        REQUIRE(result == sym);
    }

    // Check symbols and free symbols are resolvable from second nested local scope
    for (const Symbol& sym : second_local_expected_symbols) {
        auto [result, ok] = second_local->resolve(sym.name);

        if (!ok) {
            std::cerr << "name " << sym.name << " not resolvable" << std::endl;
        }
        REQUIRE(ok);

        if (result != sym) {
            std::cerr << "expected " << sym.name << " to resolve to " << sym << ", got=" << result << std::endl;
        }
        REQUIRE(result == sym);
    }

    if (second_local->free_symbols.size() != second_local_expected_free_symbols.size()) {
        std::cerr << "wrong number of free symbols. got=" << second_local->free_symbols.size()
                  << ", want=" << second_local_expected_free_symbols.size() << std::endl;
    }
    REQUIRE(second_local->free_symbols.size() == second_local_expected_free_symbols.size());

    for (int i = 0; i < static_cast<int>(second_local_expected_free_symbols.size()); i++) {
        auto sym = second_local_expected_free_symbols.at(i);
        auto result = second_local->free_symbols.at(i);

        if (result != sym) {
            std::cerr << "wrong free symbol. got=" << result << ", want="
                      << sym.name << std::endl;
        }
        REQUIRE(result == sym);
    }
}

TEST_CASE("Test Resolve Unresolvable Free") {
    auto global = new_symbol_table();
    global->define("a");

    auto first_local = new_enclosed_symbol_table(global);
    first_local->define("c");

    auto second_local = new_enclosed_symbol_table(first_local);
    second_local->define("e");
    second_local->define("f");

    std::vector<Symbol> second_local_expected = {
        {Symbol{"a", SymbolScope::GlobalScope, 0}},
        {Symbol{"c", SymbolScope::FreeScope, 0}},
        {Symbol{"e", SymbolScope::LocalScope, 0}},
        {Symbol{"f", SymbolScope::LocalScope, 1}},
    };

    std::vector<std::string> second_local_expected_unresolvable = {"b", "d"};

    // Check symbols and free symbols are resolvable from second nested local scope
    for (const Symbol& sym : second_local_expected) {
        auto [result, ok] = second_local->resolve(sym.name);

        if (!ok) {
            std::cerr << "name " << sym.name << " not resolvable" << std::endl;
        }
        REQUIRE(ok);

        if (result != sym) {
            std::cerr << "expected " << sym.name << " to resolve to " << sym << ", got=" << result << std::endl;
        }
        REQUIRE(result == sym);
    }

    for (const auto& name : second_local_expected_unresolvable) {
        auto [_, ok] = second_local->resolve(name);

        if (ok) {
            std::cerr << "name " << name << " resolved, but was expected not to" << std::endl;
        }
        REQUIRE(!ok);
    }
}

TEST_CASE("Test Define And Resolve Function Name") {
    auto global = new_symbol_table();
    global->define_function_name("a");

    auto expected = Symbol{"a", SymbolScope::FunctionScope, 0};

    auto [result, ok] = global->resolve(expected.name);

    if (!ok) {
        std::cerr << "function name " << expected.name << " not resolvable" << std::endl;
    }
    REQUIRE(ok);

    if (result != expected) {
        std::cerr << "expected " << expected.name << " to resolve to " << expected << ", got=" << result << std::endl;
    }
    REQUIRE(result == expected);
}

TEST_CASE("Test Shadowing Function Name") {
    auto global = new_symbol_table();
    global->define_function_name("a");
    global->define("a");

    auto expected = Symbol{"a", SymbolScope::GlobalScope, 0};

    auto [result, ok] = global->resolve(expected.name);

    if (!ok) {
        std::cerr << "function name " << expected.name << " not resolvable" << std::endl;
    }
    REQUIRE(ok);

    if (result != expected) {
        std::cerr << "expected " << expected.name << " to resolve to " << expected << ", got=" << result << std::endl;
    }
    REQUIRE(result == expected);
}
