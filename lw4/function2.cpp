#include <cmath>

#include "function.hpp"

float Pi(int k) {
    float result = 1.0;

    for (int i = 1; i <= k; ++i) {
        result *= (4.0 * i * i) / (4.0 * i * i - 1.0);
    }

    return 2.0 * result;
}

std::pair<int, int> partition(int* a, int begin, int end, int pivot) {
    int lt_end{begin}, eq_end{begin};
    for (int i{begin}; i < end; ++i) {
        if (a[i] < pivot) {
            std::swap(a[eq_end], a[lt_end]);
            if (eq_end != i) {
                std::swap(a[lt_end], a[i]);
            }
            ++lt_end;
            ++eq_end;
        } else if (a[i] == pivot) {
            std::swap(a[eq_end], a[i]);
            ++eq_end;
        }
    }
    return std::pair<int, int>(lt_end, eq_end);
}

void qsort(int* a, int begin, int end) {
    if ((end - begin) < 2) return;
    int pivot = a[begin + rand() % (end - begin)];

    std::pair<int, int> i = partition(a, begin, end, pivot);

    qsort(a, begin, i.first);
    qsort(a, i.second, end);
}

int* Sort(int* array, int size) {
    qsort(array, 0, size);
    return array;
}

// int main() {
//     cout.precision(100);
//     cout << pi(1000) << endl;
//     int array[7] = {1, 5, 2, 1, 3, 7, 3};
//     sort(array, 7);
//     for (int i = 0; i < 7; ++i) {
//         cout << array[i] << " ";
//     }
// }