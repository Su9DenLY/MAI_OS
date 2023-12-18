#include <iostream>

#include "function.hpp"

using namespace std;

int main() {
    int type;
    while (cin >> type) {
        if (type == 1) {
            int k;
            cin >> k;
            cout << Pi(k) << endl;
        } else if (type == 2) {
            int size;
            cin >> size;
            int *array = new int[size];
            for (int i = 0; i < size; ++i) {
                cin >> array[i];
            }
            Sort(array, size);
            for (int i = 0; i < size; ++i) {
                cout << array[i] << " ";
            }
            cout << endl;
            delete[] array;
        } else {
            break;
        }
    }
}