#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>

#include <iostream>

#include "shared_data.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    int file_d = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

    if (file_d == -1) {
        cerr << "Creating file error" << endl;
        return 1;
    }

    if (dup2(file_d, fileno(stdout)) == -1) {
        cerr << "Dup2 error" << endl;
        return 1;
    }

    int shm_fd = shm_open(shm_name, O_RDWR, S_IRUSR | S_IWUSR);
    throw_if(shm_fd, "Shared memory open error");

    SharedData* data = (SharedData*)mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED) {
        throw std::runtime_error("Shared memory map error");
    }

    string in;
    while (1) {
        int err = sem_wait(&data->sem1);
        throw_if(err, "Semaphore wait error");
        in += data->data;
        err = sem_post(&data->sem2);
        throw_if(err, "Semaphore post error");
        if (data->end) {
            break;
        }
    }

    float res = 0;
    size_t pos = 0;
    for (size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '\n' && (i - pos)) {
            res += stof(in.substr(pos, i - pos));
            pos = i + 1;
            cout << res << endl;
            res = 0;
        } else if (isspace(in[i]) && (i - pos)) {
            res += stof(in.substr(pos, i - pos));
            pos = i + 1;
        }
    }

    munmap(data, sizeof(SharedData));
    shm_unlink(shm_name);
    fclose(stdout);
    return 0;
}
