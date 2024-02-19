#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include <iostream>

#include "shared_data.hpp"

using namespace std;

int main() {
    int err;
    string file;
    cout << "Enter filename: ";
    cin >> file;
    cout << "Enter commands:" << endl;

    int shm_fd = shm_open(shm_name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    throw_if(shm_fd, "Shared memory open error");

    err = ftruncate(shm_fd, sizeof(SharedData));
    throw_if(err, "Shared memory truncate error");

    SharedData* data = (SharedData*)mmap(NULL, sizeof(SharedData), PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED) {
        throw runtime_error("Error displaying shared memory");
    }

    err = sem_init(&data->sem1, 1, 0);
    throw_if(err, "Semaphore init error");

    err = sem_init(&data->sem2, 1, 0);
    throw_if(err, "Semaphore init error");

    pid_t pid = fork();
    throw_if(pid, "Fork failed");
    if (pid == 0) {
        err = execl("./child_process", "./child_process", file.c_str(), NULL);
        throw_if(err, "Child file error");
    } else {
        string s, out = "";
        while (getline(cin, s)) {
            if (s.size()) {
                s += '\n';
            }
            out += s;
        }

        data->data[15] = '\0';
        for (size_t i = 0; i < out.size(); ++i) {
            if (!(i % 15)) {
                err = sem_post(&data->sem1);
                throw_if(err, "Semaphore post error");
                err = sem_wait(&data->sem2);
                throw_if(err, "Semaphore wait error");
            }
            data->data[i % 15] = out[i];
        }
        if (!out.size() % 15) {
            data->data[out.size() % 15] = '\0';
        }
        data->end = true;
        err = sem_post(&data->sem1);
        throw_if(err, "Semaphore post error");

        wait(nullptr);
    }
    err = sem_destroy(&data->sem1);
    throw_if(err, "Semaphore destroy error");
    err = sem_destroy(&data->sem2);
    throw_if(err, "Semaphore destroy error");

    munmap(data, sizeof(SharedData));
    shm_unlink(shm_name);
}
