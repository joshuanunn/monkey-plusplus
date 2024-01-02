CXXFLAGS = -g --std=c++17 -I../include -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wmissing-declarations -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Wswitch-default -Wundef -Werror -Wno-unused

monkey : ast.o builtins.o code.o compiler.o frame.o lexer.o object.o parser.o symbol_table.o token.o vm.o repl.o monkey.o
	$(CXX) $(CXXFLAGS) $^ -o $@

asttest : ast.o token.o ast.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

codetest : code.o code.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

compilertest : ast.o builtins.o code.o compiler.o lexer.o object.o parser.o symbol_table.o token.o compiler.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

lexertest : lexer.o token.o lexer.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

objecttest : token.o ast.o object.o object.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

parsertest : lexer.o token.o ast.o parser.o parser.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

symboltabletest : symbol_table.o symbol_table.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

tokentest : token.o token.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

vmtest : ast.o builtins.o code.o compiler.o frame.o lexer.o object.o parser.o symbol_table.o token.o vm.o vm.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test-ast : asttest
	./asttest

test-code : codetest
	./codetest

test-compiler : compilertest
	./compilertest

test-lexer : lexertest
	./lexertest

test-object : objecttest
	./objecttest

test-parser : parsertest
	./parsertest

test-symboltable : symboltabletest
	./symboltabletest

test-token : tokentest
	./tokentest

test-vm : vmtest
	./vmtest

clean :
	rm -rf *.o asttest codetest compilertest lexertest objecttest parsertest symboltabletest tokentest vmtest monkey
