CXXFLAGS = \
	-O3 --std=c++17 -Wall -Wextra -Werror -pedantic \
	-Wcast-align -Wcast-qual -Wmissing-declarations \
	-Wold-style-cast -Woverloaded-virtual -Wsign-promo \
	-I../include -I../../compiler

COMPILER_DIR = ../../compiler

TEST_EXE = \
	asttest codetest compilertest lexertest objecttest \
	parsertest symboltabletest tokentest vmtest

AST_TEST_SRC = \
	$(COMPILER_DIR)/ast.cpp \
	$(COMPILER_DIR)/token.cpp \
	ast.test.cpp

CODE_TEST_SRC = \
	$(COMPILER_DIR)/code.cpp \
	code.test.cpp

COMPILER_TEST_SRC = \
	$(COMPILER_DIR)/ast.cpp \
	$(COMPILER_DIR)/builtins.cpp \
	$(COMPILER_DIR)/code.cpp \
	$(COMPILER_DIR)/compiler.cpp \
	$(COMPILER_DIR)/lexer.cpp \
	$(COMPILER_DIR)/object.cpp \
	$(COMPILER_DIR)/parser.cpp \
	$(COMPILER_DIR)/symbol_table.cpp \
	$(COMPILER_DIR)/token.cpp \
	compiler.test.cpp

LEXER_TEST_SRC = \
	$(COMPILER_DIR)/lexer.cpp \
	$(COMPILER_DIR)/token.cpp \
	lexer.test.cpp

OBJECT_TEST_SRC = \
	$(COMPILER_DIR)/ast.cpp \
	$(COMPILER_DIR)/object.cpp \
	$(COMPILER_DIR)/token.cpp \
	object.test.cpp

PARSER_TEST_SRC = \
	$(COMPILER_DIR)/ast.cpp \
	$(COMPILER_DIR)/lexer.cpp \
	$(COMPILER_DIR)/parser.cpp \
	$(COMPILER_DIR)/token.cpp \
	parser.test.cpp

SYMBOLTABLE_TEST_SRC = \
	$(COMPILER_DIR)/symbol_table.cpp \
	symbol_table.test.cpp

TOKEN_TEST_SRC = \
	$(COMPILER_DIR)/token.cpp \
	token.test.cpp

VM_TEST_SRC = \
	$(COMPILER_DIR)/ast.cpp \
	$(COMPILER_DIR)/builtins.cpp \
	$(COMPILER_DIR)/code.cpp \
	$(COMPILER_DIR)/compiler.cpp \
	$(COMPILER_DIR)/frame.cpp \
	$(COMPILER_DIR)/lexer.cpp \
	$(COMPILER_DIR)/object.cpp \
	$(COMPILER_DIR)/parser.cpp \
	$(COMPILER_DIR)/symbol_table.cpp \
	$(COMPILER_DIR)/token.cpp \
	$(COMPILER_DIR)/vm.cpp \
	vm.test.cpp


AST_TEST_OBJ = $(AST_TEST_SRC:.cpp=.o)

CODE_TEST_OBJ = $(CODE_TEST_SRC:.cpp=.o)

COMPILER_TEST_OBJ = $(COMPILER_TEST_SRC:.cpp=.o)

LEXER_TEST_OBJ = $(LEXER_TEST_SRC:.cpp=.o)

OBJECT_TEST_OBJ = $(OBJECT_TEST_SRC:.cpp=.o)

PARSER_TEST_OBJ = $(PARSER_TEST_SRC:.cpp=.o)

SYMBOLTABLE_TEST_OBJ = $(SYMBOLTABLE_TEST_SRC:.cpp=.o)

TOKEN_TEST_OBJ = $(TOKEN_TEST_SRC:.cpp=.o)

VM_TEST_OBJ = $(VM_TEST_SRC:.cpp=.o)


.PHONY: all clean test-ast test-code test-compiler test-lexer \
	test-object test-parser test-symboltable test-token test-vm \
	$(TEST_EXE)

all: test-ast test-code test-compiler test-lexer test-object \
	test-parser test-symboltable test-token test-vm \
	$(TEST_EXE)


asttest : $(AST_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

codetest : $(CODE_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

compilertest : $(COMPILER_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

lexertest : $(LEXER_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

objecttest : $(OBJECT_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

parsertest : $(PARSER_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

symboltabletest : $(SYMBOLTABLE_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

tokentest : $(TOKEN_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

vmtest : $(VM_TEST_OBJ)
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
	rm -rf $(COMPILER_DIR)/*.o $(TEST_EXE) *.o
