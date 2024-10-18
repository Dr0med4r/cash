#include <array>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "commandline.h"

// currently available shell - cash
//

int main(void) {
  /* std::array<int, 2> fd; */
  /* std::array<int, 2> fd2; */
  /* int error = pipe(fd.data()); */
  /* if (error == -1) { */
  /*   exit(1); */
  /* } */
  /* error = pipe(fd2.data()); */
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
  /* std::cout << "pid: " << pid << "\n"; */
  /* int status; */
  /* waitpid(pid, &status, 0); */

  Command test;
  test.add_call(Call("ls", std::vector<std::string>({"-l"})));
  test.exec();
  test.add_call(Call("sort", std::vector<std::string>()));
  test.exec();
  std::cout << "after execution\n";
  return 0;
}
