#pragma once

#define BESM_UTIL_DUMMY_EXCEPTION(type)                                        \
    class type : std::runtime_error {                                          \
    public:                                                                    \
        type(std::string const &message) : std::runtime_error(message) {}      \
        type(char const *message) : std::runtime_error(message) {}             \
    };
