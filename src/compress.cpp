#include "../lib/compress.h"
#include <iostream>
#include <memory>
#include "../lib/huffman.h"
#include "../lib/lzw.h"
#include "../lib/rle.h"

namespace compress {

  void compressFile(const std::string &filePath, const std::unique_ptr<Compressor> &compressor) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
      std::cerr << "Error: Unable to open file " << filePath << std::endl;
      return;
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> fileData(fileSize);
    if (!fileData.empty()) {
      file.read(static_cast<char *>(static_cast<void *>(fileData.data())), fileSize);
    }
    file.close();

    std::vector<unsigned char> compressedData = compressor->compress(fileData);

    std::string compressedFilePath = filePath + ".compressed";
    std::ofstream compressedFile(compressedFilePath, std::ios::binary);
    if (!compressedFile.is_open()) {
      std::cerr << "Error: Unable to create compressed file " << compressedFilePath << std::endl;
      return;
    }

    if (!compressedData.empty()) {
      compressedFile.write(static_cast<const char *>(static_cast<const void *>(compressedData.data())),
                           compressedData.size());
    }
    compressedFile.close();

    std::cout << "File compressed successfully: " << compressedFilePath << std::endl;
  }

  std::unique_ptr<Compressor> createRLECompressor() { return std::make_unique<RLECompressor>(); }

  std::unique_ptr<Compressor> createLZWCompressor() { return std::make_unique<LZWCompressor>(); }

  std::unique_ptr<Compressor> createHuffmanCompressor() { return std::make_unique<HuffmanCompressor>(); }

} // namespace compress
