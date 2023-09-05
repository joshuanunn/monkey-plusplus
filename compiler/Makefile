CXXFLAGS = -g --std=c++17 -I../include

codetest : code.o code.test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test-code : codetest
	./codetest

clean :
	rm -rf *.o codetest
