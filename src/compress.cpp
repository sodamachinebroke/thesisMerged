#include "../lib/compress.h"
#include "../lib/huffman.h"
#include "../lib/lzw.h"
#include "../lib/rle.h"

namespace compress {

  void compressFile(const std::string &filePath, Compressor *compressor) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
      std::cerr << "Error: Unable to open file " << filePath << std::endl;
      return;
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> fileData(fileSize);
    file.read(reinterpret_cast<char *>(fileData.data()), fileSize);
    file.close();

    std::vector<unsigned char> compressedData = compressor->compress(fileData);

    std::string compressedFilePath = filePath + ".compressed";
    std::ofstream compressedFile(compressedFilePath, std::ios::binary);
    if (!compressedFile.is_open()) {
      std::cerr << "Error: Unable to create compressed file " << compressedFilePath << std::endl;
      return;
    }

    compressedFile.write(reinterpret_cast<char *>(compressedData.data()), compressedData.size());
    compressedFile.close();

    std::cout << "File compressed successfully: " << compressedFilePath << std::endl;
  }

  Compressor *createRLECompressor() { return new RLECompressor(); }

  Compressor *createLZWCompressor() { return new LZWCompressor(); }

  Compressor *createHuffmanCompressor() { return new HuffmanCompressor(); }

} // namespace compress
