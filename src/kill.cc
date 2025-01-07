#include "kill.h"
#include <iostream>
#include <csignal>

void ShellBuiltinKill::exec(int fd1, int fd2) {
    close_fd(fd1);
    close_fd(fd2);
    if (this->args.size() < 2) {
        std::cerr << "Too few Arguments.\n";
        return;
    }
    int signo = 0;
    size_t signal_valid = 0;
    try {
        signo = std::stoi(this->args.at(0), &signal_valid);
        if (signal_valid != args.at(0).size()) {
            std::cerr << "Signal number is not a number.\n";
            return;
        }
    } catch (std::invalid_argument &) {
        std::cerr << "Signal number is not a number.\n";
        return;
    }
    int pid = 0;
    size_t pid_valid = 0;
    try {
        pid = std::stoi(this->args.at(1), &pid_valid);
        if (pid_valid != args.at(1).size()) {
            std::cerr << "Process id is not a number.\n";
            return;
        }
    } catch (std::invalid_argument &) {
        std::cerr << "Process id is not a number.\n";
        return;
    }
    int result = kill(pid, signo);
    if (result == -1) {
        switch (errno) {
        case EINVAL:
            std::cerr << "Invalid signal.\n";
            break;
        case EPERM:
            std::cerr << "No permissions.\n";
            break;
        case ESRCH:
            std::cerr << "Process does not exist.\n";
            break;
        }
    }
    exit(0);
}
