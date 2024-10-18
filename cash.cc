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

class Command {
    std::string call;
    std::vector<std::string> args;

public:
  Command(std::string call, std::vector<std::string> args)
      : call(std::move(call)), args(std::move(args)) {}
  Command(const Command &) = default;
  Command(Command &&) = default;
  Command &operator=(const Command &) = default;
  Command &operator=(Command &&) = default;

  int exec(fd input, fd output);
};



// executes the command in the path with the current environment
//
// returns the pid of the child
//
// closes the given filedescriptors if they are not stdin or stdout
int Command::exec(fd input, fd output) {
    char **c_args;
    c_args = (char **) malloc(sizeof(char*) * (args.size() + 2));
    c_args[0] = call.data();
    for(size_t i = 0; i<args.size();i++) {
        c_args[i+1] = args.at(i).data();
    }
    c_args[args.size() + 1] = nullptr;
    // input from input and output to output
    if (input != STDIN_FILENO) {
        dup2(input, STDIN_FILENO);
        close(input);
    }
    if (output != STDOUT_FILENO) {
        dup2(output, STDOUT_FILENO);
        close(output);
    }
    int status = execvp(call.c_str(), c_args);
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
    Command a("ls", args);
    int pid = fork();
    if(pid==0)
    {
        close(fd[READ_END]);
        a.exec(STDIN_FILENO,fd[WRITE_END]);
        exit(1);
    }
    /* waitpid(pid, nullptr, WNOHANG); */
    args = std::vector<std::string>();
    a = {"sort", args};
    pid=fork();
    if(pid==0)
    {
        // always close all pipe fds that are not used in exec
        close(fd[WRITE_END]);
        close(fd2[READ_END]);
        a.exec(fd[READ_END], fd2[WRITE_END]);
        exit(1);
    }
    // close fd
    close(fd[READ_END]);
    close(fd[WRITE_END]);
    args = std::vector<std::string>();
    a = {"base64", args};
    pid=fork();
    if(pid==0)
    {
        close(fd2[WRITE_END]);
        a.exec(fd2[READ_END], STDOUT_FILENO);
        exit(1);
    }

    //close fd2
    close(fd2[READ_END]);
    close(fd2[WRITE_END]);
    std::cout << "pid: " << pid << "\n";
    int status;
    waitpid(pid, &status, 0);
    
    std::cout << "after execution\n";
    return 0;
}
