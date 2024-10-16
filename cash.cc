#include <array>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

#define WRITEEND 1
#define READEND 0
// currently available shell - cash

// executes the command in the path with the current environment
//
// returns the pid of the child
int exec_fork(std::string command, std::vector<std::string> args, std::array<int,2> filedescriptors) {
    char **c_args;
    c_args = (char **) malloc(sizeof(char*) * (args.size() + 2));
    c_args[0] = command.data();
    for(size_t i = 0; i<args.size();i++) {
        c_args[i+1] = args.at(i).data();
    }
    c_args[args.size() + 1] = nullptr;
    int pid = fork();
    switch (pid) {
        case -1:
            return -1;
            break;
        case 0:
            dup2(filedescriptors[WRITEEND], STDOUT_FILENO);
            dup2(filedescriptors[READEND], STDIN_FILENO);
            close(filedescriptors[WRITEEND]);
            close(filedescriptors[READEND]);
            execvp(command.c_str(), c_args);
            return -1;
            break;
        default:
            free((void*) c_args);
            return pid;
    }
}

int main (void) {
    std::string command = "ls";
    std::vector<std::string> args = std::vector<std::string>({"-l"});
    std::string command2 = "wc";
    std::vector<std::string> args2 = std::vector<std::string>();
    std::array<int,2> fd;
    int error = pipe(fd.data());
    if (error == -1) {
        return -1;
    }
    std::array<int,2> fd1({STDIN_FILENO,fd[WRITEEND]});
    exec_fork(command, args, fd1);
    std::array<int,2> fd2({fd[READEND],STDOUT_FILENO});
    exec_fork(command, args, fd);
    
    std::cout << "after execution\n";
    return 0;
}
