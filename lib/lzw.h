#ifndef LZW_H
#define LZW_H


class lzw {
public:
    std::vector<unsigned int> compress(const std::vector<uint8_t> &input);

    std::vector<uint8_t> decompress(const std::vector<unsigned int> &output);
};


#endif //LZW_H
