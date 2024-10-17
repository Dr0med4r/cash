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
//

typedef int fd;

struct Command {
    std::string call;
    std::vector<std::string> args;
    fd input;
    fd output;
};

void exec_pipes(std::vector<Command>) {

}


// executes the command in the path with the current environment
//
// returns the pid of the child
//
// closes the given filedescriptors if they are not stdin or stdout
int exec(Command cmd) {
    char **c_args;
    c_args = (char **) malloc(sizeof(char*) * (cmd.args.size() + 2));
    c_args[0] = cmd.call.data();
    for(size_t i = 0; i<cmd.args.size();i++) {
        c_args[i+1] = cmd.args.at(i).data();
    }
    c_args[cmd.args.size() + 1] = nullptr;
    // input from input and output to output
    if (cmd.input != STDIN_FILENO) {
        dup2(cmd.input, STDIN_FILENO);
        close(cmd.input);
    }
    if (cmd.output != STDOUT_FILENO) {
        dup2(cmd.output, STDOUT_FILENO);
        close(cmd.output);
    }
    int status = execvp(cmd.call.c_str(), c_args);
    free(c_args);
    return status;
}

int main (void) {
    std::array<int,2> fd;
    std::array<int,2> fd2;
    int error = pipe(fd.data());
    if (error == -1) {
        exit(1);
    }
    error = pipe(fd2.data());
    if (error == -1) {
        exit(1);
    }

    std::vector<std::string> args = std::vector<std::string>({"-l"});
    Command a = {"ls", args, STDIN_FILENO,fd[WRITE_END]};
    int pid = fork();
    if(pid==0)
    {
        close(fd[READ_END]);
        exec(a);
        exit(1);
    }
    /* waitpid(pid, nullptr, WNOHANG); */
    args = std::vector<std::string>();
    a = {"sort", args, fd[READ_END], fd2[WRITE_END]};
    pid=fork();
    if(pid==0)
    {
        // always close all pipe fds that are not used in exec
        close(fd[WRITE_END]);
        close(fd2[READ_END]);
        exec(a);
        exit(1);
    }
    // close fd
    close(fd[READ_END]);
    close(fd[WRITE_END]);
    args = std::vector<std::string>();
    a = {"base64", args, fd2[READ_END], STDOUT_FILENO};
    pid=fork();
    if(pid==0)
    {
        close(fd2[WRITE_END]);
        exec(a);
        exit(1);
    }

    //close fd2
    close(fd2[READ_END]);
    close(fd2[WRITE_END]);
    waitpid(pid, nullptr, WNOHANG);
    
    std::cout << "after execution\n";
    return 0;
}
