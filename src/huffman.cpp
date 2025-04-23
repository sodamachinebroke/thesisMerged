// huffman.cpp
#include "../lib/huffman.h"
#include <algorithm> // Include for std::ranges::sort
#include <bitset> // Include for std::bitset
#include <fstream>
#include <iostream>
#include <memory> // Include for smart pointers
#include <queue>
#include <stdexcept>

namespace compress {

  std::vector<uint8_t> HuffmanCompressor::compress(const std::vector<uint8_t> &data) {
    if (data.empty())
      throw std::runtime_error("Empty input");
    std::map<uint8_t, int> freq;
    for (uint8_t byte: data) {
      freq[byte]++;
    }
    if (freq.size() > 1) {
      buildHuffmanTree(freq);
    } else {
      codes.emplace(freq.find(data[0])->first, "0");
    }

    std::vector<std::pair<std::string, uint8_t>> invertedMap;
    for (const auto &[byte, code]: codes)
      invertedMap.emplace_back(code, byte);

    std::ranges::sort(invertedMap, [](const auto &a, const auto &b) { return a.first.length() < b.first.length(); });

    std::ofstream output("temp_compressed.bin", std::ios::binary);
    if (!output.is_open()) {
      throw std::runtime_error("Could not open temp_compressed.bin");
    }

    std::string encodedData = encode(data);
    output.put(encodedData.size() % 8);
    writeHeader(output, invertedMap);
    writeBits(output, encodedData);
    output.close();

    std::ifstream compressedFile("temp_compressed.bin", std::ios::binary | std::ios::ate);
    std::streamsize size = compressedFile.tellg();
    compressedFile.seekg(0, std::ios::beg);
    std::vector<uint8_t> compressedData(static_cast<size_t>(size)); // Use static_cast for size_t conversion
    compressedFile.read(reinterpret_cast<char *>(compressedData.data()), size); //  Consider using compressedData.data()
    compressedFile.close();
    return compressedData;
  }

  std::vector<uint8_t> HuffmanCompressor::decompress(const std::vector<uint8_t> &compressedData) {
    auto [codePairs, dataOffset, paddingBits] = decompressHeader(compressedData);
    std::vector<uint8_t> decompData = decompressData(compressedData, codePairs, dataOffset, paddingBits);
    return decompData;
  }

  void HuffmanCompressor::storeCodes(const std::shared_ptr<MinHeapNode> &root,
                                     const std::string &str) { // Use shared_ptr
    if (!root)
      return;
    if (!root->left && !root->right)
      codes[root->data] = str;
    storeCodes(root->left, str + "0");
    storeCodes(root->right, str + "1");
  }

  void HuffmanCompressor::buildHuffmanTree(std::map<uint8_t, int> &freq) {
    auto cmp = [](const std::shared_ptr<MinHeapNode> &l, const std::shared_ptr<MinHeapNode> &r) {
      return l->freq > r->freq;
    };
    std::priority_queue<std::shared_ptr<MinHeapNode>, std::vector<std::shared_ptr<MinHeapNode>>, decltype(cmp)> minHeap(
        cmp);

    for (auto &[byte, frequency]: freq) {
      minHeap.push(std::make_shared<MinHeapNode>(byte, frequency));
    }

    while (minHeap.size() > 1) {
      std::shared_ptr<MinHeapNode> left = minHeap.top();
      minHeap.pop();
      std::shared_ptr<MinHeapNode> right = minHeap.top();
      minHeap.pop();
      std::shared_ptr<MinHeapNode> top = std::make_shared<MinHeapNode>(0, left->freq + right->freq);
      top->left = left;
      top->right = right;
      minHeap.push(top);
    }
    if (!minHeap.empty()) {
      storeCodes(minHeap.top(), "");
    }
  }

  std::string HuffmanCompressor::encode(const std::vector<uint8_t> &data) {
    std::string encodedString;
    for (uint8_t byte: data) {
      encodedString += codes[byte];
    }
    return encodedString;
  }

  void HuffmanCompressor::writeBits(std::ofstream &output, const std::string &bits) {
    uint8_t currentByte = 0;
    int bitCount = 0;
    for (const char bit: bits) {
      currentByte = (currentByte << 1) | (bit == '1');
      bitCount++;
      if (bitCount == 8) {
        output.put(currentByte);
        currentByte = 0;
        bitCount = 0;
      }
    }
    if (bitCount > 0) {
      currentByte <<= (8 - bitCount);
      output.put(currentByte);
    }
  }

