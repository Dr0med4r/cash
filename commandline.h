#ifndef CASH_COMMANDLINE_H
#define CASH_COMMANDLINE_H
#include <exception>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>
typedef int fd;
#define WRITE_END 1
#define READ_END 0
#define NO_OPTION 0

class Call {
    std::string command;
    std::vector<std::string> args;

  public:
    Call(const Call &) = default;
    Call(Call &&) = default;
    Call &operator=(const Call &) = default;
    Call &operator=(Call &&) = default;
    Call(std::string call = "", std::vector<std::string> args = std::vector<std::string>{})
        : command(std::move(call)), args(std::move(args)) {}
    ~Call() = default;

    friend std::ostream& operator<<(std::ostream& os, const Call& obj);
    int exec(fd input, fd output);
    void set_call(std::string call);
    void add_arg(std::string arg);
};

class ExecError : public std::runtime_error {
  public:
    ExecError(const std::string &msg) : std::runtime_error(msg) {};
    ExecError(ExecError &&) = default;
    ExecError(const ExecError &) = default;
    ExecError &operator=(ExecError &&) = default;
    ExecError &operator=(const ExecError &) = default;
    ~ExecError() = default;

  private:
};

class Command {
    std::vector<Call> calls;
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
    void add_call(Call call);
    void exec();
    void set_background(bool wait);
};
#endif // !CASH_COMMANDLINE_H
