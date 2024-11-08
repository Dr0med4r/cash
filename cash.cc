#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>

#include "commandline.h"
#include "errors.h"
#include "parse.hh"

// currently available shell - cash
//

int main(void) {
    /* int input = open_input("lol"); */
    /* test.set_input(input); */
    /* int output = open_output("lwwl"); */
    /* test.set_output(output); */
    /* test.add_call(Call("echo", std::vector<std::string>{"hi"})); */
    /* test.exec(); */
    while (true) {
        std::cout << "$ ";
        Command test;
        auto parser = yy::parser(&test);
        int status = 1;
        try {
            status = parser.parse();
        } catch (FileError &e) {
            std::cerr << e.what();
        }
        if (status == 0) {
            std::cout << test << "\n";
            test.exec();
        }
    }
    return 0;
}
