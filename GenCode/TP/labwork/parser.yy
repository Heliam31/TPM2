%{
	#include <iostream>
	#include <vector>
	#include <algorithm>
	#include "AST.hpp"
	using namespace std;

	extern int lexer_line;
	extern const char *lexer_file;

	int yylex();
	void yyerror(char const *msg) {
		throw ParseException(Position(), msg);
	}

	vector<State *> states;
	vector<When *> whens;

	Declaration *checkLoc(string name, int line) {
		auto d = Declaration::getSymbol(name);
		if(d == nullptr)
			throw ParseException(Position(line), name + " does not exist.");
		if(d->type() != Declaration::REG
		&& d->type() != Declaration::VAR)
			throw ParseException(Position(line), name + " should be a register or a variable.");
		return d;
	}

	Declaration *checkMem(string name, int line) {
		auto d = Declaration::getSymbol(name);
		if(d == nullptr)
			throw ParseException(Position(line), name + " does not exist.");
		if(d->type() != Declaration::REG
		&& d->type() != Declaration::VAR
		&& d->type() != Declaration::CST)
			throw ParseException(Position(line), name + " should be a register, a variable or a constant.");
		return d;
	}
%}

%define api.value.type union
%token<char *> ID
%token<long int> INT
%type<int> line
%type<Expression *> expr
%type<Statement *> opt_stmts stmts stmt
%type<bool> opt_not
%type<Condition *> cond


%token DOTDOT
%token GE
%token LE
%token NE

%token AND
%token AUTO
%token CONST
%token ELSE
%token ENDIF
%token GOTO
%token GT2
%token GT3
%token IF
%token LT2
%token LT3
%token NOT
%token OR
%token REG
%token SIG
%token STATE
%token STOP
%token THEN
%token VAR
%token WHEN

%nonassoc NOT
%nonassoc '~'
%nonassoc '['

%left OR
%left AND

%left '+' '-'
%left '*' '/' '%'
%left '&'
%left '|' '^'
%left LT2 GT2 LT3 GT3

%%

top: opt_decls
	{ }
;

line:
	%empty
		{ $$ = lexer_line; }

opt_decls:
	%empty
		{ }
|	decls
		{ }
;

decls:
	decl
		{ }
|	decls decl
		{ }
;

decl:

	CONST line ID '=' expr
		{
			auto x = $5->eval();
			if(!x)
				throw ParseException($5->pos, "should be a constant!");
			delete $5;
			(new ConstDecl($3, *x))->setLine($2);
			free($3);
		}

|	VAR line ID
		{ (new VarDecl($3))->setLine($2); free($3); }

|	REG line ID '@' expr
		{
			auto x = $5->eval();
			if(!x)
				throw ParseException($5->pos, "should be a constant!");
			delete $5;
			(new RegDecl($3, *x))->setLine($2);
			free($3);
		}

|	SIG line ID '@' ID '[' expr ']'
		{
			auto dec = Declaration::getSymbol($5);
			if(dec == nullptr)
				throw ParseException(Position($2), string($5) + " does not exist!");
			if(dec->type() != Declaration::REG)
				throw ParseException(Position($2), string($5) + " must be a register.");
			auto x = $7->eval();
			if(!x)
				throw ParseException($7->pos, "bit number should be a constant!");
			if(*x >= 32)
				throw ParseException($7->pos, "bit number must be less than 32!");
			(new SigDecl($3, static_cast<RegDecl *>(dec), *x))->setLine($2);
			delete $7;
			free($3);
			free($5);
		}

|	AUTO line ID opt_stmts states
		{
			$4->fix(states);
			for(auto s: states)
				s->fix(states);
			(new AutoDecl($3, $4, states))->setLine($2);
			free($3);
			states.clear();
		}
;

states:
	state
		{ }
|	states state
		{ }
;

state:
	STATE line ID ':' opt_stmts opt_whens
		{
			for(auto s: states)
				if(s->name() == $3)
					throw ParseException(Position($2), string(" state ") + $3 + " already exists.");
			auto s = new State($3, $5, whens);
			s->setLine($2);
			states.push_back(s);
			whens.clear();
			free($3);
		}
;

opt_whens:
	%empty
		{ }
|	whens
		{ }
;

whens:
	when
		{ }
|	whens when
		{ }
;

when:
	WHEN line opt_not ID ':' opt_stmts
		{
			auto s = Declaration::getSymbol($4);
			if(s == nullptr)
				throw ParseException(Position($2), string($4) + " does not exist.");
			if(s->type() != Declaration::SIG)
				throw ParseException(Position($2), string($4) + " should be a singal!");
			auto w = new When($3, static_cast<SigDecl *>(s), $6);
			w->setLine($2);
			whens.push_back(w);
		}
;

opt_not:
	%empty
		{ $$ = false; }
|	'!'
		{ $$ = true; }
;

opt_stmts:
	%empty
		{ $$ = new NOPStatement(); }
