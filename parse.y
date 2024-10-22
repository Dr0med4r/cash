/* -*- indented-text -*- */
%code requires
{
#include "commandline.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include "ShellState.h"

int yylex(void);
extern "C" void yyerror(char *s);

%}


%d -----------------------------------------------------------

%start cmd_line
%nterm <
%token <char*> EXIT PIPE INPUT_REDIR OUTPUT_REDIR STRING NL BACKGROUND


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
                { $<Call>$.add_arg($STRING)
                }
        | STRING
                { $<Call>$ = Call($STRING);
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
                { $<Command>$ = $<Command>[left]; $<Command>$.add_call($<Call>simple)
                }
        | simple
                { $<Command>$ = Command(); $<Command>$.add_call($<Call>simple)
                }
        ;
%%

