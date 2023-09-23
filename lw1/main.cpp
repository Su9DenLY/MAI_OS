#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main() {
    int pipe1[2], pipe2[2];

    if (pipe(pipe1) == -1 or pipe(pipe2) == -1) {
        cerr << "Pipe error" << endl;
        return 1;
    }
    string filename;
    getline(cin, filename);
    pid_t pid = fork();

    if (pid == -1) {
        cerr << "Fork error" << endl;
        return 1;
    } else if (pid == 0) {
        close(pipe1[1]);
        close(pipe2[0]);

        if (dup2(pipe1[0], fileno(stdin)) == -1 or dup2(pipe2[1], fileno(stdout)) == -1) {
            cerr << "Dup2 error" << endl;
            return 1;
        }

        if (execl("./child_process", "./child_process", filename.c_str(), NULL) == -1) {
            cerr << "Exec error" << endl;
            return 1;
        }
        
        close(pipe1[0]);
        close(pipe2[1]);

    } else {
        close(pipe1[0]);
        close(pipe2[1]);

        string line;
        while(getline(cin, line)) {
            if (line == "exit") {
                break;
            }
            line += "\n";
            write(pipe1[1], line.c_str(), line.length());
        }

        close(pipe1[1]);
        close(pipe2[0]);
        wait(nullptr);
    }
    
    return 0;
}