#ifndef RLE_H
#define RLE_H

#include <cstdint>
#include <stdexcept>
#include <vector>
#include "compress.h"

namespace compress {
  class RLECompressor {
  public:
    std::vector<uint8_t> compress(const std::vector<uint8_t> &data);

    std::vector<uint8_t> decompress(const std::vector<uint8_t> &compressedData);
  };
} // namespace compress

#endif // RLE_H
