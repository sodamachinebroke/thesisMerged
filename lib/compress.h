
#ifndef COMPRESS_H
#define COMPRESS_H

#include "../lib/rle.h"
#include "../lib/lzw.h"

namespace compress {
    std::vector<unsigned int> lzw::lzw_compress(const std::vector<uint8_t> &input);

    std::vector<uint8_t> lzw::lzw_decompress(const std::vector<unsigned int> &output);

    std::vector<std::pair<uint8_t, uint8_t> > rle::rle_compress(const std::vector<uint8_t> &data);

    std::vector<uint8_t> rle::rle_decompress(const std::vector<std::pair<uint8_t, uint8_t> > &compressed);
}

#endif //COMPRESS_H
