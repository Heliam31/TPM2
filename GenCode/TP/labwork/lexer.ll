%{
	#include "AST.hpp"
	#include "parser.hpp"
	#include <string.h>
	#include <stdlib.h>
	void yyerror(const char *);
	const char *lexer_file = "<stdin>";
	int lexer_line = 1;
%}

id	[a-zA-Z_][a-zA-Z_0-9]*
dec	[0-9]+
hex	"0x"[0-9a-fA-F]+
bin	"0b"[01]+
syms [=@\[\]!:=<>()&|^+%/~\-\*]

%x ecom
%x ccom

%%

[ \t]	{ }
\n		{ lexer_line++; }

{syms}	{ return *yytext; }
".."	{ return DOTDOT; }
">="	{ return GE; }
"<="	{ return LE; }
"!="	{ return NE; }
"<<"	{ return LT2; }
">>"	{ return GT2; }
"<<<"	{ return LT3; }
">>>"	{ return GT3; }

"//"	{ BEGIN(ecom); }
"/*"	{ BEGIN(ccom); }

{dec}	{ yylval.INT = strtol(yytext, NULL, 10); return INT; }
{hex}	{ yylval.INT = strtol(yytext+2, NULL, 16); return INT; }
{bin}	{ yylval.INT = strtol(yytext+2, NULL, 2); return INT; }

"and"	{ return AND; }
"auto"	{ return AUTO; }
"const"	{ return CONST; }
"else"	{ return ELSE; }
"endif"	{ return ENDIF; }
"goto"	{ return GOTO; }
"if"	{ return IF; }
"not"	{ return NOT; }
"or"	{ return OR; }
"reg"	{ return REG; }
"sig"	{ return SIG; }
"state"	{ return STATE; }
"stop"	{ return STOP; }
"then"	{ return THEN; }
"var"	{ return VAR; }
"when"	{ return WHEN; }
{id}	{ yylval.ID = strdup(yytext); return ID; }

.		{ yyerror("bad character"); }

<ecom>\n	{ lexer_line++; BEGIN(INITIAL); }
<ecom>.		{ }

<ccom>"*/"	{ BEGIN(INITIAL); }
<ccom>\n	{ lexer_line++; }
<ccom>.		{ }

%%


int yywrap(void) {return 1;}

