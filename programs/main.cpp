#include <map>
#include <vector>
#include <array>
#include <set>
#include <string>

#include "custom_exceptions.hpp"

#include <iostream>

//TODO: переделать логику на обработку флагов по паттерну -<flag>=<value>

//TODO: придумать как адекватно показать что флаг был упомянут
class flag {
private:
    std::string flag_name;
    std::string default_value;
    std::vector<std::string> values;
    size_t limit;
    bool added;
public:
    flag(std::string flag_name): flag_name(flag_name), default_value(""), limit(0) {}

    flag(std::string flag_name, size_t limit = 1): flag_name(flag_name), default_value(""), limit(0) {
        if (this->limit > 0) {
            // this->values.reserve(this->limit);
        }
        this->added = false;
    }

    flag(std::string flag_name, std::string default_value, size_t limit = 1): flag_name(flag_name), default_value(default_value), limit(0) {
        if (this->limit > 0) {
            // this->values.reserve(this->limit);
        }
        this->added = false;
    }

    void add() {
        this->added = true;
    }

    bool is_add() {
        return this->added;
    }

    std::string get_name() {
        return this->flag_name;
    }

    void add_value (std::string value) {
        this->add();
        if (this->values.size() < this->limit || this->limit < 0) {
            this->values.push_back(value);
        } else {
            throw compression_test_exceptions::flag_extra_argument(this->flag_name);
        }
    }

    std::vector<std::string> get_values() {
        if (this->values.size() >= 1) {
            return this->values;
        } else if (this->default_value != "" && this->added) {
            return {this->default_value};
        } else {
            return std::vector<std::string>();
        }
    }

};

std::array<flag, 3> flags = {
    flag("-input_files",  -1),
    flag("-analyze", "result.csv",  1),
    flag("-csv", "result.csv",  1)
};

void load_flags (int argc, char** argv) {
    std::string default_flag = "-input_files";
    std::map<std::string, int> flag_names_to_ittr;
    for (int i = 0; i < flags.max_size(); ++i) {
        flag_names_to_ittr[flags[i].get_name()] = i;
    }

    
    int i = 0;
    while (i < argc) {
        if (argv[i][0] == '-') {
            if (flag_names_to_ittr.find(argv[i]) == flag_names_to_ittr.end() || argv[i] == default_flag) {
                std::cerr << "uknow flag " << argv[i] << std::endl;
            }
            ++i;
            if (i < argc && argv[i][0] != '-') {
                flags[flag_names_to_ittr[argv[i - 1]]].add_value(argv[i]);
            } else {
                flags[flag_names_to_ittr[argv[i - 1]]].add();
                continue;
            }
        } else {
            flags[flag_names_to_ittr[default_flag]].add_value(argv[i]);
        }
    }
}

void load(int argc, char** argv) {
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << ' ';
    }
    std::cout << '\n' << std::endl;

    load_flags(argc, argv);

    for (auto flag : flags) {
        if (flag.is_add()) {
            std::cout << flag.get_name() << '(' << flag.is_add() << "): ";
            for (std::string val : flag.get_values()) {
                std::cout << val << ' ';
            }
            std::cout << '\n';
        } else {
            std::cout << flag.get_name() << '\n';
        }
    }
}

int main(int argc, char** argv) {
    load(argc -1, argv + 1);

    return 0;
}