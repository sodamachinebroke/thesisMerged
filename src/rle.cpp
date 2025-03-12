#include "../lib/rle.h"

std::vector<std::pair<uint8_t, uint8_t> > compress(const std::vector<uint8_t> &data) {
    if (data.empty()) throw std::runtime_error("Data is empty");

    std::vector<std::pair<uint8_t, uint8_t> > compressed;
    uint8_t current_byte = data[0];
    uint8_t count = 1;

    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i] == current_byte) {
            if (++count == 255) {
                compressed.emplace_back(current_byte, count);
                count = 0;
            }
        } else {
            compressed.emplace_back(current_byte, count);
            current_byte = data[i];
            count = 1;
        }
    }
    compressed.emplace_back(current_byte, count);
    return compressed;
}

std::vector<uint8_t> decompress(const std::vector<std::pair<uint8_t, uint8_t> > &compressed) {
    std::vector<uint8_t> decompressed;
    for (const auto &[byte, count]: compressed) {
        decompressed.insert(decompressed.end(), count, byte);
    }
    return decompressed;
}