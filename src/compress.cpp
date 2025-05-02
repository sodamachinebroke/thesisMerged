#include "compress.h"
#include "huffman.h"
#include "lzw.h"
#include "rle.h"

namespace compress {

  Compressor createRLECompressor() { return Compressor(RLECompressor{}); }

  Compressor createLZWCompressor() { return Compressor(LZWCompressor{}); }

  Compressor createHuffmanCompressor() { return Compressor(HuffmanCompressor{}); }

} // namespace compress
