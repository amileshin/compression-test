#ifndef CUSTOM_EXCEPTION_HEADER_FILE
#define CUSTOM_EXCEPTION_HEADER_FILE

#include <exception>
#include <stdexcept>
#include <string>

namespace compression_test_exceptions {
    class invalid_flag_argument : public std::invalid_argument {
    public:
        explicit invalid_flag_argument (std::string flag, std::string param) : invalid_argument("Invalid argument " + param + " for flag " + flag) {};
    };

    class flag_extra_argument : public std::invalid_argument {
    public:
        explicit flag_extra_argument (std::string flag) : invalid_argument("Flag " + flag + " already used") {};
    };
};

#endif