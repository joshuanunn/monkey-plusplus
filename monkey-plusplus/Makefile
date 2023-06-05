CXXFLAGS = --std=c++17 -I../include

monkey : lexer.o token.o repl.o monkey.o
	$(CXX) $^ -o $@

lexertest : lexer.o token.o lexer.test.o
	$(CXX) $^ -o $@

test-lexer : lexertest
	./lexertest

clean :
	rm -rf *.o lexertest monkey
