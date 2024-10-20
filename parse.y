/* -*- indented-text -*- */

%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "ShellState.h"

int yylex(void);
extern "C" void yyerror(char *s);

%}

%union {
       char *string;
}


%start cmd_line
%token <string> EXIT PIPE INPUT_REDIR OUTPUT_REDIR STRING NL BACKGROUND


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
                { $$.add_arg($STRING)
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

pipeline    : pipeline PIPE simple
                { $$ = $[left]; $$.add_call($simple)
                }
        | simple
                { $$ = Command(); $$.add_call($simple)
                }
        ;
%%

