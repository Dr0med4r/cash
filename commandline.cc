#include "commandline.h"
#include <array>
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include <sys/wait.h>
#include <unistd.h>

// close fd if not stdin or stdout
//
// returns -1 on close error
// 0 on success and 1 on stdfile
int close_fd(int fd) {
  if (fd != STDIN_FILENO && fd != STDOUT_FILENO) {
    return close(fd);
  }
  return 1;
}

// executes the command in the path with the current environment
//
// returns the pid of the child
//
// closes the given filedescriptors if they are not stdin or stdout
int Call::exec(fd input, fd output) {
  char **c_args;
  c_args = (char **)malloc(sizeof(char *) * (args.size() + 2));
  c_args[0] = command.data();
  for (size_t i = 0; i < args.size(); i++) {
    c_args[i + 1] = args.at(i).data();
  }
  c_args[args.size() + 1] = nullptr;
  // input from input and output to output
  // dup2 does nothing if fd and fd2 are the same
  dup2(input, STDIN_FILENO);
  close_fd(input);
  dup2(output, STDOUT_FILENO);
  close_fd(output);
  int status = execvp(command.c_str(), c_args);
  free(c_args);
  return status;
}

void Command::set_input(fd input) {
  if (fcntl(input, F_GETFD) == -1) {
    throw std::runtime_error{"not a valid fd"};
  }
  this->input = input;
}

void Command::set_output(fd output) {
  if (fcntl(input, F_GETFD) == -1) {
    throw std::runtime_error{"not a valid fd"};
  }
  this->output = output;
}

void Command::add_call(Call call) { calls.push_back(call); }
bool Command::has_valid_fds() { return input != -1 && output != -1; }

// executes all given commands with pipes and the given input and output
// redirection
//
// closes input and output fds if not stdfile
void Command::exec(bool wait) {
  if (!has_valid_fds()) {
    throw ExecError{"the output or input is not set"};
  }
  int pid = -1;
  switch (calls.size()) {
  case 0:
    return;
  default: {
    std::array<int, 2> fd1 = {input, 0};
    std::array<int, 2> fd2 = {0, output};
    std::array<std::array<int, 2>, 2> fds = {fd1, fd2};
    int which_pipe = 0;
    // alternate between the two pipes as input and output
    for (size_t i = 0; i < calls.size(); i++) {
      which_pipe = 1 - which_pipe;
      int this_pipe = which_pipe;
      int other_pipe = 1 - which_pipe;
      int error = pipe(fds[this_pipe].data());
      if (error == -1) {
        throw ExecError{"error creating pipe"};
      }
      if (i == calls.size() - 1) {
        fds[this_pipe][WRITE_END] = output;
      }
      pid = fork();
      if (pid == -1) {
        throw ExecError{"error forking"};
      }
      if (pid == 0) {
        close_fd(fds[other_pipe][WRITE_END]);
        close_fd(fds[this_pipe][READ_END]);
        calls.at(i).exec(fds[other_pipe][READ_END], fds[this_pipe][WRITE_END]);
      }
      for (int fd : fds[other_pipe]) {
        close_fd(fd);
      }
    }
    for (auto arr : fds) {
      for (int fd : arr) {
        close_fd(fd);
      }
    }

  } break;
  }
  // as the input and output are closed now do not use them again
  input = -1;
  output = -1;
  if (wait && pid > 0) {
    waitpid(pid, nullptr, NO_OPTION);
  }
}
