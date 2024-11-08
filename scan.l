/* -*- indented-text -*- */
%{

#include "parse.hh"
#include <string.h>
#undef YY_DECL
#define YY_DECL yy::parser::symbol_type yylex()
YY_DECL;

#ifdef DOESNT_HAVE_STRDUP
#warning DOESNT_HAVE_STRDUP
char *strdup(const char *s);
#endif

#define ECHO
%}

%option noyywrap noinput nounput
%option outfile="scan.cc"

NL              [\n]
PIPE            [|]
OUTPUT_REDIR    [>]
INPUT_REDIR     [<]
BG              [&]
CHAR            [0-9a-zA-Z_/\-\*\.]
SPACE           [\032]
%% 

exit            { return yy::parser::make_EXIT(); }
{NL}            { return yy::parser::make_NL(); }
{BG}            { return yy::parser::make_BACKGROUND(); }
{PIPE}          { return yy::parser::make_PIPE();}
{OUTPUT_REDIR}  { return yy::parser::make_OUTPUT_REDIR(); }
{INPUT_REDIR}   { return yy::parser::make_INPUT_REDIR(); }
{CHAR}+         { return yy::parser::make_STRING(yytext); }
<<EOF>>         { return yy::parser::make_EXIT(); }
.               ;
%%
