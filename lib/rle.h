#ifndef RLE_H
#define RLE_H


#include<cstdint>
#include <vector>
#include<utility>

//RLE
class rle {
public:
    std::vector<std::pair<uint8_t, uint8_t> > rle_compress(const std::vector<uint8_t> &data);

    std::vector<uint8_t> rle_decompress(const std::vector<std::pair<uint8_t, uint8_t> > &compressed);
};

#endif //RLE_H
