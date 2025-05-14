#include <benchmark/benchmark.h>
#include <random>
#include <string>
#include <vector>
#include "../lib/lzw.h"

static void BM_LZWCompressionRatio(benchmark::State &state) {
  compress::LZWCompressor compressor;
  const size_t size = state.range(0);
  std::string highlyCompressible(size, 'a');

  for (auto _: state) {
    const std::vector<uint8_t> original(highlyCompressible.begin(), highlyCompressible.end());
    auto compressed = compressor.compress(original);
    const auto decompressed = compressor.decompress(compressed);

    benchmark::DoNotOptimize(decompressed);
    state.counters["CompressionRatio"] = benchmark::Counter(
        compressed.size() * 1.0 / original.size(), benchmark::Counter::kAvgIterations | benchmark::Counter::kIsRate);
  }
}

static void BM_LZWSpeedAndRatio(benchmark::State &state) {
  compress::LZWCompressor compressor;
  const size_t size = state.range(0);
  std::string inputData(size, ' ');
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, 255);
  for (char &c: inputData) {
    c = static_cast<char>(distrib(gen));
  }
  const std::vector<uint8_t> original(inputData.begin(), inputData.end());

  for (auto _: state) {
    auto compressed = compressor.compress(original);
    const auto decompressed = compressor.decompress(compressed);
    benchmark::DoNotOptimize(decompressed);

    state.counters["CompressionRatio"] =
        benchmark::Counter(static_cast<double>(compressed.size()) / original.size(),
                           benchmark::Counter::kAvgIterations | benchmark::Counter::kIsRate);
  }
  state.SetBytesProcessed(state.iterations() * size * 2);
}


BENCHMARK(BM_LZWCompressionRatio)->RangeMultiplier(2)->Range(8, 8 << 16)->MeasureProcessCPUTime();
BENCHMARK(BM_LZWSpeedAndRatio)->RangeMultiplier(2)->Range(8, 8 << 16)->MeasureProcessCPUTime();
BENCHMARK_MAIN();
