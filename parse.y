%require "3.2"
%language "c++"

%define api.value.type variant
%define api.token.constructor
%parse-param {Command* command}
//%lex-param{str}
%output "parse.cc"
// %define api.header.include 

/* -*- indented-text -*- */
%code requires
{
#include "commandline.h"
}

%code 
{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
yy::parser::symbol_type yylex();
int open_input(std::string file);
int open_output(std::string file);
//#include "ShellState.h"
}



%start cmd_line
%nterm <Call> command simple
%nterm <Command*> pipeline
%token  EXIT PIPE INPUT_REDIR OUTPUT_REDIR BACKGROUND
%token <std::string> STRING
%token NL 0


%%
cmd_line    :
        | EXIT             { }
        | pipeline back_ground
        ;

back_ground : BACKGROUND        { 
                                    command->set_background(true);
                                }
        |                       {  }
        ;

simple      : command redir
        ;

command     : command[left] STRING
                { 
                    $$ = std::move($left); $$.add_arg($STRING);
                }
        | STRING
                { 
                    $$ = Call($STRING);
                }
        ;

redir       : input_redir output_redir
        ;

output_redir:    OUTPUT_REDIR STRING
                { 
                    int output = open_output($STRING);
                    command->set_output(output);
                }
        |        /* empty */
				{
				}
        ;

input_redir:    INPUT_REDIR STRING
                {
                    int input = open_input($STRING);
                    command->set_input(input);
                }
        |       /* empty */
                {
				}
        ;

pipeline    : pipeline[left] PIPE simple
                { 
                    $$ = std::move($[left]); $$->add_call($simple);
                }
        | simple
                { 
                    $$ = std::move(command); $$->add_call($simple);
                }
        ;
%%

void yy::parser::error(const std::string &message)
{
    std::cerr << "Error: " << message << std::endl;
}

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
