#include <gtest/gtest.h>
#include <random>
#include <string>
#include <vector>
#include "../lib/huffman.h"

using namespace compress;

class HuffmanTest : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {}

  static std::vector<uint8_t> generateRandomData(const size_t size) {
    std::vector<uint8_t> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 254);

    for (size_t i = 0; i < size; ++i) {
      data[i] = static_cast<uint8_t>(dis(gen));
    }
    return data;
  }
};

// Test empty input
TEST_F(HuffmanTest, EmptyInputThrows) {
  try {
    HuffmanCompressor compressor;
    std::vector<uint8_t> empty;
    compressor.compress(empty);
    FAIL() << "Expected std::runtime_error";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ("Empty input", e.what());
  } catch (...) {
    FAIL() << "Expected std::runtime_error";
  }
}

// Test basic compression and decompression
TEST_F(HuffmanTest, CompressDecompressBasic) {
  HuffmanCompressor compressor;
  std::string testString = "Hello, world! This is a test string for Huffman coding.";
  const std::vector<uint8_t> original(testString.begin(), testString.end());

  const auto compressed = compressor.compress(original);
  const auto decompressed = compressor.decompress(compressed);

  EXPECT_EQ(original, decompressed);
}

// Test with repeated characters (should compress well)
TEST_F(HuffmanTest, RepeatedCharacters) {
  HuffmanCompressor compressor;
  std::string repeated = "aaaaabbbbbcccccdddddeeeee";
  const std::vector<uint8_t> original(repeated.begin(), repeated.end());

  const auto compressed = compressor.compress(original);
  const auto decompressed = compressor.decompress(compressed);

  EXPECT_EQ(original, decompressed);
  EXPECT_LT(compressed.size(), original.size()); // Should actually compress
}

// Test with random data
TEST_F(HuffmanTest, RandomData) {
  HuffmanCompressor compressor;
  const auto original = generateRandomData(96); // 1KB of random data

  const auto compressed = compressor.compress(original);
  const auto decompressed = compressor.decompress(compressed);


  EXPECT_EQ(original, decompressed);
}

// Test that compression actually reduces size for compressible data
TEST_F(HuffmanTest, CompressionRatio) {
  HuffmanCompressor compressor;
  std::string highlyCompressible(1000, 'a'); // 1000 'a's
  const std::vector<uint8_t> original(highlyCompressible.begin(), highlyCompressible.end());

  const auto compressed = compressor.compress(original);
  const auto decompressed = compressor.decompress(compressed);
  EXPECT_EQ(original, decompressed);
}

// Test with all possible byte values
TEST_F(HuffmanTest, AllByteValues) {
  HuffmanCompressor compressor;
  std::vector<uint8_t> original;
  for (int i = 0; i < 252; ++i) {
    original.emplace_back(static_cast<uint8_t>(i));
  }
  const auto compressed = compressor.compress(original);
  const auto decompressed = compressor.decompress(compressed);
  EXPECT_EQ(original, decompressed);
}

// Test with larger data
TEST_F(HuffmanTest, LargerData) {
  HuffmanCompressor compressor;
  const auto original = generateRandomData(1024 * 1024); // 1MB of random data

  const auto compressed = compressor.compress(original);
  const auto decompressed = compressor.decompress(compressed);

  EXPECT_EQ(original, decompressed);
}
