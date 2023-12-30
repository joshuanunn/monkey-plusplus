#include "frame.hpp"

Frame::Frame(std::shared_ptr<CompiledFunction> fn, int ip, int base_pointer) :
    fn{fn}, ip{ip}, base_pointer{base_pointer} {}

Instructions Frame::instructions() {
    return fn->instructions;
}

std::shared_ptr<Frame> new_frame(std::shared_ptr<CompiledFunction> fn, int base_pointer) {
    return std::make_shared<Frame>(Frame(fn, -1, base_pointer));
}
