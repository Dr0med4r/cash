#include "pwd.h"
#include <filesystem>
#include <iostream>

void ShellBuiltinPwd::exec(int fd1, int fd2) {
    setup_fds(fd1, fd2);
    std::cout << std::filesystem::current_path() << "\n";
    exit(0);
}
