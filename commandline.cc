#include "commandline.h"
#include "errors.h"
#include "shellcall.h"
#include <array>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <sys/wait.h>
#include <typeinfo>
#include <unistd.h>

void Call::resolve_alias() {
    if (!ShellCallAlias::contains_alias(command)) {
        return;
    }
    std::vector<std::string> replacement = ShellCallAlias::get_alias(command);
    if (replacement.size() == 0) {
        return;
    }
    this->command = replacement.front();
    if (replacement.size() < 2) {
        return;
    }
    this->args.insert(this->args.begin(), ++replacement.begin(),
                      replacement.end());
}

// executes the command in the path with the current environment
//
// returns the pid of the child
//
// closes the given filedescriptors if they are not stdin or stdout
void Call::exec(fd input, fd output) {
    resolve_alias();
    char **c_args;
    c_args = (char **)malloc(sizeof(char *) * (args.size() + 2));
    c_args[0] = command.data();
    for (size_t i = 0; i < args.size(); i++) {
        c_args[i + 1] = args.at(i).data();
    }
    c_args[args.size() + 1] = nullptr;
    // input from input and output to output
    // dup2 does nothing if fd and fd2 are the same
    dup2(input, STDIN_FILENO);
    close_fd(input);
    dup2(output, STDOUT_FILENO);
    close_fd(output);
    execvp(command.c_str(), c_args);
    std::stringstream msg;
    msg << "executing " << *this << " failed";
    if (errno == ENOENT) {
        msg << ": command not found";
    }
    if (errno == EACCES) {
        msg << ": access denied";
    }
    msg << "\n";

    free(c_args);
    throw ExecError{msg.str()};
}

void Call::add_arg(std::string arg) { args.push_back(arg); }

std::ostream &operator<<(std::ostream &os, const Call &obj) {

    os << "Call(" << obj.command << ", [ ";
    for (auto elem : obj.args) {
        os << elem << ", ";
    }
    os << "])";
    return os;
}

void Command::set_input(fd input) {
    if (fcntl(input, F_GETFD) == -1) {
        throw std::runtime_error{"not a valid fd"};
    }
    this->input = input;
}

void Command::set_output(fd output) {
    if (fcntl(input, F_GETFD) == -1) {
        throw std::runtime_error{"not a valid fd"};
    }
    this->output = output;
}

void Command::add_call(std::unique_ptr<Call> call) {
    calls.push_back(std::move(call));
}
bool Command::has_valid_fds() { return input != -1 && output != -1; }
void Command::set_background(bool background) { wait = !background; }
std::ostream &operator<<(std::ostream &os, const Command &obj) {
    os << "Command( ";
    os << "input: " << obj.input << ", ";
    os << "output: " << obj.output << ", ";
    os << "background: " << !obj.wait << ", ";
    for (auto &elem : obj.calls) {
        os << *elem << ", ";
    }
    os << ")";
    return os;
}

// executes all given commands with pipes and the given input and output
// redirection
//
// closes input and output fds if not stdfile
void Command::exec() {
    if (!has_valid_fds()) {
        throw ExecError{"the output or input is not set"};
    }
    int pid = -1;
    switch (calls.size()) {
    case 0:
        return;
    default: {
        std::array<int, 2> fd1 = {input, 0};
        std::array<int, 2> fd2 = {0, output};
        std::array<std::array<int, 2>, 2> fds = {fd1, fd2};
        int which_pipe = 0;
        size_t i = 0;
        // alternate between the two pipes as input and output
        for (auto &call : calls) {
            which_pipe = 1 - which_pipe;
            int this_pipe = which_pipe;
            int other_pipe = 1 - which_pipe;
            int error = pipe(fds[this_pipe].data());
            if (error == -1) {
                throw ExecError{"error creating pipe"};
            }
            if (i == calls.size() - 1) {
                fds[this_pipe][WRITE_END] = output;
            }
            auto &val = *call;
            if (typeid(val) == typeid(Call)) {
                pid = fork();
            } else {
                pid = 0;
            }
            if (pid == -1) {
                throw ExecError{"error forking"};
            }
            if (pid == 0) {
                close_fd(fds[other_pipe][WRITE_END]);
                close_fd(fds[this_pipe][READ_END]);
                try {
                    call->exec(fds[other_pipe][READ_END],
                               fds[this_pipe][WRITE_END]);

                } catch (ExecError &e) {
                    std::cerr << e.what();
                    exit(1);
                }
            }
            for (int fd : fds[other_pipe]) {
                close_fd(fd);
            }
            if (wait && pid > 0) {
                waitpid(pid, nullptr, NO_OPTION);
            }
            i++;
        }
        for (auto arr : fds) {
            for (int fd : arr) {
                close_fd(fd);
            }
        }

    } break;
    }
    // as the input and output are closed now do not use them again
    input = -1;
    output = -1;
}
