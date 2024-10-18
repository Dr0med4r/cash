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
  Call(std::string call, std::vector<std::string> args)
      : command(std::move(call)), args(std::move(args)) {}
  ~Call() = default;

  int exec(fd input, fd output);
};

class Command {
  std::vector<Call> calls;
  fd input = STDIN_FILENO;
  fd output = STDOUT_FILENO;

public:
  Command(const Command &) = default;
  Command(Command &&) = default;
  Command &operator=(const Command &) = default;
  Command &operator=(Command &&) = default;
  Command() {}
  ~Command() = default;
  bool set_input(fd input);
  bool set_output(fd output); 
  bool has_valid_output();
  void add_call(Call call);
  void exec(bool wait = true);
};
