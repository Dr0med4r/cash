%require "3.2"
%language "c++"

%define api.value.type variant
%define api.token.constructor
%parse-param {Command& command}
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
yy::parser::symbol_type yylex();
//#include "ShellState.h"
}



%start cmd_line
%nterm <Call> command simple
%nterm <Command> pipeline
%token  EXIT PIPE INPUT_REDIR OUTPUT_REDIR BACKGROUND
%token <std::string> STRING
%token NL 0


%%
cmd_line    :
        | EXIT             { }
        | pipeline back_ground
        ;

back_ground : BACKGROUND        {  }
        |                       {  }
        ;

simple      : command redir
        ;

command     : command STRING
                { $$.add_arg($STRING); std::cout <<"command, string: " << $STRING << "\n";
                }
        | STRING
                { $$ = Call($STRING);std::cout << "string: " << $STRING << "\n";
                }
        ;

redir       : input_redir output_redir
        ;

output_redir:    OUTPUT_REDIR STRING
                { 
                }
        |        /* empty */
				{
				}
        ;

input_redir:    INPUT_REDIR STRING
                {
                }
        |       /* empty */
                {
				}
        ;

pipeline    : pipeline[left] PIPE simple
                { $$ = $[left]; $$.add_call($simple); std::cout << "pipe, simple: " << $simple << "\n";
                }
        | simple
                { $$ = command; $$.add_call($simple); std::cout << "simple: " << $simple << "\n";
                }
        ;
%%

void yy::parser::error(const std::string &message)
{
    std::cerr << "Error: " << message << std::endl;
}
