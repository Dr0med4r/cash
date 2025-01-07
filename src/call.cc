#include "call.h"
#include "alias.h"
#include "cash.h"

#include <sstream>

// replace the call and insert additional arguments
void Call::resolve_alias() {
    if (!ShellBuiltinAlias::contains_alias(command)) {
        return;
    }
    std::vector<std::string> replacement =
        ShellBuiltinAlias::get_alias(command);
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

void Call::setup_fds(fd input, fd output) {
    // input from input and output to output
    // dup2 does nothing if fd and fd2 are the same
    dup2(input, STDIN_FILENO);
    close_fd(input);
    dup2(output, STDOUT_FILENO);
    close_fd(output);
}

// executes the command in the path with the current environment
//
// returns the pid of the child
//
// closes the given filedescriptors if they are not stdin or stdout
void Call::exec(fd input, fd output) {
    Cash::reset_signals();
    resolve_alias();
    char **c_args;
    c_args = (char **)malloc(sizeof(char *) * (args.size() + 2));
    c_args[0] = command.data();
    for (size_t i = 0; i < args.size(); i++) {
        c_args[i + 1] = args.at(i).data();
    }
    c_args[args.size() + 1] = nullptr;
    setup_fds(input, output);
    execvp(command.c_str(), c_args);
    std::stringstream msg;
    msg << "executing " << *this << " failed: ";
    if (errno == ENOENT) {
        msg << "command not found";
    }
    if (errno == EACCES) {
        msg << "access denied";
    }
    msg << "\n";

    free(c_args);
    throw ExecError{msg.str()};
}

void Call::add_arg(std::string arg) { args.push_back(arg); }

std::ostream &operator<<(std::ostream &os, const Call &obj) {
    os << obj.command;
    for (auto elem : obj.args) {
        os << " " << elem;
    }
    return os;
}
