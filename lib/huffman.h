#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <string>
#include <bitset>
#include <algorithm>
#include "compress.h"

namespace compress {

	struct MinHeapNode {
    	uint8_t data;
    	int freq;
    	MinHeapNode *left, *right;

    	MinHeapNode(uint8_t data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
	};

	struct CompareMinHeapNode {
    	bool operator()(const MinHeapNode *l, const MinHeapNode *r) const { return l->freq > r->freq; }
	};

	struct DecompressionInfo {
    	std::vector<std::pair<uint8_t, std::string>> codePairs;
    	size_t dataOffset;
    	int paddingBits;
	};

	class HuffmanCompressor : public Compressor {
	public:
    	std::vector<uint8_t> compress(const std::vector<uint8_t>& data) override;
    	std::vector<uint8_t> decompress(const std::vector<uint8_t>& compressedData) override;

	private:
    	std::map<uint8_t, std::string> codes;

    	void storeCodes(const MinHeapNode *root, std::string str);
    	void buildHuffmanTree(std::map<uint8_t, int> &freq);
    	std::string encode(const std::vector<uint8_t> &data);
    	void writeBits(std::ofstream &output, const std::string &bits);
    	void writeHeader(std::ofstream &output, const std::vector<std::pair<std::string, uint8_t>> &invertedMap);
    	std::string b(uint8_t v);
    	void reconstructCodeMap(const std::vector<uint8_t> &compressedData, std::map<std::string, uint8_t> &codeMap);
    	std::vector<uint8_t> decompressData(const std::vector<uint8_t> &compressedData,
                                        const std::vector<std::pair<uint8_t, std::string>> &codePairs, size_t dataOffset,
                                        int paddingBits);
    	bool compareFiles(const std::string &file1, const std::string &file2);
    	DecompressionInfo decompressHeader(const std::vector<unsigned char> &compressed);
	};

}

#endif // HUFFMAN_H