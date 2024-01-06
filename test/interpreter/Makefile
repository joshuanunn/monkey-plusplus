CXXFLAGS = -O3 --std=c++17 -Wall -Wextra -Werror -pedantic \
	-Wcast-align -Wcast-qual -Wmissing-declarations \
	-Wold-style-cast -Woverloaded-virtual -Wsign-promo \
	-I../include -I../../interpreter

INTERPRETER_DIR = ../../interpreter

TEST_EXE = asttest evaluatortest lexertest objecttest parsertest tokentest

AST_TEST_SRC = \
	$(INTERPRETER_DIR)/ast.cpp \
	$(INTERPRETER_DIR)/token.cpp \
	ast.test.cpp

EVALUATOR_TEST_SRC = \
	$(INTERPRETER_DIR)/ast.cpp \
	$(INTERPRETER_DIR)/builtins.cpp \
	$(INTERPRETER_DIR)/evaluator.cpp \
	$(INTERPRETER_DIR)/lexer.cpp \
	$(INTERPRETER_DIR)/object.cpp \
	$(INTERPRETER_DIR)/parser.cpp \
	$(INTERPRETER_DIR)/token.cpp \
	evaluator.test.cpp

LEXER_TEST_SRC = \
	$(INTERPRETER_DIR)/lexer.cpp \
	$(INTERPRETER_DIR)/token.cpp \
	lexer.test.cpp

OBJECT_TEST_SRC = \
	$(INTERPRETER_DIR)/ast.cpp \
	$(INTERPRETER_DIR)/object.cpp \
	$(INTERPRETER_DIR)/token.cpp \
	object.test.cpp

PARSER_TEST_SRC = \
	$(INTERPRETER_DIR)/ast.cpp \
	$(INTERPRETER_DIR)/lexer.cpp \
	$(INTERPRETER_DIR)/parser.cpp \
	$(INTERPRETER_DIR)/token.cpp \
	parser.test.cpp

TOKEN_TEST_SRC = \
	$(INTERPRETER_DIR)/token.cpp \
	token.test.cpp


AST_TEST_OBJ = $(AST_TEST_SRC:.cpp=.o)

EVALUATOR_TEST_OBJ = $(EVALUATOR_TEST_SRC:.cpp=.o)

LEXER_TEST_OBJ = $(LEXER_TEST_SRC:.cpp=.o)

OBJECT_TEST_OBJ = $(OBJECT_TEST_SRC:.cpp=.o)

PARSER_TEST_OBJ = $(PARSER_TEST_SRC:.cpp=.o)

TOKEN_TEST_OBJ = $(TOKEN_TEST_SRC:.cpp=.o)


.PHONY: all clean test-ast test-evaluator test-lexer test-object \
	test-parser test-token $(TEST_EXE)

all: test-ast test-evaluator test-lexer test-object \
	test-parser test-token $(TEST_EXE)


asttest : $(AST_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

evaluatortest : $(EVALUATOR_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

lexertest : $(LEXER_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

objecttest : $(OBJECT_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

parsertest : $(PARSER_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

tokentest : $(TOKEN_TEST_OBJ)
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
	rm -rf $(INTERPRETER_DIR)/*.o $(TEST_EXE) *.o
