#ifndef MONKEY_PLUSPLUS_FRAME_HPP
#define MONKEY_PLUSPLUS_FRAME_HPP

#include "code.hpp"
#include "object.hpp"

struct Frame {
    Frame() = delete;

    Frame(const Frame& other) = default;

    Frame(Frame&& other) noexcept = default;

    Frame& operator=(const Frame& other) = default;

    Frame& operator=(Frame&& other) noexcept = default;

    Frame(std::shared_ptr<Closure> cl, int ip, int base_pointer);

    std::shared_ptr<Closure> cl;

    int ip;

    int base_pointer;

    Instructions instructions();
};

std::shared_ptr<Frame> new_frame(std::shared_ptr<Closure> cl, int base_pointer);

#endif //MONKEY_PLUSPLUS_FRAME_HPP
