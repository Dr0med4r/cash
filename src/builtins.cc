#include "builtins.h"
#include "commandline.h"
#include <cerrno>
#include <csignal>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
void ShellBuiltinPwd::exec(int fd1, int fd2) {
    setup_fds(fd1, fd2);
    std::cout << std::filesystem::current_path() << "\n";
    exit(0);
}

void ShellBuiltinCd::exec(int fd1, int fd2) {
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

std::string join(std::vector<std::string>::const_iterator begin,
                 std::vector<std::string>::const_iterator end,
                 std::string seperator = " ") {
    std::string result = *begin;
    begin++;
    for (; begin != end; begin++) {
        result += seperator;
        result += *begin;
    }
    return result;
}

std::unordered_map<std::string, std::vector<std::string>>
    ShellBuiltinAlias::aliases =
        std::unordered_map<std::string, std::vector<std::string>>{};

void ShellBuiltinAlias::exec(int fd1, int fd2) {
    close_fd(fd1);
    close_fd(fd2);
    if (this->command == "alias") {
        if (this->args.size() == 0) {
            for (auto const &[alias, replacement] : aliases) {

                std::string replacement_string =
                    join(replacement.begin(), replacement.end());
                std::cout << "alias " << alias << "='" << replacement_string
                          << "'\n";
            }
            return;
        }
        std::string alias = std::move(this->args.at(0));
        if (this->args.size() == 1) {
            if (aliases.contains(alias)) {
                auto const replacement = aliases.at(alias);
                std::string replacement_string =
                    join(replacement.begin(), replacement.end());
                std::cout << "alias " << alias << "='" << replacement_string
                          << "'\n";
            }
            return;
        }
        if (this->args.size() <= 1) {
            return;
        }
        std::vector<std::string> replacement =
            std::vector<std::string>{++this->args.begin(), this->args.end()};

        aliases.insert_or_assign(alias, replacement);
    } else if (this->command == "unalias") {
        if (this->args.size() < 1) {
            std::cerr << "Too few Arguments.\n";
        }
        aliases.erase(this->args.at(0));
    }
}

bool ShellBuiltinAlias::contains_alias(std::string &key) {
    return aliases.contains(key);
}

std::vector<std::string> ShellBuiltinAlias::get_alias(std::string &key) {
    return aliases.at(key);
}

void ShellBuiltinKill::exec(int fd1, int fd2) {
    close_fd(fd1);
    close_fd(fd2);
    if (this->args.size() < 2) {
        std::cerr << "Too few Arguments.\n";
        return;
    }
    int signo = 0;
    int pid = 0;
    size_t processed = 0;
    try {
        signo = std::stoi(this->args.at(0), &processed);
        if (processed != args.at(0).size()) {
            std::cerr << "Signal number is not a number.\n";
            return;
        }
    } catch (std::invalid_argument &) {
        std::cerr << "Signal number is not a number.\n";
        return;
    }
    processed = 0;
    try {
        pid = std::stoi(this->args.at(1), &processed);
        if (processed != args.at(1).size()) {
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
