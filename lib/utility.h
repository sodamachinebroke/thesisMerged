#ifndef UTILITY_H
#define UTILITY_H

#include <cstdint>
#include <vector>

struct VectorHash {
    size_t operator()(const std::vector<uint8_t> &v) const {
        size_t seed = v.size();
        for (uint8_t i: v) {
            std::hash<uint8_t> hasher;
            seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

#endif //UTILITY_H
