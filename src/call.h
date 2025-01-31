#pragma once
#include <string>
#include <vector>
#include <set>

typedef int fd;

class Call {
  protected:
    std::string command;
    std::vector<std::string> args;
    void setup_fds(fd input, fd output);

  public:
    Call(const Call &) = default;
    Call(Call &&) = default;
    Call &operator=(const Call &) = default;
    Call &operator=(Call &&) = default;
    Call(std::string call = "",
         std::vector<std::string> args = std::vector<std::string>{})
        : command(std::move(call)), args(std::move(args)) {}
    virtual ~Call() {};

    friend std::ostream &operator<<(std::ostream &os, const Call &obj);
    virtual void exec(fd input, fd output);
    void set_call(std::string call);
    void add_arg(std::string arg);

  private:
    void resolve_alias(std::set<std::string> already_resolved);
};
