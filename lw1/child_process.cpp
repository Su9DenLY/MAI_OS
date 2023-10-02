#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[]) {
    int file_d = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

    if (file_d == -1) {
        cerr << "Creating file error" << endl;
        return 1;
    }
    
    if (dup2(file_d, fileno(stdout)) == -1) {
        cerr << "Dup2 error" << endl;
        return 1;
    }
    string line;

    char c = ' ';
    float ans = 0.0, number = 0.0;
    while (scanf("%f%c", &number, &c) > 0) {
        ans += number;
        if (c == '\n') {
            cout << ans << endl;
            ans = 0.0;
        }
    }

    close(file_d);
    return 0;
}