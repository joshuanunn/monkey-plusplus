CXXFLAGS = -g --std=c++17 -I../include

monkey : lexer.o token.o ast.o parser.o object.o builtins.o evaluator.o repl.o monkey.o
	$(CXX) $(CXXFLAGS) $^ -o $@

asttest : ast.o token.o ast.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

evaluatortest : lexer.o token.o ast.o parser.o object.o builtins.o evaluator.o evaluator.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

lexertest : lexer.o token.o lexer.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

objecttest : token.o ast.o object.o object.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

parsertest : lexer.o token.o ast.o parser.o parser.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

tokentest : token.o token.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test-ast : asttest
	./asttest

test-evaluator : evaluatortest
	./evaluatortest

test-lexer : lexertest
	./lexertest

test-object : objecttest
	./objecttest

test-parser : parsertest
	./parsertest

test-token : tokentest
	./tokentest

clean :
	rm -rf *.o asttest evaluatortest lexertest objecttest parsertest tokentest monkey
