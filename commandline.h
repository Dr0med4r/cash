#pragma once
#include "errors.h"
#include <exception>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>
typedef int fd;
#define WRITE_END 1
#define READ_END 0
#define NO_OPTION 0

// close fd if not stdin or stdout
//
// returns -1 on close error
// 0 on success and 1 on stdfile
inline int close_fd(int fd) {
    if (fd != STDIN_FILENO && fd != STDOUT_FILENO) {
        return close(fd);
    }
    return 1;
}

class Call {
protected: 
    std::string command;
    std::vector<std::string> args;

  public:
    Call(const Call &) = default;
    Call(Call &&) = default;
    Call &operator=(const Call &) = default;
    Call &operator=(Call &&) = default;
    Call(std::string call = "", std::vector<std::string> args = std::vector<std::string>{})
        : command(std::move(call)), args(std::move(args)) {}
    virtual ~Call(){};

    friend std::ostream& operator<<(std::ostream& os, const Call& obj);
    virtual void exec(fd input, fd output);
    void set_call(std::string call);
    void add_arg(std::string arg);
};


class Command {
    std::vector<std::unique_ptr<Call>> calls;
    fd input = STDIN_FILENO;
    fd output = STDOUT_FILENO;
    bool wait = true;

  public:
    Command(const Command &) = default;
    Command(Command &&) = default;
    Command &operator=(const Command &) = default;
    Command &operator=(Command &&) = default;
    Command() {}
    ~Command() = default;

    friend std::ostream& operator<<(std::ostream& os, const Command& obj);
    void set_input(fd input);
    void set_output(fd output);
    bool has_valid_fds();
    void add_call(std::unique_ptr<Call> call);
    void exec();
    void set_background(bool wait);
};