  void HuffmanCompressor::writeHeader(std::ofstream &output,
                                      const std::vector<std::pair<std::string, uint8_t>> &invertedMap) {
    std::map<size_t, size_t> lengthCounts;
    for (const auto &[code, byte]: invertedMap) {
      lengthCounts[code.length()]++;
    }
    output.put(static_cast<uint8_t>(invertedMap.size()));
    for (const auto &[length, count]: lengthCounts) {
      output.put(static_cast<uint8_t>(count));
      output.put(static_cast<uint8_t>(length));
      for (const auto &[code, byte]: invertedMap) {
        if (code.length() == length) {
          output.put(byte);
          writeBits(output, code);
        }
      }
    }
  }

  std::string HuffmanCompressor::b(uint8_t v) {
    std::string s(8, '0');
    for (int i = 0; i < 8; ++i)
      s[7 - i] = ((v >> i) & 1) + '0';
    return s;
  }

  void HuffmanCompressor::reconstructCodeMap(const std::vector<uint8_t> &compressedData,
                                             std::map<std::string, uint8_t> &codeMap) {
    size_t index = 0;
    while (index < compressedData.size()) {
      const uint8_t count = compressedData[index++];
      const uint8_t length = compressedData[index++];
      for (int i = 0; i < count; ++i) {
        if (index >= compressedData.size())
          throw std::runtime_error("Invalid compressed data format.");
        uint8_t byte = compressedData[index++];
        if (index >= compressedData.size())
          throw std::runtime_error("Invalid compressed data format.");
        uint8_t codeByte = compressedData[index++];
        std::string code;
        for (int j = 0; j < length; ++j) {
          code += (codeByte & (1 << (7 - j))) ? '1' : '0';
        }
        code = code.substr(0, length);
        codeMap[code] = byte;
      }
    }
  }

  std::vector<uint8_t> HuffmanCompressor::decompressData(const std::vector<uint8_t> &compressedData,
                                                         const std::vector<std::pair<uint8_t, std::string>> &codePairs,
                                                         size_t dataOffset, const int paddingBits) {
    std::string bitString = "";
    for (size_t i = dataOffset; i < compressedData.size(); ++i) {
      uint8_t byte = compressedData[i];
      bitString += b(byte);
    }
    if (paddingBits > 0) {
      bitString = bitString.substr(0, bitString.size() - paddingBits);
    }
    std::vector<uint8_t> decodedData;
    std::string currentCode;
    size_t bitIndex = 0;
    while (bitIndex < bitString.size()) {
      currentCode += bitString[bitIndex];
      for (const auto &pair: codePairs) {
        if (pair.second == currentCode) {
          decodedData.push_back(pair.first);
          currentCode.clear();
          break;
        }
      }
      bitIndex++;
    }
    return decodedData;
  }

  bool HuffmanCompressor::compareFiles(const std::string &file1, const std::string &file2) {
    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);
    if (!f1.is_open() || !f2.is_open()) {
      throw std::runtime_error("Could not open one of the files for comparison.");
    }
    std::vector<uint8_t> data1((std::istreambuf_iterator<char>(f1)), std::istreambuf_iterator<char>());
    std::vector<uint8_t> data2((std::istreambuf_iterator<char>(f2)), std::istreambuf_iterator<char>());
    if (data1.size() != data2.size()) {
      return false;
    }
    for (size_t i = 0; i < data1.size(); ++i) {
      if (data1[i] != data2[i]) {
        return false;
      }
    }
    return true;
  }

  DecompressionInfo HuffmanCompressor::decompressHeader(const std::vector<uint8_t> &compressed) {
    std::vector<std::pair<uint8_t, std::string>> decompCodes;
    const int padding = compressed[0];
    const int mapLength = compressed[1];
    int i = 2;
    while (decompCodes.size() < mapLength) {
      const int numCodes = compressed[i];
      const int codeLength = compressed[i + 1];
      for (int j = 0; j < numCodes; ++j) {
        const int codeIndex = i + 2 + j * 2;
        const uint8_t symbolByte = compressed[codeIndex + 1];
        std::bitset<8> symbolBits(symbolByte);
        std::string symbol = symbolBits.to_string().substr(0, codeLength); // changed to use substr
        uint8_t symbolValue = compressed[codeIndex];
        decompCodes.emplace_back(symbolValue, symbol);
      }
      i += 2 + numCodes * 2;
    }
    return {decompCodes, static_cast<size_t>(i), padding};
  }
} // namespace compress
