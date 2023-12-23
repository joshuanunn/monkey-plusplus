#ifndef MONKEY_PLUSPLUS_SYMBOL_TABLE_HPP
#define MONKEY_PLUSPLUS_SYMBOL_TABLE_HPP

#include <iomanip>
#include <map>
#include <memory>
#include <string>

enum class SymbolScope {
    GlobalScope
};

struct Symbol {
    std::string name;

    SymbolScope scope;

    int index;

    bool operator==(const Symbol& other) const;

    bool operator!=(const Symbol& other) const;
};

std::ostream& operator<<(std::ostream& out, const Symbol& sym);

struct SymbolTable {
    SymbolTable() = default;

    SymbolTable(const SymbolTable& other) = delete;

    SymbolTable(SymbolTable&& other) noexcept = default;

    SymbolTable& operator=(const SymbolTable& other) = delete;

    SymbolTable& operator=(SymbolTable&& other) noexcept = delete;

    std::map<std::string, Symbol> store;

    int num_definitions;

    Symbol define(std::string name);

    std::tuple<Symbol, bool> resolve(const std::string& name);
};

std::shared_ptr<SymbolTable> new_symbol_table();

#endif //MONKEY_PLUSPLUS_SYMBOL_TABLE_HPP