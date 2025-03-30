#ifndef COMPRESS_H
#define COMPRESS_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

namespace compress {
  class Compressor {
  public:
    virtual ~Compressor() = default;

    virtual std::vector<uint8_t> compress(const std::vector<uint8_t> &data) = 0;

    virtual std::vector<uint8_t> decompress(const std::vector<uint8_t> &compressedData) = 0;
  };

  // Forward declarations of compressors
  class RLECompressor;
  class LZWCompressor;
  class HuffmanCompressor;


  // Factory functions
  Compressor *createRLECompressor();

  Compressor *createLZWCompressor();

  Compressor *createHuffmanCompressor();

  void compressFile(const std::string &filePath, Compressor *compressor);
} // namespace compress

#endif // COMPRESS_H
