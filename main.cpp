#include <fstream>
#include <iostream>
#include <vector>
#include "./lib/compress.h"

int main() {
  /*const std::vector<uint8_t> data = {1, 2, 2, 2, 3, 4, 4, 5};

  // Test RLE
  compress::Compressor *rleCompressor = compress::createRLECompressor();
  const std::vector<uint8_t> rleCompressed = rleCompressor->compress(data);
  std::vector<uint8_t> rleDecompressed = rleCompressor->decompress(rleCompressed);
  std::cout << "RLE: Original size: " << data.size() << ", Compressed size: " << rleCompressed.size() << std::endl;
  delete rleCompressor;

  // Test LZW
  compress::Compressor *lzwCompressor = compress::createLZWCompressor();
  const std::vector<uint8_t> lzwCompressed = lzwCompressor->compress(data);
  std::vector<uint8_t> lzwDecompressed = lzwCompressor->decompress(lzwCompressed);
  std::cout << "LZW: Original size: " << data.size() << ", Compressed size: " << lzwCompressed.size() << std::endl;
  delete lzwCompressor;

  // Test Huffman
  compress::Compressor *huffmanCompressor = compress::createHuffmanCompressor();
  const std::vector<uint8_t> huffmanCompressed = huffmanCompressor->compress(data);
  std::vector<uint8_t> huffmanDecompressed = huffmanCompressor->decompress(huffmanCompressed);
  std::cout << "Huffman: Original size: " << data.size() << ", Compressed size: " << huffmanCompressed.size()
            << std::endl;
  delete huffmanCompressor;

  // Test file compression
  // std::string filePath = "../public/input4.bundle";
  // compress::compressFile(filePath, compress::createRLECompressor());
  std::cout << "File compression test completed." << std::endl;*/

  return 0;
}
