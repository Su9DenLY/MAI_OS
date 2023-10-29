#include <fcntl.h>
#include <unistd.h>

#include <iostream>

using namespace std;

const float epsilon = 0.001;

int main() {
    // int file_d = open("input100.txt", O_RDONLY);
    int file_d = open("input2000.csv", O_RDONLY);
    // int file_d = open("input2500.csv", O_RDONLY);
    // int file_d = open("input3000.csv", O_RDONLY);

    if (file_d == -1) {
        cerr << "File error" << endl;
        return 1;
    }

    if (dup2(file_d, fileno(stdin)) == -1) {
        cerr << "Dup2 error" << endl;
        return 1;
    }

    int n, m;
    // cout << "Enter the number of unknowns: ";
    cin >> n;
    // cout << "Enter matrix\n";
    m = n + 1;
    float* solve = new float[m];

    // Выделение памяти под матрицу
    float** matrix = new float*[n];
    for (int i = 0; i < n; ++i) {
        matrix[i] = new float[m];
    }

    // Ввод матрицы
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> matrix[i][j];
        }
    }

    // Метод Гаусса
    // Прямой ход
    for (int i = 0; i < n; i++) {
        float tmp = matrix[i][i];
        for (int j = n; j >= i; j--) {
            matrix[i][j] /= tmp;
        }

        for (int j = i + 1; j < n; j++) {
            tmp = matrix[j][i];
            for (int k = n; k >= i; k--) {
                matrix[j][k] -= tmp * matrix[i][k];
            }
        }
    }

    // for (int i = 0; i < n; ++i) {
    //     for (int j = 0; j < m; ++j) {
    //         cout << matrix[i][j] << "  ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    // Обратный ход
    solve[n - 1] = matrix[n - 1][n];
    for (int i = n - 2; i >= 0; i--) {
        solve[i] = matrix[i][n];
        for (int j = i + 1; j < n; j++) {
            solve[i] -= matrix[i][j] * solve[j];
        }
    }

    // Вывод решения
    cout << "Solve" << endl;
    for (int i = 0; i < n; i++) {
        cout << "x" << i + 1 << " = " << solve[i] << endl;
    }

    // Чистка памяти
    for (int i = 0; i < n; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
    delete[] solve;

    return 0;
}

/*
2 1 -1 8
-3 -1 2 -11
-2 1 2 3
*/