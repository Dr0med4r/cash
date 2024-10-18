#include "commandline.h"
#include <array>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
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
  if (input != STDIN_FILENO) {
    dup2(input, STDIN_FILENO);
    close(input);
  }
  if (output != STDOUT_FILENO) {
    dup2(output, STDOUT_FILENO);
    close(output);
  }
  int status = execvp(command.c_str(), c_args);
  free(c_args);
  return status;
}

bool Command::set_input(fd input) {
  if (input == -1) {
    return false;
  }
  this->input = input;
  return true;
}

bool Command::set_output(fd output) {
  if (output == -1) {
    return false;
  }
  this->output = output;
  return true;
}

void Command::add_call(Call call) { calls.push_back(call); }

void Command::exec(bool wait) {
  int pid = -1;
  switch (calls.size()) {
  case 0:
    return;
  case 1: {
    pid = fork();
    if (pid == 0) {
      calls.at(0).exec(input, output);
    }
  } break;
  case 2: {
    std::array<int, 2> fd;
    int error = pipe(fd.data());
    if (error == -1) {
      exit(1);
    }
    pid = fork();
    if (pid == 0) {
      close(fd[READ_END]);
      calls.at(0).exec(input, fd[WRITE_END]);
    }
    pid = fork();
    if (pid == 0) {
      close(fd[WRITE_END]);
      calls.at(1).exec(fd[READ_END], output);
    }
    close(fd[WRITE_END]);
    close(fd[READ_END]);
  } break;
  default: {

  } break;
  }
  if (wait && pid > 0) {
    waitpid(pid, nullptr, NO_OPTION);
  }
  /* std::array<int, 2> fd; */
  /* std::array<int, 2> fd2; */
  /* int error = pipe(fd2.data()); */
  /* if (error == -1) { */
  /*   exit(1); */
  /* } */
  /**/
  /* std::vector<std::string> args = std::vector<std::string>({"-l"}); */
  /* Call a("ls", args); */
  /* int pid = fork(); */
  /* if (pid == 0) { */
  /*   close(fd[READ_END]); */
  /*   a.exec(STDIN_FILENO, fd[WRITE_END]); */
  /*   exit(1); */
  /* } */
  /* waitpid(pid, nullptr, WNOHANG); */
  /* args = std::vector<std::string>(); */
  /* a = {"sort", args}; */
  /* pid = fork(); */
  /* if (pid == 0) { */
  /*   // always close all pipe fds that are not used in exec */
  /*   close(fd[WRITE_END]); */
  /*   close(fd2[READ_END]); */
  /*   a.exec(fd[READ_END], fd2[WRITE_END]); */
  /*   exit(1); */
  /* } */
  /* // close fd */
  /* close(fd[READ_END]); */
  /* close(fd[WRITE_END]); */
  /* args = std::vector<std::string>(); */
  /* a = {"base64", args}; */
  /* pid = fork(); */
  /* if (pid == 0) { */
  /*   close(fd2[WRITE_END]); */
  /*   a.exec(fd2[READ_END], STDOUT_FILENO); */
  /*   exit(1); */
  /* } */
  /**/
  /* // close fd2 */
  /* close(fd2[READ_END]); */
  /* close(fd2[WRITE_END]); */
}
