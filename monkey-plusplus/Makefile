CXXFLAGS = --std=c++17 -I../include

monkey : lexer.o token.o repl.o
	$(CXX) $^ -o $@

lexertest : lexer.o token.o lexer.test.o
	$(CXX) $^ -o $@

repl : monkey
	./monkey

test-lexer : lexertest
	./lexertest

clean :
	rm -rf *.o lexertest monkey
