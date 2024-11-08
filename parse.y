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
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <string>  
#include <iostream> 
#include <sstream> 
#include "errors.h"
#include "shellcall.h"
yy::parser::symbol_type yylex();
int open_input(std::string file);
int open_output(std::string file);
//#include "ShellState.h"
}



%start cmd_line
%nterm <Call*> command simple
%nterm <Command*> pipeline
%token  EXIT PIPE INPUT_REDIR OUTPUT_REDIR BACKGROUND
%token <std::string> STRING
%token NL 0


%%
cmd_line    :
        | EXIT { 
        exit(0);
        }
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
                    $$ = std::move($left); $$->add_arg($STRING);
                }
        | STRING
                { 
                    if ($STRING == "cd") {
                        $$ = new ShellCallCd($STRING);
                    }
                    else if ($STRING == "pwd") {
                        $$ = new ShellCallPwd($STRING);
                    }
                    else if ($STRING == "alias" || $STRING == "unalias") {
                        
                    }
                    else {
                        $$ = new Call($STRING);
                    }
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
                    $$ = std::move($[left]); $$->add_call(std::unique_ptr<Call>($simple));
                }
        | simple
                { 
                    $$ = std::move(command); $$->add_call(std::unique_ptr<Call>($simple));
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
        std::stringstream msg;
        msg << "open " << file << " failed: " << std::strerror(errno)
                  << "\n";
        throw FileError{msg.str()};
    }
    return input;
}

int open_output(std::string file) {
    int output = open(file.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
                      S_IRUSR | S_IWUSR | S_IROTH | S_IRGRP);
    if (output == -1) {
        std::stringstream msg;
        msg << "open " << file << " failed: " << std::strerror(errno)
                  << "\n";
        throw FileError{msg.str()};
    }
    return output;
}
