#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

// #include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct arg_t {
    int start, finish, id, n, m;
    vector<vector<double>>* matrix;
    vector<double>* solve;
};

void input(int& n, int& m, vector<vector<double>>& matrix, vector<double>& solve, vector<arg_t>& args, int& thread_count) {
    // cin >> n;
    // m = n + 1;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> matrix[i][j];
        }
    }

    for (int i = 0; i < thread_count; ++i) {
        args[i].id = i;
        args[i].n = n;
        args[i].m = m;
        args[i].matrix = &matrix;
        args[i].solve = &solve;
        args[i].start = n / thread_count * i;
        args[i].finish = n / thread_count * (i + 1);
    }
}

void* forward_elimination(void* arg) {
    arg_t* args{static_cast<arg_t*>(arg)};

    int start = min(args->start, args->n);
    int finish = min(args->finish + 1, args->n);

    for (int i = start; i < finish; i++) {
        double tmp = (*args->matrix)[i][i];
        for (int j = args->n; j >= i; j--) {
            (*args->matrix)[i][j] /= tmp;
        }

        for (int j = i + 1; j < finish; j++) {
            tmp = (*args->matrix)[j][i];
            for (int k = args->n; k >= i; k--) {
                (*args->matrix)[j][k] -= tmp * (*args->matrix)[i][k];
            }
        }
    }
    pthread_exit(0);
}

void* back_substitution(void* arg) {
    arg_t* args{static_cast<arg_t*>(arg)};

    int start = min(args->start, args->n);
    int finish = min(args->finish + 1, args->n);

    (*args->solve)[finish - 1] = (*args->matrix)[finish - 1][finish];
    for (int i = finish - 2; i >= 0; i--) {
        (*args->solve)[i] = (*args->matrix)[i][finish];
        for (int j = i + 1; j < finish; j++) {
            (*args->solve)[i] -= (*args->matrix)[i][j] * (*args->solve)[j];
        }
    }

    pthread_exit(0);
}

int main(int argc, char* argv[]) {
    int file_d = open("input.txt", O_RDONLY);
    // int file_d = open("input100.csv", O_RDONLY);
    // int file_d = open("input2000.csv", O_RDONLY);
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

    int thread_count = stoi(argv[1]);
    if (thread_count < 1) {
        cerr << "At least 1 thread must exist" << endl;
        return 1;
    }

    int n, m;
    cin >> n;
    m = n + 1;

    thread_count = min(thread_count, n);
    vector<arg_t> args(thread_count);
    vector<pthread_t> tid(thread_count);
    vector<double> solve(n, 0);
    vector<vector<double>> matrix(n, vector<double>(m, 0));

    input(n, m, matrix, solve, args, thread_count);

    for (int i = 0; i < thread_count; ++i) {
        pthread_create(&tid[i], nullptr, forward_elimination, static_cast<void*>(&args[i]));
    }

    for (int i = 0; i < thread_count; ++i) {
        pthread_join(tid[i], nullptr);
    }

    for (int i = 0; i < thread_count; ++i) {
        pthread_create(&tid[i], nullptr, back_substitution, static_cast<void*>(&args[i]));
    }

    for (int i = 0; i < thread_count; ++i) {
        pthread_join(tid[i], nullptr);
    }

    cout << "Solve" << endl;
    for (int i = 0; i < n; i++) {
        cout << "x" << i + 1 << " = " << solve[i] << endl;
    }

    close(file_d);

    return 0;
}