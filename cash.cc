#include <array>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

#define WRITE_END 1
#define READ_END 0
// currently available shell - cash

// executes the command in the path with the current environment
//
// returns the pid of the child
int exec(std::string command, std::vector<std::string> args) {
    char **c_args;
    c_args = (char **) malloc(sizeof(char*) * (args.size() + 2));
    c_args[0] = command.data();
    for(size_t i = 0; i<args.size();i++) {
        c_args[i+1] = args.at(i).data();
    }
    c_args[args.size() + 1] = nullptr;
    return execvp(command.c_str(), c_args);
}

int main (void) {
    std::array<int,2> fd;
    int error = pipe(fd.data());
    if (error == -1) {
        exit(1);
    }
    std::string command = "ls";
    std::vector<std::string> args = std::vector<std::string>({"-l"});
    int pid = fork();
    if(pid==0)
    {
        dup2(fd[WRITE_END], STDOUT_FILENO);
        close(fd[READ_END]);
        close(fd[WRITE_END]);
        exec(command, args);
        exit(1);
    }
    waitpid(pid, nullptr, WNOHANG);

    std::array<int,2> fd2;
    error = pipe(fd2.data());
    if (error == -1) {
        exit(1);
    }

    command = "sort";
    args = std::vector<std::string>();
    pid=fork();
    if(pid==0)
    {
        close(fd[WRITE_END]);
        close(fd2[READ_END]);
        dup2(fd[READ_END], STDIN_FILENO);
        dup2(fd2[WRITE_END], STDOUT_FILENO);
        close(fd[READ_END]);
        close(fd2[WRITE_END]);
        exec(command, args);
        exit(1);
    }
    waitpid(pid, nullptr, WNOHANG);
    command = "base64";
    args = std::vector<std::string>();
    pid=fork();
    if(pid==0)
    {
        dup2(fd2[READ_END], STDIN_FILENO);
        close(fd2[WRITE_END]);
        close(fd2[READ_END]);
        exec(command, args);
        exit(1);
    }

    close(fd[READ_END]);
    close(fd[WRITE_END]);
    waitpid(pid, nullptr, WNOHANG);
    
    std::cout << "after execution\n";
    return 0;
}
