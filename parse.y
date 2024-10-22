%require "3.2"
%language "c++"

%define api.value.type variant
%define api.token.constructor
// %define api.header.include 

/* -*- indented-text -*- */
%code requires
{
#include "commandline.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "commandline.h"
//#include "ShellState.h"

int yylex(void);
extern "C" void yyerror(char *s);

Command commandline();

%}



%start cmd_line
%nterm <Call> command simple
%nterm <Command> pipeline
%token <std::string> EXIT PIPE INPUT_REDIR OUTPUT_REDIR STRING NL BACKGROUND


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
                { $$ = Command(); $$.add_call($simple);
                }
        ;
%%

