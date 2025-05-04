#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <algorithm>
#include <bitset>
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "compress.h"

namespace compress {

  struct MinHeapNode {
    uint8_t data;
    int freq;
    std::shared_ptr<MinHeapNode> left, right;

    MinHeapNode(const uint8_t data, const int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
  };

  struct CompareMinHeapNode {
    bool operator()(const std::shared_ptr<MinHeapNode> &l, const std::shared_ptr<MinHeapNode> &r) const {
      return l->freq > r->freq;
    }
  };

  struct DecompressionInfo {
    std::vector<std::pair<uint8_t, std::string>> codePairs;
    size_t dataOffset;
    int paddingBits;
  };

  class HuffmanCompressor final {
  public:
    std::vector<uint8_t> compress(const std::vector<uint8_t> &data);
    std::vector<uint8_t> decompress(const std::vector<uint8_t> &compressedData);

  private:
    std::map<uint8_t, std::string> codes;

    void storeCodes(const std::shared_ptr<MinHeapNode> &root, const std::string &str);
    void buildHuffmanTree(std::map<uint8_t, int> &freq);
    std::string encode(const std::vector<uint8_t> &data);
    void writeBits(std::ofstream &output, const std::string &bits);
    void writeHeader(std::ofstream &output, const std::vector<std::pair<std::string, uint8_t>> &invertedMap);
    std::string b(uint8_t v);
    void reconstructCodeMap(const std::vector<uint8_t> &compressedData, std::map<std::string, uint8_t> &codeMap);
    std::vector<uint8_t> decompressData(const std::vector<uint8_t> &compressedData,
                                        const std::vector<std::pair<uint8_t, std::string>> &codePairs,
                                        size_t dataOffset, int paddingBits);
    bool compareFiles(const std::string &file1, const std::string &file2);
    DecompressionInfo decompressHeader(const std::vector<uint8_t> &compressed);
  };

} // namespace compress

#endif // HUFFMAN_H
