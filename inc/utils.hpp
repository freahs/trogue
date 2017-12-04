#ifndef TROGUE_UTILS_H
#define TROGUE_UTILS_H

#include <random>

namespace trogue {
    namespace utils {
        inline int random(int min, int max) {
            if (min == max) return min;
            static std::random_device srd;
            static std::mt19937 mt(srd());
            auto dist = std::uniform_int_distribution<int>(min, max);
            auto x = dist(mt);
            return x;
        }

        inline float random_float(float min, float max) {
            static std::random_device srd;
            static std::mt19937 mt(srd());
            auto dist = std::uniform_real_distribution<>(min, max);
            auto x = dist(mt);
            return x;
        }
    }
}

#endif
