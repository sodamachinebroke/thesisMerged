#include <fstream>
#include <iostream>
#include <vector>
#include "./lib/compress.h"

int main() {
  const std::vector<unsigned char> data = {1, 2, 2, 2, 3, 4, 4, 5};

  // Test RLE
  compress::Compressor *rleCompressor = compress::createRLECompressor();
  std::vector<unsigned char> rleCompressed = rleCompressor->compress(data);
  std::vector<unsigned char> rleDecompressed = rleCompressor->decompress(rleCompressed);
  std::cout << "RLE: Original size: " << data.size() << ", Compressed size: " << rleCompressed.size() << std::endl;
  delete rleCompressor;

  // Test LZW
  compress::Compressor *lzwCompressor = compress::createLZWCompressor();
  std::vector<unsigned char> lzwCompressed = lzwCompressor->compress(data);
  std::vector<unsigned char> lzwDecompressed = lzwCompressor->decompress(lzwCompressed);
  std::cout << "LZW: Original size: " << data.size() << ", Compressed size: " << lzwCompressed.size() << std::endl;
  delete lzwCompressor;

  // Test Huffman
  compress::Compressor *huffmanCompressor = compress::createHuffmanCompressor();
  std::vector<unsigned char> huffmanCompressed = huffmanCompressor->compress(data);
  std::vector<unsigned char> huffmanDecompressed = huffmanCompressor->decompress(huffmanCompressed);
  std::cout << "Huffman: Original size: " << data.size() << ", Compressed size: " << huffmanCompressed.size()
            << std::endl;
  delete huffmanCompressor;

  // Test file compression
  // std::string filePath = "../public/input4.bundle";
  // compress::compressFile(filePath, compress::createRLECompressor());
  std::cout << "File compression test completed." << std::endl;

  return 0;
}
