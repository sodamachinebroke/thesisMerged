#include <gtest/gtest.h>
#include <random>
#include <string>
#include <vector>
#include "lzw.h"

using namespace compress;

class LZWTest : public ::testing::Test {
protected:
  LZWCompressor compressor;

  static std::vector<uint8_t> strToVec(const std::string &s) { return {s.begin(), s.end()}; }

  static std::string vecToStr(const std::vector<uint8_t> &v) { return {v.begin(), v.end()}; }

  static std::vector<uint8_t> generateRandomData(const size_t size) {
    std::vector<uint8_t> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    for (auto &byte: data) {
      byte = static_cast<uint8_t>(dis(gen));
    }
    return data;
  }
};

// Basic compression tests
TEST_F(LZWTest, CompressSimpleSequence) {
  const auto input = strToVec("ABABABA");

  const auto compressed = compressor.compress(input);

  // Should produce 8 bytes (4 16-bit codes)
  ASSERT_EQ(compressed.size(), 8);

  // First two codes must be literal 'A' and 'B'
  EXPECT_EQ(compressed[0], 0x00);
  EXPECT_EQ(compressed[1], 0x41); // 'A'
  EXPECT_EQ(compressed[2], 0x00);
  EXPECT_EQ(compressed[3], 0x42); // 'B'

  const auto decompressed = compressor.decompress(compressed);
  EXPECT_EQ(input, decompressed);
}

TEST_F(LZWTest, EmptyInputThrows) {
  EXPECT_THROW(compressor.compress({}), std::invalid_argument);
  EXPECT_THROW(compressor.decompress({}), std::invalid_argument);
}

// Roundtrip verification tests
TEST_F(LZWTest, RoundtripSimpleText) {
  const std::string test_str = "TOBEORNOTTOBEORTOBEORNOT";
  const auto original = strToVec(test_str);

  const auto compressed = compressor.compress(original);
  const auto decompressed = compressor.decompress(compressed);

  EXPECT_EQ(original, decompressed);
}

TEST_F(LZWTest, RoundtripBinaryData) {
  const std::vector<uint8_t> original{
      0x00, 0xFF, 0x55, 0xAA, 0x00, 0xFF, 0x55, 0xAA // Repeated pattern
  };

  const auto compressed = compressor.compress(original);
  const auto decompressed = compressor.decompress(compressed);

  EXPECT_EQ(original, decompressed);
}

// Dictionary management tests
TEST_F(LZWTest, HandlesDictionaryReset) {
  // Generate data that will force dictionary reset
  const std::vector<uint8_t> large_input(70000, 'A'); // Will exceed 16-bit code space

  const auto compressed = compressor.compress(large_input);
  EXPECT_FALSE(compressed.empty());

  const auto decompressed = compressor.decompress(compressed);
  EXPECT_EQ(large_input, decompressed);
}

TEST_F(LZWTest, SingleCharacterRepeated) {
  const auto input = strToVec("AAAAAA");
  const auto compressed = compressor.compress(input);
  const auto decompressed = compressor.decompress(compressed);
  EXPECT_EQ(input, decompressed);
}

// Error case tests
TEST_F(LZWTest, InvalidOddLengthInput) {
  const std::vector<uint8_t> corrupt_data{0x00, 0x41, 0x00}; // Odd number of bytes
  EXPECT_THROW(compressor.decompress(corrupt_data), std::invalid_argument);
}

// Test cases that will throw as expected:
TEST_F(LZWTest, JumpInCodeNumbers) {
  std::vector<uint8_t> corrupt{
      0x00, 0x41, // 'A'
      0x01, 0x05 // Jumps to 261
  };
  EXPECT_THROW(compressor.decompress(corrupt), std::runtime_error);
}

// Performance tests
TEST_F(LZWTest, LargeRandomDataRoundtrip) {
  const size_t test_sizes[] = {100, 1000, 10000, 100000};
  const int iterations = 100;

  for (size_t size: test_sizes) {
    for (int i = 0; i < iterations; i++) {
      const auto original = generateRandomData(size);
      const auto compressed = compressor.compress(original);

      // Verify compressed data is valid
      ASSERT_EQ(compressed.size() % 2, 0);

      const auto decompressed = compressor.decompress(compressed);

      // Compare original and decompressed
      ASSERT_EQ(original.size(), decompressed.size());
      for (size_t j = 0; j < original.size(); j++) {
        if (original[j] != decompressed[j]) {
          FAIL() << "Mismatch at position " << j << " in test size " << size << " iteration " << i;
        }
      }
    }
  }
}

TEST_F(LZWTest, AllByteValuesRoundtrip) {
  std::vector<uint8_t> original(256);
  std::iota(original.begin(), original.end(), 0);

  const auto compressed = compressor.compress(original);
  const auto decompressed = compressor.decompress(compressed);

  EXPECT_EQ(original, decompressed);
}

// Edge case tests
TEST_F(LZWTest, MaxDictionarySizeBehavior) {
  // Generate data that will nearly fill dictionary
  std::vector<uint8_t> original;
  for (int i = 0; i < 500; i++) {
    original.insert(original.end(), {static_cast<uint8_t>(i), static_cast<uint8_t>(i + 1)});
  }

  const auto compressed = compressor.compress(original);
  const auto decompressed = compressor.decompress(compressed);

  EXPECT_EQ(original, decompressed);
}
