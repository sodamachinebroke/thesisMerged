#include <gtest/gtest.h>
#include <vector>
#include "rle.h" // Your RLE header

using namespace compress;

class RLETest : public ::testing::Test {
protected:
  RLECompressor compressor;
};

// Basic runs
TEST_F(RLETest, CompressSimpleRepeats) {
  const std::vector<uint8_t> input{'A', 'A', 'A', 'B', 'B', 'C'};
  const std::vector<uint8_t> expected{'A', 3, 'B', 2, 'C', 1};
  EXPECT_EQ(compressor.compress(input), expected);
}

TEST_F(RLETest, DecompressSimpleRepeats) {
  const std::vector<uint8_t> compressed{'A', 3, 'B', 2, 'C', 1};
  const std::vector<uint8_t> expected{'A', 'A', 'A', 'B', 'B', 'C'};
  EXPECT_EQ(compressor.decompress(compressed), expected);
}

// Edge cases
TEST_F(RLETest, EmptyInput) {
  EXPECT_THROW(compressor.compress({}), std::runtime_error);
  EXPECT_EQ(compressor.decompress({}), std::vector<uint8_t>());
}

TEST_F(RLETest, NoCompressionGain) {
  const std::vector<uint8_t> input{'A', 'B', 'C', 'D'};
  const std::vector<uint8_t> expected{'A', 1, 'B', 1, 'C', 1, 'D', 1};
  EXPECT_EQ(compressor.compress(input), expected);
}

// Verify roundtrip
TEST_F(RLETest, RoundtripRandomData) {
  std::vector<uint8_t> original(1000);
  std::generate(original.begin(), original.end(), []() { return rand() % 256; });

  const auto compressed = compressor.compress(original);
  const auto decompressed = compressor.decompress(compressed);

  EXPECT_EQ(original, decompressed);
}
