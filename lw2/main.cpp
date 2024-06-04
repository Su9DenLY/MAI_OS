#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include <chrono>
#include <iostream>

using namespace std;

struct Args {
    int tid;
    int num_threads;
    float *matrix;
    int columns;
    pthread_barrier_t *barrier;
};

void print(float *matrix, int rows, int columns) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            cout << matrix[i * columns + j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void *ge_parallel(void *args) {
    Args *local_args = (Args *)args;

    int tid = local_args->tid;
    int num_threads = local_args->num_threads;
    float *matrix = local_args->matrix;
    int columns = local_args->columns;
    pthread_barrier_t *barrier = local_args->barrier;

    for (int i = 0; i < columns - 1; i++) {
        if ((i % num_threads) == tid) {
            float pivot = matrix[i * columns + i];

            for (int j = i + 1; j < columns; j++) {
                matrix[i * columns + j] /= pivot;
            }

            matrix[i * columns + i] = 1;
        }

        pthread_barrier_wait(barrier);

        // print(matrix, columns - 1, columns);

        for (int j = i + 1; j < columns; j++) {
            if ((j % num_threads) == tid) {
                float scale = matrix[j * columns + i];

                for (int l = i + 1; l < columns; l++) {
                    matrix[j * columns + l] -= matrix[i * columns + l] * scale;
                }

                matrix[j * columns + i] = 0;
            }
        }
    }

    return 0;
}

void launch_threads(int num_threads, float *matrix, float *solve, int rows, int columns) {
    pthread_t *threads = new pthread_t[num_threads];

    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, num_threads);

    Args *thread_args = new Args[num_threads];

    for (int i = 0; i < num_threads; i++) {
        thread_args[i].tid = i;
        thread_args[i].num_threads = num_threads;
        thread_args[i].matrix = matrix;
        thread_args[i].columns = columns;
        thread_args[i].barrier = &barrier;

        pthread_create(&threads[i], NULL, ge_parallel, (void *)&thread_args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    solve[rows - 1] = matrix[rows * rows + rows - 1];
    for (int i = rows - 2; i >= 0; i--) {
        solve[i] = matrix[i * (rows + 1) + rows];
        for (int j = i + 1; j < rows; j++) {
            solve[i] -= matrix[i * (rows + 1) + j] * solve[j];
        }
    }

    delete[] threads;
    delete[] thread_args;
}

int main(int argc, char *argv[]) {
    int num_threads = stoi(argv[1]);
    if (argc < 1) {
        cerr << "At least 1 thread must exist" << endl;
        return 1;
    }

    int output_file_d = open("../InOutFiles/output.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    int input_file_d = open("../InOutFiles/10.txt", O_RDONLY);
    // int input_file_d = open("../InOutFiles/2500.txt", O_RDONLY);
    // int input_file_d = open("../InOutFiles/3000.txt", O_RDONLY);

    if (input_file_d == -1 || output_file_d == -1) {
        cerr << "File error" << endl;
        return 1;
    }

    if (dup2(input_file_d, fileno(stdin)) == -1) {
        cerr << "Dup2 error" << endl;
        return 1;
    }

    int rows, columns;
    cin >> rows;
    columns = rows + 1;

    float *matrix;
    float *solve;

    matrix = new float[columns * columns];
    solve = new float[rows];

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            cin >> matrix[i * columns + j];
        }
    }

    chrono::high_resolution_clock::time_point start;
    chrono::high_resolution_clock::time_point end;

    start = chrono::high_resolution_clock::now();
    launch_threads(num_threads, matrix, solve, rows, columns);
    end = chrono::high_resolution_clock::now();

    chrono::duration<float> elapsed = chrono::duration_cast<chrono::duration<float>>(end - start);
    cout << "Time = " << elapsed.count() << " seconds" << endl;

    // print(matrix, columns - 1, columns);

    if (dup2(output_file_d, fileno(stdout)) == -1) {
        cerr << "Dup2 error" << endl;
        return 1;
    }

    for (int i = 0; i < rows; i++) {
        cout << solve[i] << endl;
    }

    delete[] matrix;
    delete[] solve;
    close(input_file_d);
    close(output_file_d);

    return 0;
}
