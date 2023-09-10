CXXFLAGS = -g --std=c++17 -I../include

codetest : code.o code.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

compilertest : ast.o code.o compiler.o lexer.o object.o parser.o token.o compiler.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test-code : codetest
	./codetest

test-compiler : compilertest
	./compilertest

clean :
	rm -rf *.o codetest compilertest
