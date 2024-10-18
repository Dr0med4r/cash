#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>

#include "commandline.h"

// currently available shell - cash
//

int main(void) {

  Command test;
  int input = open("lol", O_RDONLY);
  test.set_input(input);
  int output = open("lel", O_WRONLY | O_CREAT | O_TRUNC,
                    S_IRUSR | S_IWUSR | S_IROTH | S_IRGRP);
  test.set_output(output);
  test.add_call(Call("echo", std::vector<std::string>()));
  test.exec();
  test.exec();
  std::cout << "after execution\n";
  return 0;
}
