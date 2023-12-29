#include "frame.hpp"

Frame::Frame(std::shared_ptr<CompiledFunction> fn, int ip) : fn{fn}, ip{ip} {}

Instructions Frame::instructions() {
    return fn->instructions;
}

std::shared_ptr<Frame> new_frame(std::shared_ptr<CompiledFunction> fn) {
    return std::make_shared<Frame>(Frame(fn, -1));
}
