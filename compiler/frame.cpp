#include "frame.hpp"

Frame::Frame(std::shared_ptr<Closure> cl, int ip, int base_pointer) :
    cl{cl}, ip{ip}, base_pointer{base_pointer} {}

Instructions Frame::instructions() {
    return cl->fn->instructions;
}

std::shared_ptr<Frame> new_frame(std::shared_ptr<Closure> cl, int base_pointer) {
    return std::make_shared<Frame>(Frame(cl, -1, base_pointer));
}
