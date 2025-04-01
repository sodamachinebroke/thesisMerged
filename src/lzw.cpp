#include "lzw.h"
#include <stdexcept>

namespace compress {

  void LZWCompressor::resetDictionary(std::unordered_map<std::vector<uint8_t>, uint16_t, VectorHash> &dict) {
    dict.clear();
    for (uint16_t i = 0; i <= 255; i++) {
      dict[{static_cast<uint8_t>(i)}] = i;
    }
  }

  void LZWCompressor::resetDictionary(std::unordered_map<uint16_t, std::vector<uint8_t>> &dict) {
    dict.clear();
    for (uint16_t i = 0; i <= 255; i++) {
      dict[i] = {static_cast<uint8_t>(i)};
    }
  }

  std::vector<uint8_t> LZWCompressor::compress(const std::vector<uint8_t> &input) {
    if (input.empty())
      throw std::invalid_argument("Empty input");

    std::unordered_map<std::vector<uint8_t>, uint16_t, VectorHash> dictionary;
    resetDictionary(dictionary);

    std::vector<uint8_t> current;
    uint16_t next_code = DICT_RESET_CODE + 1;
    std::vector<uint16_t> codes;

    for (uint8_t c: input) {
      std::vector<uint8_t> next = current;
      next.push_back(c);

      if (dictionary.count(next)) {
        current = next;
      } else {
        codes.push_back(dictionary[current]);

        // Add new entry and handle dictionary full condition
        if (next_code < MAX_DICT_SIZE) {
          dictionary[next] = next_code++;
        } else {
          codes.push_back(DICT_RESET_CODE);
          resetDictionary(dictionary);
          next_code = DICT_RESET_CODE + 1;
        }

        current = {c};
      }
    }

    if (!current.empty()) {
      codes.push_back(dictionary[current]);
    }

    // Pack 16-bit codes into bytes
    std::vector<uint8_t> compressed;
    for (uint16_t code: codes) {
      compressed.push_back(static_cast<uint8_t>(code >> 8));
      compressed.push_back(static_cast<uint8_t>(code & 0xFF));
    }

    return compressed;
  }

  std::vector<uint8_t> LZWCompressor::decompress(const std::vector<uint8_t> &compressedData) {
    if (compressedData.empty())
      throw std::invalid_argument("Empty input");
    if (compressedData.size() % 2 != 0)
      throw std::invalid_argument("Odd number of bytes");

    // Reconstruct 16-bit codes
    std::vector<uint16_t> codes;
    for (size_t i = 0; i < compressedData.size(); i += 2) {
      uint16_t code = (compressedData[i] << 8) | compressedData[i + 1];
      codes.push_back(code);
    }

    std::unordered_map<uint16_t, std::vector<uint8_t>> dictionary;
    resetDictionary(dictionary);

    std::vector<uint8_t> output;
    uint16_t next_code = DICT_RESET_CODE + 1;

    if (codes.empty())
      return output;

    // Process first code
    std::vector<uint8_t> previous = dictionary[codes[0]];
    output.insert(output.end(), previous.begin(), previous.end());

    for (size_t i = 1; i < codes.size(); i++) {
      std::vector<uint8_t> current;

      if (codes[i] == DICT_RESET_CODE) {
        resetDictionary(dictionary);
        next_code = DICT_RESET_CODE + 1;
        previous.clear();
        continue;
      }

      if (dictionary.count(codes[i])) {
        current = dictionary[codes[i]];
      } else if (codes[i] == next_code) {
        current = previous;
        if (!previous.empty()) {
          current.push_back(previous[0]);
        }
      } else {
        throw std::runtime_error("Invalid compressed data");
      }

      output.insert(output.end(), current.begin(), current.end());

      // Add new dictionary entry
      if (!previous.empty() && next_code < MAX_DICT_SIZE) {
        std::vector<uint8_t> new_entry = previous;
        if (!current.empty()) {
          new_entry.push_back(current[0]);
        }
        dictionary[next_code++] = new_entry;
      }

      previous = current;
    }

    return output;
  }

} // namespace compress
