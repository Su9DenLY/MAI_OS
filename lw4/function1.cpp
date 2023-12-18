#include <utility>

#include "function.hpp"

float Pi(int k) {
    float result = 0.0;

    for (int i = 0; i < k; ++i) {
        result += (1.0 - i % 2 * 2.0) / (2.0 * i + 1.0);
    }

    return result * 4.0;
}

int* Sort(int* array, int size) {
    bool sorted;
    for (int i{0}; i < size; ++i) {
        sorted = true;
        for (int j{0}; j < size - i - 1; ++j) {
            if (array[j] > array[j + 1]) {
                std::swap(array[j], array[j + 1]);
                sorted = false;
            }
        }
        if (sorted) break;
    }
    return array;
}