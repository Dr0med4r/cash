/* -*- indented-text -*- */
%{
#include <string.h>
#include "parse.hh"
#include "scan.h"

#ifdef DOESNT_HAVE_STRDUP
#warning DOESNT_HAVE_STRDUP
char *strdup(const char *s);
#endif
#undef YY_DECL
#define YY_DECL int Scan::yylex(std::string *const yylval)

#define ECHO
%}

%option noyywrap
%option c++
%option yyclass="Scan"

NL              [\n]
PIPE            [|]
OUTPUT_REDIR    [>]
INPUT_REDIR     [<]
BG              [&]
CHAR            [0-9a-zA-Z_/\-\*\.]
SPACE           [\032]
%% 

exit            { ECHO; return yy::parser::token::EXIT; }
{NL}            { ECHO; return 0; }
{BG}            { ECHO; return yy::parser::token::BACKGROUND; }
{PIPE}          { ECHO; return yy::parser::token::PIPE;}
{OUTPUT_REDIR}  { ECHO; return yy::parser::token::OUTPUT_REDIR; }
{INPUT_REDIR}   { ECHO; return yy::parser::token::INPUT_REDIR; }
{CHAR}+         { ECHO; *yylval = std::string(strdup(yytext)); return yy::parser::token::STRING; }
.               ;
%%
