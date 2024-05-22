#include <iostream>
#include <fstream>

#include "measure.hpp" 

void update_result(unsigned int max) {
    static unsigned int lv = 0, old_string= 0;

    std::string lv_str = std::to_string(lv) + '/' + std::to_string(max);
    
    for (int i = 0; i < old_string; ++i) {
        std::cout << '\r';
    }
    std::cout << std::to_string(lv).size();
    ++lv;
    old_string = lv_str.size();
}

std::string getFile(std::string fileName) {
    std::ifstream in(fileName);
    std::string in_str;

    while(!in.eof()) {
        char c = in.get();
        in_str += c;
    }
    in.close();

    return in_str;
}

int main(int argc, char** argv) {
    // Создаем исходные данные для сжатия
    std::string input_file;

    std::ofstream res_out ("results.txt");
    std::ofstream csv_out ("res.csv");
    csv_out << "name,level,compression time,decompression time,ratio\n";

    std::cout << "Input file: ";
    std::cin >> input_file;
    std::string in = getFile(input_file);
    long long input_data_size = in.size();
    char* input_data = new char[input_data_size];
    strcpy(input_data, in.c_str());
    std::cout << "Input file size: " << input_data_size << " bytes" << std::endl;

    
    Result result = lz4_level_measure(input_data, input_data_size);

    res_out << result.to_string("LZ4 ") << std::endl;
    csv_out << result.to_csv_string("LZ4,1");

    for (int level = 1; level <= 12; ++level) {
        Result result = lz4hc_level_measure(input_data, input_data_size, level);

        res_out << result.to_string("LZ4HC -" + std::to_string(level)) << std::endl;
        csv_out << result.to_csv_string("LZ4HC," + std::to_string(level));
    }

    for (int level = ZSTD_minCLevel(); level < ZSTD_defaultCLevel(); ++level) {
        Result result = zstd_level_measure(input_data, input_data_size, level);

        res_out << result.to_string("ZSTD -" + std::to_string(level)) << std::endl;
        csv_out << result.to_csv_string("ZSTD," + std::to_string(level));
    }

    res_out.close();
    csv_out.close();

    delete[] input_data;

    return 0;
}