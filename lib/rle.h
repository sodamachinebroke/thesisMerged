#ifndef RLE_H
#define RLE_H

#include "compress.h"
#include <vector>
#include <stdexcept>
#include <cstdint>

namespace compress {
    class RLECompressor : public Compressor {
    public:
        std::vector<uint8_t> compress(const std::vector<uint8_t> &data) override;

        std::vector<uint8_t> decompress(const std::vector<uint8_t> &compressedData) override;
    };
}

#endif // RLE_H
