#ifndef MEASURE_HEADER
#define MEASURE_HEADER

#include <iostream>
#include <lz4.h>
#include <lz4hc.h>
#include <zstd.h>
#include <chrono>
#include <string.h>


struct Result {
    size_t input_data_size;
    size_t compress_data_size;
    size_t decompress_data_size;
    double compress_time;
    double decompress_time;

    std::string error;

    Result(std::string err) {
        this->set_error(err);
    }

    Result(size_t input_data_size, size_t compress_data_size, size_t decompress_data_size, double compress_time, double decompress_time) {
        this->input_data_size = input_data_size;
        this->compress_data_size = compress_data_size;
        this->decompress_data_size = decompress_data_size;
        this->compress_time = compress_time;
        this->decompress_time = decompress_time;

        this->error = "";
    }

    void set_error(std::string error) {
        this->input_data_size = -1;
        this->compress_data_size = -1;
        this->decompress_data_size = -1;
        this->compress_time = -1;
        this->decompress_time = -1;

        this->error = error;
    }

    std::string to_string(std::string name) {
        std::string res = "Compression: " + name + "\n";
        if (this->error == "") {
            res += "Compression speed: " + std::to_string(get_compspeed()) + " MB/ms\n"
            + "Ratio: " + std::to_string(get_ratio()) + '\n'
            + "Decompression speed: " + std::to_string(get_decompspeed()) + " MB/ms\n";
        } else {
            res += this->error + '\n';
        }
        return res;
    }

    std::string to_csv_string(std::string name) {
        if (this->error != "") {
            return "";
        } else {
            return name + ',' + std::to_string(get_compspeed()) + ',' + std::to_string(get_ratio()) + ',' + std::to_string(get_decompspeed()) + '\n';
        }
    }
private:
    double get_ratio() {
        return (double)this->compress_data_size * 100 / (double)this->input_data_size;
    }

    double get_compspeed() {
        return ((double)input_data_size / compress_time)  * 60 / (1024.0 * 1024.0);
    }

    double get_decompspeed() {
        return ((double)compress_data_size / decompress_time)  * 60 / (1024.0 * 1024.0);
    }
};

Result lz4_level_measure(char* input_data, size_t input_data_size) {
    size_t max_compressed_size = LZ4_compressBound(input_data_size);
    char* lz4_compressed_data = new char[max_compressed_size];

    // Замер времени для сжатия алгоритмом LZ4
    auto start = std::chrono::high_resolution_clock::now();
    int compressed_size = LZ4_compress_default(input_data, lz4_compressed_data, input_data_size, max_compressed_size);
    std::chrono::duration<double, std::milli> compression = std::chrono::high_resolution_clock::now() - start;

    if (compressed_size <= 0) {
        delete[] lz4_compressed_data;
        return Result("Compression failed");
    }

    char* lz4_decompressed_data = new char[input_data_size];

    // Замер времени для расжатия алгоритмом LZ4
    start = std::chrono::high_resolution_clock::now();
    int decompressed_size = LZ4_decompress_safe(lz4_compressed_data, lz4_decompressed_data, compressed_size, input_data_size);
    std::chrono::duration<double, std::milli> decompression = std::chrono::high_resolution_clock::now() - start;

    delete[] lz4_compressed_data;
    delete[] lz4_decompressed_data;

    if (compressed_size <= 0) {
        return Result("Compression failed");
    }

    return Result(input_data_size, compressed_size, decompressed_size, compression.count(), decompression.count());

}

Result lz4hc_level_measure(char* input_data, size_t input_data_size, int level) {
    size_t max_compressed_size = LZ4_compressBound(input_data_size);
    char* lz4hc_compressed_data = new char[max_compressed_size];

    // Замер времени для сжатия алгоритмом LZ4HC
    auto start = std::chrono::high_resolution_clock::now();
    int compressed_size = LZ4_compress_HC(input_data, lz4hc_compressed_data, input_data_size, max_compressed_size, level);
    std::chrono::duration<double, std::milli> compression = std::chrono::high_resolution_clock::now() - start;

    if (compressed_size == 0) {
        delete[] lz4hc_compressed_data;
        return Result("Compression failed");
    }

    char* lz4hc_decompressed_data = new char[input_data_size];

    // Замер времени для расжатия алгоритмом LZ4HC
    start = std::chrono::high_resolution_clock::now();
    int decompressed_size = LZ4_decompress_safe(lz4hc_compressed_data, lz4hc_decompressed_data, compressed_size, input_data_size);
    std::chrono::duration<double, std::milli> decompression = std::chrono::high_resolution_clock::now() - start;

    delete[] lz4hc_compressed_data;
    delete[] lz4hc_decompressed_data;

    if (compressed_size <= 0) {
        return Result("Compression failed");
    }

    return Result(input_data_size, compressed_size, decompressed_size, compression.count(), decompression.count());

}

Result zstd_level_measure(char* input_data, size_t input_data_size, int level) {
    size_t max_compressed_size = ZSTD_compressBound(input_data_size);
    char* zstd_compressed_data = new char[max_compressed_size];

    // Замер времени для сжатия алгоритмом ZSTD
    auto start = std::chrono::high_resolution_clock::now();
    size_t compressed_size = ZSTD_compress(zstd_compressed_data, max_compressed_size, input_data, input_data_size, level);
    std::chrono::duration<double, std::milli> compression = std::chrono::high_resolution_clock::now() - start;

    if (ZSTD_isError(compressed_size)) {
        delete[] zstd_compressed_data;
        return Result("Compression failed" + std::string(ZSTD_getErrorName(compressed_size)));
    }

    unsigned long long max_decompressed_size = ZSTD_getFrameContentSize(zstd_compressed_data, compressed_size);
    char* zstd_decompressed_data = new char[max_decompressed_size];

    // Замер времени для расжатия алгоритмом ZSTD
    start = std::chrono::high_resolution_clock::now();
    int decompressed_size = ZSTD_decompress(zstd_decompressed_data, max_decompressed_size, zstd_compressed_data, compressed_size);
    std::chrono::duration<double, std::milli> decompression = std::chrono::high_resolution_clock::now() - start;

    delete[] zstd_compressed_data;
    delete[] zstd_decompressed_data;

    if (ZSTD_isError(compressed_size)) {
        return Result("Compression failed" + std::string(ZSTD_getErrorName(compressed_size)));
    }

    return Result(input_data_size, compressed_size, decompressed_size, compression.count(), decompression.count());

}
#endif