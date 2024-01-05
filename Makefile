CXXFLAGS =	-O3 --std=c++17 -Wall -Wextra -Werror -pedantic \
			-Wcast-align -Wcast-qual -Wmissing-declarations \
			-Wold-style-cast -Woverloaded-virtual -Wsign-promo

COMPILER_EXE = monkey-compiler
COMPILER_BENCHMARK_EXE = benchmark-compiler

INTERPRETER_EXE = monkey-interpreter
INTERPRETER_BENCHMARK_EXE = benchmark-interpreter

COMPILER_DIR = ./compiler
INTERPRETER_DIR = ./interpreter

COMPILER_SRC = \
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
				$(COMPILER_DIR)/vm.cpp

INTERPRETER_SRC = \
				$(INTERPRETER_DIR)/ast.cpp \
				$(INTERPRETER_DIR)/builtins.cpp \
				$(INTERPRETER_DIR)/evaluator.cpp \
				$(INTERPRETER_DIR)/lexer.cpp \
				$(INTERPRETER_DIR)/object.cpp \
				$(INTERPRETER_DIR)/parser.cpp \
				$(INTERPRETER_DIR)/repl.cpp \
				$(INTERPRETER_DIR)/token.cpp

COMPILER_OBJ = $(COMPILER_SRC:.cpp=.o)
INTERPRETER_OBJ = $(INTERPRETER_SRC:.cpp=.o)

.PHONY: all clean compiler compiler-benchmark interpreter interpreter-benchmark

all: compiler compiler-benchmark interpreter interpreter-benchmark


compiler : $(COMPILER_EXE)

$(COMPILER_EXE) : $(COMPILER_OBJ) $(COMPILER_DIR)/monkey.o $(COMPILER_DIR)/repl.o
	$(CXX) $(CXXFLAGS) $^ -o $@

compiler-benchmark : $(COMPILER_BENCHMARK_EXE)

$(COMPILER_BENCHMARK_EXE) : $(COMPILER_OBJ) $(COMPILER_DIR)/benchmark.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(COMPILER_OBJ): $(COMPILER_INC)


interpreter : $(INTERPRETER_EXE)

$(INTERPRETER_EXE) : $(INTERPRETER_OBJ) $(INTERPRETER_DIR)/monkey.o $(INTERPRETER_DIR)/repl.o
	$(CXX) $(CXXFLAGS) $^ -o $@

interpreter-benchmark : $(INTERPRETER_BENCHMARK_EXE)

$(INTERPRETER_BENCHMARK_EXE) : $(INTERPRETER_OBJ) $(INTERPRETER_DIR)/benchmark.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(INTERPRETER_OBJ): $(INTERPRETER_INC)


clean :
	rm -rf $(COMPILER_DIR)/*.o $(COMPILER_EXE) $(COMPILER_BENCHMARK_EXE)
	rm -rf $(INTERPRETER_DIR)/*.o $(INTERPRETER_EXE) $(INTERPRETER_BENCHMARK_EXE)
