#include "shellcall.h"
#include "commandline.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <numeric>
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

std::unordered_map<std::string, std::string> ShellCallAlias::aliases =
    std::unordered_map<std::string, std::string>{};

void ShellCallAlias::exec(int fd1, int fd2) {
    close_fd(fd1);
    close_fd(fd2);
    if (this->command == "alias") {
        if (this->args.size() == 0) {
            for (auto const &[alias, replacement] : aliases) {
                std::cout << "alias " << alias << "='" << replacement << "'\n";
            }
            return;
        }
        if (this->args.size() == 1) {
            auto alias = this->args.at(0);
            if (aliases.contains(alias)) {
                auto const replacement = aliases.at(alias);
                std::cout << "alias " << alias << "='" << replacement << "'\n";
            }
            return;
        }
        auto join = [](std::string base, std::string accumulator) {
            return std::move(base) + " " + std::move(accumulator);
        };
        std::string replacement = std::accumulate(
            ++this->args.begin(), this->args.end(), std::string{}, join);
        aliases.insert_or_assign(this->args.front(), replacement);
    } else if (this->command == "unalias") {
        if (this->args.size() < 1) {
            std::cerr << "Too few Arguments.\n";
        }
        aliases.erase(this->args.at(0));
    }
}

void ShellCallAlias::resolve_alias(std::string &to_expand) {
    std::string first_arg = to_expand.substr(0, to_expand.find(" "));
    if (aliases.contains(first_arg)) {
        to_expand.erase(0,first_arg.size());
        to_expand.insert(0, aliases.at(first_arg));
    }
}
