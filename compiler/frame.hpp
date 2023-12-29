#ifndef MONKEY_PLUSPLUS_FRAME_HPP
#define MONKEY_PLUSPLUS_FRAME_HPP

#include "code.hpp"
#include "object.hpp"

struct Frame {
    Frame(std::shared_ptr<CompiledFunction> fn, int ip);

    std::shared_ptr<CompiledFunction> fn;

    int ip;

    Instructions instructions();
};

std::shared_ptr<Frame> new_frame(std::shared_ptr<CompiledFunction> fn);

#endif //MONKEY_PLUSPLUS_FRAME_HPP
