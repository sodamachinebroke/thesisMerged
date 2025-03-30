#include "../lib/lzw.h"

namespace compress {


std::vector<uint8_t> LZWCompressor::compress(const std::vector<uint8_t>& input) {
    std::unordered_map<std::vector<uint8_t>, unsigned int, VectorHash> table;
    for (unsigned int i = 0; i <= 255; i++) {
        table[{static_cast<uint8_t>(i)}] = i;
    }

    std::vector<uint8_t> p;
    unsigned int code = 256;
    std::vector<unsigned int> output;

    for (uint8_t c: input) {
        std::vector<uint8_t> pc = p;
        pc.push_back(c);
        if (table.contains(pc)) {
            p = pc;
        } else {
            output.push_back(table[p]);
            table[pc] = code++;
            p = {c};
        }
    }
    if (!p.empty()) {
        output.push_back(table[p]);
    }

    std::vector<uint8_t> compressed;
    for (unsigned int val : output) {
        compressed.push_back(static_cast<uint8_t>((val >> 24) & 0xFF));
        compressed.push_back(static_cast<uint8_t>((val >> 16) & 0xFF));
        compressed.push_back(static_cast<uint8_t>((val >> 8) & 0xFF));
        compressed.push_back(static_cast<uint8_t>(val & 0xFF));
    }
    return compressed;
}

std::vector<uint8_t> LZWCompressor::decompress(const std::vector<uint8_t>& compressedData) {
    std::vector<unsigned int> output;

    if (compressedData.size() % 4 != 0) {
        throw std::runtime_error("Invalid compressed data format");
    }

    for (size_t i = 0; i < compressedData.size(); i += 4) {
        unsigned int val = (static_cast<unsigned int>(compressedData[i]) << 24) |
                           (static_cast<unsigned int>(compressedData[i + 1]) << 16) |
                           (static_cast<unsigned int>(compressedData[i + 2]) << 8) |
                           static_cast<unsigned int>(compressedData[i + 3]);
        output.push_back(val);
    }

    std::unordered_map<unsigned int, std::vector<uint8_t> > table;
    for (unsigned int i = 0; i <= 255; i++) {
        table[i] = {static_cast<uint8_t>(i)};
    }

    if (output.empty()) {
        return {};
    }

    std::vector<uint8_t> decoded;
    std::vector<uint8_t> p = table[output[0]];
    decoded.insert(decoded.end(), p.begin(), p.end());
    unsigned int code = 256;

    for (size_t i = 1; i < output.size(); i++) {
        std::vector<uint8_t> current;
        if (table.contains(output[i])) {
            current = table[output[i]];
        } else if (output[i] == code) {
            current = p;
            current.push_back(p[0]);
        } else {
            return {};
        }

        decoded.insert(decoded.end(), current.begin(), current.end());
        std::vector<uint8_t> pc = p;
        pc.insert(pc.end(), current.begin(), current.begin() + 1);
        table[code++] = pc;
        p = current;
    }

    return decoded;
}

}