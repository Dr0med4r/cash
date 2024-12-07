#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <readline/history.h>
#include <readline/readline.h>

#include "../parse.hh"
#include "builtins.h"
#include "commandline.h"
#include "errors.h"

// currently available shell - cash
//

extern std::string yyinput;

int main(void) {
    struct sigaction act = {};
    act.sa_handler = SIG_IGN;
    if (sigaction(SIGINT, &act, NULL) == -1) {
        std::cerr << "set sigaction failed\n";
        exit(1);
    }
    using_history();
    const int history_length = 10000;
    stifle_history(history_length);
    while (true) {
        char* input_char = readline("$ ");
        if (!input_char) {
            exit(0);
        }
        std::string input = input_char;
        char* output_string;
        int success = history_expand(input.data(), &output_string);
        if (success == -1) {
            std::cerr << "there was an error while expanding history\n";
        }
        std::string result = output_string;
        if (result != "") {
            add_history(result.data());
        }
        // the result of readline does not contain a newline at the end but the
        // parser expects one but add it after adding the result to the history
        result.push_back('\n');
        yyinput = result;
        Command test;
        auto parser = yy::parser(&test);
        int status = 1;
        try {
            status = parser.parse();
        } catch (FileError& e) {
            std::cerr << e.what();
        }
        if (status == 0) {
            std::cout << test << "\n";
            test.exec();
        }
    }
    return 0;
}
