CXXFLAGS = --std=c++17 -I../include

monkey : lexer.o token.o ast.o parser.o repl.o monkey.o
	$(CXX) $(CXXFLAGS) $^ -o $@

lexertest : lexer.o token.o lexer.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

parsertest : lexer.o token.o ast.o parser.o parser.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test-lexer : lexertest
	./lexertest

test-parser : parsertest
	./parsertest

clean :
	rm -rf *.o lexertest parsertest monkey
