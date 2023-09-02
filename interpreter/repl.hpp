#ifndef MONKEY_PLUSPLUS_REPL_HPP
#define MONKEY_PLUSPLUS_REPL_HPP

#include <vector>
#include <string>

static const std::string PROMPT{">> "};

static const std::string MONKEY_FACE{R"(            __,__
   .--.  .-"     "-.  .--.
  / .. \/  .-. .-.  \/ .. \
 | |  '|  /   Y   \  |'  | |
 | \   \  \ 0 | 0 /  /   / |
  \ '- ,\.-"""""""-./, -' /
   ''-' /_   ^ ^   _\ '-''
       |  \._   _./  |
       \   \ '~' /   /
        '._ '-=-' _.'
           '-----')"};

void start_repl();

void print_parser_errors(std::vector<std::string> errors);

#endif //MONKEY_PLUSPLUS_REPL_HPP
