#include "symbol_table.hpp"

bool Symbol::operator==(const Symbol& other) const {
    return name == other.name && scope == other.scope && index == other.index;
}

bool Symbol::operator!=(const Symbol& other) const {
    return !(name == other.name && scope == other.scope && index == other.index);
}

std::ostream& operator<<(std::ostream& out, const Symbol& sym) {
    out << "Symbol{\"" << sym.name << "\", SymbolScope::";

    if (sym.scope == SymbolScope::LocalScope) {
        out << "LocalScope";
    } else if (sym.scope == SymbolScope::GlobalScope) {
        out << "GlobalScope";
    } else if (sym.scope == SymbolScope::BuiltinScope) {
        out << "BuiltinScope";
    } else {
        out << "UNDEFINED";
    }

    out << ", " << sym.index << "}";

    return out;
}

std::shared_ptr<SymbolTable> new_symbol_table() {
    return std::make_shared<SymbolTable>(SymbolTable{});
}

std::shared_ptr<SymbolTable> new_enclosed_symbol_table(std::shared_ptr<SymbolTable> outer) {
    auto s = new_symbol_table();
    s->outer = outer;
    return s;
}

Symbol SymbolTable::define(std::string name) {
    auto symbol = Symbol{name:name, index:num_definitions};

    // If there is no outer SymbolTable set, then its scope is global
    if (!outer) {
        symbol.scope = SymbolScope::GlobalScope;
    // If it is enclosed, then its scope is local
    } else {
        symbol.scope = SymbolScope::LocalScope;
    }

    store[name] = symbol;
    num_definitions++;
    return symbol;
}

Symbol SymbolTable::define_builtin(int index, std::string name) {
    auto symbol = Symbol{name:name, scope:SymbolScope::BuiltinScope, index:index};
    store[name] = symbol;
    return symbol;
}

std::tuple<Symbol, bool> SymbolTable::resolve(const std::string& name) {
    Symbol obj;
    bool ok;

    auto contains = store.find(name);
    ok = !(contains == store.end());
    if (ok) {
        obj = store[name];
    } else if (!ok && outer) {
        return outer->resolve(name);
    }

    return std::make_tuple(obj, ok);
}