|	stmts
		{ $$ = $1; }
;

stmts:
	stmt
		{ $$ = $1; }
|	stmts stmt
		{ $$ = new SeqStatement($1, $2); }
;

stmt:

	ID line '=' expr
		{
			auto d = checkLoc($1, $2);
			$$ = new SetStatement(d, $4);
			$$->setLine($2);
			free($1);
		}

|	ID line '[' expr ']' '=' expr
		{
			auto d = checkLoc($1, $2);
			$$ = new SetFieldStatement(d, $4, $4, $7);
			$$->setLine($2);
			free($1);
		}
|	ID line '[' expr DOTDOT expr ']' '=' expr
		{
			auto d = checkLoc($1, $2);
			$$ = new SetFieldStatement(d, $4, $6, $9);
			$$->setLine($2);
			free($1);
		}
|	IF line cond THEN stmts ENDIF
		{ $$ = new IfStatement($3, $5, new NOPStatement()); $$->setLine($2); }
|	IF line cond THEN stmts ELSE stmts ENDIF
		{ $$ = new IfStatement($3, $5, $7); $$->setLine($2); }
|	GOTO line ID
		{ $$ = new GotoStatement($3); $$->setLine($2); }
|	STOP
		{ $$ = new StopStatement(); }
;

cond:
	expr '=' line expr
		{ $$ = new CompCond(CompCond::EQ, $1, $4); $$->setLine($3); }
|	expr NE line expr
		{ $$ = new CompCond(CompCond::NE, $1, $4); $$->setLine($3); }
|	expr '<' line expr
		{ $$ = new CompCond(CompCond::LT, $1, $4); $$->setLine($3); }
|	expr LE line expr
		{ $$ = new CompCond(CompCond::LE, $1, $4); $$->setLine($3); }
|	expr '>' line expr
		{ $$ = new CompCond(CompCond::GT, $1, $4); $$->setLine($3); }
|	expr GE line expr
		{ $$ = new CompCond(CompCond::GE, $1, $4); $$->setLine($3); }
|	NOT line cond
		{ $$ = new NotCond($3); $$->setLine($2); }
|	cond AND line cond
		{ $$ = new AndCond($1, $4); $$->setLine($3); }
|	cond OR line cond
		{ $$ = new OrCond($1, $4); $$->setLine($3); }
|	'(' cond ')'
		{ $$ = $2; }
;

expr:
	INT line
		{ $$ = new ConstExpr($1); $$->setLine($2); }
|	ID line
		{
			auto d = checkMem($1, $2);
			$$ = new MemExpr(d);
			$$->setLine($2);
			free($1);
		}

|	expr '[' expr ']'
		{ $$ = new BitFieldExpr($1, $3, $3); $$->pos = $1->pos;  }

|	expr '[' expr DOTDOT expr ']'
		{ $$ = new BitFieldExpr($1, $3, $5); $$->pos = $1->pos;  }

|	line '~' expr
		{ $$ = new UnopExpr(UnopExpr::INV, $3); $$->setLine($1); }

|	line '-' expr
		{ $$ = new UnopExpr(UnopExpr::NEG, $3); $$->setLine($1); }

|	'+' expr
		{ $$ = $2; }
|	expr '+' line expr
		{ $$ = new BinopExpr(BinopExpr::ADD, $1, $4); $$->setLine($3); }
|	expr '-' line expr
		{ $$ = new BinopExpr(BinopExpr::SUB, $1, $4); $$->setLine($3); }
|	expr '*' line expr
		{ $$ = new BinopExpr(BinopExpr::MUL, $1, $4); $$->setLine($3); }
|	expr '/' line expr
		{ $$ = new BinopExpr(BinopExpr::DIV, $1, $4); $$->setLine($3); }
|	expr '%' line expr
		{ $$ = new BinopExpr(BinopExpr::MOD, $1, $4); $$->setLine($3); }
|	expr '&' line expr
		{ $$ = new BinopExpr(BinopExpr::BIT_AND, $1, $4); $$->setLine($3); }
|	expr '|' line expr
		{ $$ = new BinopExpr(BinopExpr::BIT_OR, $1, $4); $$->setLine($3); }
|	expr '^' line expr
		{ $$ = new BinopExpr(BinopExpr::XOR, $1, $4); $$->setLine($3); }
|	expr LT2 line expr
		{ $$ = new BinopExpr(BinopExpr::SHL, $1, $4); $$->setLine($3); }
|	expr GT2 line expr
		{ $$ = new BinopExpr(BinopExpr::SHR, $1, $4); $$->setLine($3); }
|	expr LT3 line expr
		{ $$ = new BinopExpr(BinopExpr::ROL, $1, $4); $$->setLine($3); }
|	expr GT3 line expr
		{ $$ = new BinopExpr(BinopExpr::ROR, $1, $4); $$->setLine($3); }
|	'(' expr ')'
		{ $$ = $2; }
;
