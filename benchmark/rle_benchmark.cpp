#include <benchmark/benchmark.h>
#include "../lib/rle.h"

static void BM_RLECompressComplexRepeats(benchmark::State &state) {
  compress::RLECompressor compressor;
  const size_t size = state.range(0);
  std::vector<uint8_t> data;
  data.insert(data.end(), size * 10, 'A');
  for (int i = 0; i < size * 5; ++i)
    data.push_back(static_cast<uint8_t>('B' + i));
  data.insert(data.end(), size * 7, 'C');
  data.insert(data.end(), size * 2, 'D');
  data.insert(data.end(), size * 15, 'E');
  for (int i = 0; i < size * 6; ++i)
    data.push_back(static_cast<uint8_t>('F' + i));
  for (int i = 0; i < size * 20; ++i)
    data.push_back((i % 2 == 0) ? 'G' : 'H');

  data.insert(data.end(), size * 36, 'I');

  for (auto _: state) {
    auto compressed = compressor.compress(data);
    const auto decompressed = compressor.decompress(compressed);
    benchmark::DoNotOptimize(decompressed);

    state.counters["CompressionRatio"] =
        benchmark::Counter(static_cast<double>(compressed.size()) / data.size(),
                           benchmark::Counter::kAvgIterations | benchmark::Counter::kIsRate);
  }
  state.SetBytesProcessed(state.iterations() * data.size() * 2);
}

BENCHMARK(BM_RLECompressComplexRepeats)->Range(8, 8 << 16);
BENCHMARK_MAIN();
