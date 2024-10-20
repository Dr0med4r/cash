#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>

#include "parse.h"
#include "commandline.h"

// currently available shell - cash
//

int open_input(std::string file) {
    int input = open(file.c_str(), O_RDONLY);
    if (input == -1) {
        std::cerr << "open " << file << " failed: " << std::strerror(errno)
                  << "\n";
    }
    return input;
}

int open_output(std::string file) {
    int output = open(file.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
                      S_IRUSR | S_IWUSR | S_IROTH | S_IRGRP);
    if (output == -1) {
        std::cerr << "open " << file << " failed: " << std::strerror(errno)
                  << "\n";
    }
    return output;
}

int main(void) {
    
    int status = yyparse();
    /* Command test; */
    /* int input = open_input("lol"); */
    /* test.set_input(input); */
    /* int output = open_output("lwwl"); */
    /* test.set_output(output); */
    /* test.add_call(Call("echo", std::vector<std::string>{"hi"})); */
    /* test.exec(); */
    /* std::cout << "after execution\n"; */
    return 0;
}
