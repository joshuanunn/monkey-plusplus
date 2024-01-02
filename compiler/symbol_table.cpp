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
    } else if (sym.scope == SymbolScope::FreeScope) {
        out << "FreeScope";
    } else if (sym.scope == SymbolScope::FunctionScope) {
        out << "FunctionScope";
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
    auto symbol = Symbol{};
    symbol.name = name;
    symbol.index = num_definitions;

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

Symbol SymbolTable::define_free(Symbol original) {
    free_symbols.push_back(original);

    auto symbol = Symbol{
        original.name,
        SymbolScope::FreeScope,
        static_cast<int>(free_symbols.size()) - 1
    };

    store[original.name] = symbol;
    return symbol;
}

Symbol SymbolTable::define_builtin(int index, std::string name) {
    auto symbol = Symbol{name, SymbolScope::BuiltinScope, index};
    store[name] = symbol;
    return symbol;
}

Symbol SymbolTable::define_function_name(std::string name) {
    auto symbol = Symbol{name, SymbolScope::FunctionScope, 0};
    store[name] = symbol;
    return symbol;
}

std::tuple<Symbol, bool> SymbolTable::resolve(const std::string& name) {
    // Has name been defined in this scope (i.e. this symbol table)?
    Symbol obj;
    auto contains = store.find(name);
    auto ok = !(contains == store.end());
    if (ok) {
        obj = store[name];
    }

    // If not then...
    if (!ok && outer) {
        auto [o_obj, o_ok] = outer->resolve(name);
        if (!o_ok) {
            return std::make_tuple(o_obj, o_ok);
        }

        // Is the name a global binding or builtin function?
        if (o_obj.scope == SymbolScope::GlobalScope || o_obj.scope == SymbolScope::BuiltinScope) {
            return std::make_tuple(o_obj, o_ok);
        }

        // Otherwise name was defined as a local in an enclosing scope, and is therefore a free variable in this scope
        auto free = define_free(o_obj);
        return std::make_tuple(free, true);
    }

    return std::make_tuple(obj, ok);
}
