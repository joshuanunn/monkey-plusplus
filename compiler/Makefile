CXXFLAGS = -g --std=c++17 -I../include

monkey : ast.o code.o compiler.o lexer.o object.o parser.o symbol_table.o token.o vm.o repl.o monkey.o
	$(CXX) $(CXXFLAGS) $^ -o $@

codetest : code.o code.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

compilertest : ast.o code.o compiler.o lexer.o object.o parser.o symbol_table.o token.o compiler.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

symboltabletest : symbol_table.o symbol_table.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

vmtest : ast.o code.o compiler.o lexer.o object.o parser.o symbol_table.o token.o vm.o vm.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test-code : codetest
	./codetest

test-compiler : compilertest
	./compilertest

test-symboltable : symboltabletest
	./symboltabletest

test-vm : vmtest
	./vmtest

clean :
	rm -rf *.o codetest compilertest symboltabletest vmtest monkey
