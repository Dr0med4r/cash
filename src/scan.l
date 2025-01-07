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

std::string yyinput = "";

#define YY_INPUT(buf,result,max_size) \
    {\
        if (yyinput.length() == 0) {\
            result = YY_NULL;\
        }\
        buf[0] = yyinput.at(0);\
        yyinput.erase(0,1);\
        result = 1;\
    }
%}

%option noyywrap noinput nounput
%option outfile="scan.cc"

NEW_LINE                [\n]
PIPE                    [|]
OUTPUT_REDIRECTION      [>]
INPUT_REDIRECTION       [<]
BG                      [&]
CHAR                    [0-9a-zA-Z_/\-\*\.]
SPACE                   [\032]
%% 

exit                    { return yy::parser::make_EXIT(); }
{NEW_LINE}              { return yy::parser::make_NEW_LINE(); }
{BG}                    { return yy::parser::make_BACKGROUND(); }
{PIPE}                  { return yy::parser::make_PIPE();}
{OUTPUT_REDIRECTION}    { return yy::parser::make_OUTPUT_REDIRECTION(); }
{INPUT_REDIRECTION}     { return yy::parser::make_INPUT_REDIRECTION(); }
{CHAR}+                 { return yy::parser::make_STRING(yytext); }
<<EOF>>                 { return yy::parser::make_EXIT(); }
.                   ;
%%
