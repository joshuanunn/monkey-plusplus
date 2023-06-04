CXXFLAGS = --std=c++17 -I../include

test : lexer.o token.o lexer.test.o
	$(CXX) $^ -o $@

run : test
	./test

clean :
	rm -rf *.o test
