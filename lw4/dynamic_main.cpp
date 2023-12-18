#include <dlfcn.h>

#include <iostream>

using namespace std;

typedef float (*pi_func)(int);
typedef int* (*sort_func)(int*, int);

pi_func Pi = nullptr;
sort_func Sort = nullptr;
void* lib_handle = nullptr;

void load_lib(const char* file) {
    lib_handle = dlopen(file, RTLD_LAZY);
    if (!lib_handle) {
        throw runtime_error(dlerror());
    }

    Pi = (pi_func)dlsym(lib_handle, "Pi");
    if (!Pi) {
        throw runtime_error(dlerror());
    }
    Sort = (sort_func)dlsym(lib_handle, "Sort");
    if (!Sort) {
        throw runtime_error(dlerror());
    }
}

int main() {
    load_lib("libfunction1.so");
    int type, current_lib = 1;
    while (cin >> type) {
        if (type == 0) {
            dlclose(lib_handle);
            switch (current_lib) {
                case 1:
                    load_lib("libfunction2.so");
                    current_lib = 2;
                    break;

                default:
                    load_lib("libfunction1.so");
                    current_lib = 1;
                    break;
            }
        } else if (type == 1) {
            int k;
            cin >> k;
            cout << Pi(k) << endl;
        } else if (type == 2) {
            int size;
            cin >> size;
            int* array = new int[size];
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