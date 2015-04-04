#ifndef QP_COLORS_H_
#define QP_COLORS_H_

#include <iostream>
#include <string>

namespace qap {

namespace color {

const std::string reset = "\033[0m";
const std::string red =   "\033[31m";
const std::string green = "\033[32m";

// Just a little helper class for writing colored output
// to the terminal
struct writer {
    std::ostream& out_;
    writer(std::ostream& out) : out_(out) {};
    template <typename T>
    std::ostream& operator ()(T const& message, std::string const& color) {
        out_ << color << message << qap::color::reset;
        return out_;
    }
};


} // color namespace

} // qap namespace

#endif // include guard
