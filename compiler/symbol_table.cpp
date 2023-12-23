#include "symbol_table.hpp"

bool Symbol::operator==(const Symbol& other) const {
    return name == other.name && scope == other.scope && index == other.index;
}

bool Symbol::operator!=(const Symbol& other) const {
    return !(name == other.name && scope == other.scope && index == other.index);
}

std::ostream& operator<<(std::ostream& out, const Symbol& sym) {
    out << "Symbol{\"" << sym.name << "\", SymbolScope::";

    if (sym.scope == SymbolScope::GlobalScope) {
        out << "GlobalScope";
    } else {
        out << "UNDEFINED";
    }
    
    out << ", " << sym.index << "}";

    return out;
}

std::shared_ptr<SymbolTable> new_symbol_table() {
    return std::make_shared<SymbolTable>(SymbolTable{});
}

Symbol SymbolTable::define(std::string name) {
    auto symbol = Symbol{name, SymbolScope::GlobalScope, num_definitions};

    store[name] = symbol;
    num_definitions++;
    return symbol;
}

std::tuple<Symbol, bool> SymbolTable::resolve(const std::string& name) {
    Symbol obj;
    bool ok;

    auto contains = store.find(name);
    ok = !(contains == store.end());
    if (ok) {
        obj = store[name];
    }

    return std::make_tuple(obj, ok);
}
