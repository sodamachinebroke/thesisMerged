#ifndef LZW_H
#define LZW_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include "compress.h"

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

  class LZWCompressor {
  public:
    std::vector<uint8_t> compress(const std::vector<uint8_t> &data);
    std::vector<uint8_t> decompress(const std::vector<uint8_t> &compressedData);

  private:
    static constexpr uint16_t DICT_RESET_CODE = 256;
    static constexpr uint16_t MAX_DICT_SIZE = 65535; // 2^16 - 1

    static void resetDictionary(std::unordered_map<std::vector<uint8_t>, uint16_t, VectorHash> &dict);
    static void resetDictionary(std::unordered_map<uint16_t, std::vector<uint8_t>> &dict);
  };
} // namespace compress

#endif // LZW_H
