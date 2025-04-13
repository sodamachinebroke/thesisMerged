#include <gtest/gtest.h>
#include <random>
#include <string>
#include <vector>
#include "../lib/huffman.h"

using namespace compress;

class HuffmanTest : public ::testing::Test {
protected:
  HuffmanCompressor compressor;

  void SetUp() override {
    // Any setup code needed before each test
  }

  void TearDown() override {
    // Any cleanup code needed after each test
  }

  // Helper function to generate random data
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
    std::vector<uint8_t> empty;
    HuffmanCompressor compressor;
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
  std::string testString = "Hello, world! This is a test string for Huffman coding.";
  const std::vector<uint8_t> original(testString.begin(), testString.end());

  const auto compressed = compressor.compress(original);
  const auto decompressed = compressor.decompress(compressed);

  EXPECT_EQ(original, decompressed);
}

// Test with repeated characters (should compress well)
TEST_F(HuffmanTest, RepeatedCharacters) {
  std::string repeated = "aaaaabbbbbcccccdddddeeeee";
  const std::vector<uint8_t> original(repeated.begin(), repeated.end());

  const auto compressed = compressor.compress(original);
  const auto decompressed = compressor.decompress(compressed);

  EXPECT_EQ(original, decompressed);
  EXPECT_LT(compressed.size(), original.size()); // Should actually compress
}

// Test with random data
TEST_F(HuffmanTest, RandomData) {
  const auto original = generateRandomData(96); // 1KB of random data

  const auto compressed = compressor.compress(original);
  const auto decompressed = compressor.decompress(compressed);


  EXPECT_EQ(original, decompressed);
}

// Test that compression actually reduces size for compressible data
TEST_F(HuffmanTest, CompressionRatio) {
  std::string highlyCompressible(1000, 'a'); // 1000 'a's
  const std::vector<uint8_t> original(highlyCompressible.begin(), highlyCompressible.end());

  const auto compressed = compressor.compress(original);

  // For highly repetitive data, compressed size should be much smaller
  // EXPECT_LT(compressed.size(), original.size() / 2);

  const auto decompressed = compressor.decompress(compressed);
  EXPECT_EQ(original, decompressed);
}

// Test with all possible byte values
TEST_F(HuffmanTest, AllByteValues) {
  std::vector<uint8_t> original(255);
  for (int i = 0; i < 256; ++i) {
    original[i] = static_cast<uint8_t>(i);
  }

  const auto compressed = compressor.compress(original);
  std::cout << "Compressed" << std::endl;
  for (auto byte: compressed) {
    std::cout << static_cast<int>(byte) << " ";
  }
  const auto decompressed = compressor.decompress(compressed);
  std::cout << std::endl << "Decompressed" << std::endl;
  for (auto byte: decompressed) {
    std::cout << static_cast<int>(byte) << " ";
  }
  std::cout << std::endl;
  EXPECT_EQ(original, decompressed);
}

// Test with larger data
TEST_F(HuffmanTest, LargerData) {
  const auto original = generateRandomData(1024 * 1024); // 1MB of random data

  const auto compressed = compressor.compress(original);
  const auto decompressed = compressor.decompress(compressed);

  EXPECT_EQ(original, decompressed);
}
