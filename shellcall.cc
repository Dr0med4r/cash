#include "shellcall.h"
#include "commandline.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <vector>
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
    ShellCallAlias::aliases =
        std::unordered_map<std::string, std::vector<std::string>>{};

void ShellCallAlias::exec(int fd1, int fd2) {
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

bool ShellCallAlias::contains_alias(std::string &key) {
    return aliases.contains(key);
}

std::vector<std::string> ShellCallAlias::get_alias(std::string &key) {
    return aliases.at(key);
}
