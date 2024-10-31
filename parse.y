%require "3.2"
%language "c++"

%define api.value.type variant
%define api.token.constructor
%parse-param {param params}
%lex-param{str}
%output "parse.cc"
// %define api.header.include 

/* -*- indented-text -*- */
%code requires
{
#include "commandline.h"
#include "scan.h"
typedef struct param {
        Scan& lexer;
        Command& command;
} param;
}

%code 
{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "commandline.h"
//#include "ShellState.h"

#define yylex params.lexer.yylex
std::string *const str = new std::string{};

Command commandline();

}



%start cmd_line
%nterm <Call> command simple
%nterm <Command> pipeline
%token  EXIT PIPE INPUT_REDIR OUTPUT_REDIR NL BACKGROUND
%token <std::string> STRING


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
                { $$.add_arg($STRING);
                }
        | STRING
                { $$ = Call($STRING);
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
                { $$ = $[left]; $$.add_call($simple);
                }
        | simple
                { $$ = params.command; $$.add_call($simple);
                }
        ;
%%

void yy::parser::error(const std::string &message)
{
    std::cerr << "Error: " << message << std::endl;
}
