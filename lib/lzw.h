#ifndef LZW_H
#define LZW_H

#include "compress.h"
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>
#include <stdexcept>

namespace compress {
    struct VectorHash {
        size_t operator()(const std::vector<uint8_t> &v) const {
            std::hash<uint8_t> hasher;
            size_t seed = 0;
            for (uint8_t i: v) {
                seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };

    class LZWCompressor : public Compressor {
    public:
        std::vector<uint8_t> compress(const std::vector<uint8_t> &data) override;

        std::vector<uint8_t> decompress(const std::vector<uint8_t> &compressedData) override;
    };
}

#endif // LZW_H
