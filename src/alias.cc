#include "alias.h"
#include <iostream>

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
        alias();
    } else if (this->command == "unalias") {
        unalias();
    }
}

void ShellBuiltinAlias::alias() {
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
}

void ShellBuiltinAlias::unalias() {
    if (this->args.size() < 1) {
        std::cerr << "Too few Arguments.\n";
        return;
    }
    aliases.erase(this->args.at(0));
}

bool ShellBuiltinAlias::contains_alias(std::string &key) {
    return aliases.contains(key);
}

std::vector<std::string> ShellBuiltinAlias::get_alias(std::string &key) {
    return aliases.at(key);
}
