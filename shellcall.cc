#include "shellcall.h"
#include "commandline.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
void ShellCallPwd::exec(int fd1, int fd2) {
    close_fd(fd1);
    close_fd(fd2);
    std::cout << std::filesystem::current_path() << "\n";
    ;
}

void ShellCallCd::exec(int fd1, int fd2) {
    close_fd(fd1);
    close_fd(fd2);
    if (this->args.size() > 1) {
        std::cerr << "Too many Arguments.\n";
    }
    if (this->args.size() < 1) {
        args.push_back(std::getenv("HOME"));
    }
    try {

        std::filesystem::current_path(this->args.front());
    } catch (std::filesystem::filesystem_error const &e) {
        std::cerr << e.what() << "\n";
    }
}
