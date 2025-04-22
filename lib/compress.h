#ifndef COMPRESS_H
#define COMPRESS_H

#include <cstdint>
#include <fstream>
#include <memory>
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
  std::unique_ptr<Compressor> createRLECompressor();

  std::unique_ptr<Compressor> createLZWCompressor();

  std::unique_ptr<Compressor> createHuffmanCompressor();

  void compressFile(const std::string &filePath, const std::unique_ptr<Compressor> &compressor);
} // namespace compress

#endif // COMPRESS_H
