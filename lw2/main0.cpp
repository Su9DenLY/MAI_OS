#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#include <iostream>
#include <vector>

using namespace std;

struct arg_t {
    int start_i, start_j, i;
    vector<vector<double>>* matrix;
    vector<vector<double>>* solve;
};

void* filter_forward_eliminationthread(void* arg) {
    arg_t* args{static_cast<arg_t*>(arg)};
    return nullptr;
}
int main() {
    // int file_d = open("input100.csv", O_RDONLY);
    // int file_d = open("input2000.csv", O_RDONLY);
    // int file_d = open("input2500.csv", O_RDONLY);
    int file_d = open("input3000.csv", O_RDONLY);

    if (file_d == -1) {
        cerr << "File error" << endl;
        return 1;
    }

    if (dup2(file_d, fileno(stdin)) == -1) {
        cerr << "Dup2 error" << endl;
        return 1;
    }

    int n, m;
    cin >> n;
    m = n + 1;

    vector<double> solve(m, 0);
    vector<vector<double>> matrix(n, vector<double>(m, 0));

    // Ввод матрицы
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> matrix[i][j];
        }
    }

    // Метод Гаусса
    // Прямой ход
    for (int i = 0; i < n; i++) {
        double tmp = matrix[i][i];
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

    close(file_d);

    return 0;
}