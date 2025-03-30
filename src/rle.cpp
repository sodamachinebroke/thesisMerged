#include "../lib/rle.h"

namespace compress {

std::vector<uint8_t> RLECompressor::compress(const std::vector<uint8_t>& data) {
    if (data.empty()) {
        throw std::runtime_error("Data is empty");
    }

    std::vector<uint8_t> compressed;
    if (data.empty()) return compressed;

    std::vector<std::pair<uint8_t, uint8_t>> tempCompressed;
    uint8_t current_byte = data[0];
    uint8_t count = 1;

    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i] == current_byte) {
            if (++count == 255) {
                tempCompressed.emplace_back(current_byte, count);
                count = 0;
            }
        } else {
            tempCompressed.emplace_back(current_byte, count);
            current_byte = data[i];
            count = 1;
        }
    }
    tempCompressed.emplace_back(current_byte, count);

    for (const auto& pair : tempCompressed) {
        compressed.push_back(pair.first);
        compressed.push_back(pair.second);
    }

    return compressed;
}

std::vector<uint8_t> RLECompressor::decompress(const std::vector<uint8_t>& compressedData) {
    std::vector<uint8_t> decompressed;
    if (compressedData.size() % 2 != 0) {
        throw std::runtime_error("Invalid compressed data format");
    }

    for (size_t i = 0; i < compressedData.size(); i += 2) {
        uint8_t byte = compressedData[i];
        uint8_t count = compressedData[i + 1];
        decompressed.insert(decompressed.end(), count, byte);
    }
    return decompressed;
}

}