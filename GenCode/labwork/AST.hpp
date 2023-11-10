#ifndef IOC_AST_CPP
#define IOC_AST_CPP

#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "Quad.hpp"

using namespace std;

typedef unsigned long value_t;

class AutoDecl;
class Expression;
class Statement;
class Condition;
class Declaration;
class State;
class When;

class Position {
public:
	Position();
	Position(int line);
	const char *file;
	int line;
	inline string to_str() const { return string(file) + ":" + to_string(line); }
	inline operator string() const { return to_str(); }
};
ostream& operator<<(ostream& out, const Position& pos);

class AST {
public:
	virtual ~AST() { }
	virtual void print(ostream& out) const = 0;
	void setLine(int line) { pos.line = line; }
	Position pos;
};
ostream& operator<<(ostream& out, const AST *ast);

class ParseException {
public:
	inline ParseException(Position pos, string msg): _pos(pos), _msg(msg) {}
	inline Position pos() const { return _pos; }
	inline string msg() const { return _msg; }
private:
	Position _pos;
	string _msg;
};


/****** Statements ******/

class Statement: public AST {
public:
	typedef enum {
		NOP,
		SEQ,
		SET,
		SET_FIELD,
		IF,
		GOTO,
		STOP
	} type_t;

	inline Statement(type_t type) {}
	virtual ~Statement() {}
	virtual void fix(const vector<State *>& states);
	inline type_t type() const { return _type; }
	virtual void reduce() = 0;
	virtual void gen(AutoDecl& automaton, QuadProgram& prog) const = 0;

private:
	type_t _type;
};
inline ostream& operator<<(ostream& out, Statement *stmt) { stmt->print(out); return out; }

class NOPStatement: public Statement {
public:
	inline NOPStatement(): Statement(NOP) {}
	void print(ostream& out) const override;
	void reduce() override;
	void gen(AutoDecl& automaton, QuadProgram& prog) const override;
};

class SeqStatement: public Statement {
public:
	SeqStatement(Statement *stmt1, Statement *stmt2)
		: Statement(SEQ), _stmt1(stmt1), _stmt2(stmt2) {}
	~SeqStatement();
	void print(ostream& out) const override;
	void fix(const vector<State *>& states) override;
	void reduce() override;
	void gen(AutoDecl& automaton, QuadProgram& prog) const override;
private:
	Statement *_stmt1, *_stmt2;
};

class SetStatement: public Statement {
public:
	inline SetStatement(Declaration *dec, Expression *expr)
		: Statement(SET), _dec(dec), _expr(expr) {}
	~SetStatement();
	void print(ostream& out) const override;
	void reduce() override;
	void gen(AutoDecl& automaton, QuadProgram& prog) const override;
private:
	Declaration *_dec;
	Expression *_expr;
};

class SetFieldStatement: public Statement {
public:
	inline SetFieldStatement(Declaration *dec, Expression *hi, Expression *lo, Expression *expr)
		: Statement(SET_FIELD), _dec(dec), _hi(hi), _lo(lo),  _expr(expr) {}
	~SetFieldStatement();
	void print(ostream& out) const override;
	void reduce() override;
	void gen(AutoDecl& automaton, QuadProgram& prog) const override;
private:
	Declaration *_dec;
	Expression *_hi, *_lo, *_expr;
};

class IfStatement: public Statement {
public:
	inline IfStatement(Condition *cond, Statement *stmt1, Statement *stmt2)
		: Statement(IF), _cond(cond), _stmt1(stmt1), _stmt2(stmt2) { }
	~IfStatement();
	void print(ostream& out) const override;
	void fix(const vector<State *>& states) override;
	void reduce() override;
	void gen(AutoDecl& automaton, QuadProgram& prog) const override;

private:
	Condition *_cond;
	Statement *_stmt1, *_stmt2;
};

class GotoStatement: public Statement {
public:
	inline GotoStatement(string id): Statement(GOTO), _id(id), _state(nullptr) {}
	void print(ostream& out) const override;
	void fix(const vector<State *>& states) override;
	void reduce() override;
	void gen(AutoDecl& automaton, QuadProgram& prog) const override;
private:
	string _id;
	State *_state;
};

class StopStatement: public Statement {
public:
	inline StopStatement(): Statement(STOP) {}
	void print(ostream& out) const  override;
	void reduce() override;
	void gen(AutoDecl& automaton, QuadProgram& prog) const override;
};


/****** Expressions ******/

class Expression: public AST {
public:

	typedef enum {
		NONE,
		CST,
		MEM,
		BITFIELD,
		UNOP,
		BINOP
	} type_t;

	inline Expression(type_t type): _type(type) { }
	virtual ~Expression() {}
	inline type_t type() const { return _type; }
	virtual optional<value_t> eval() const = 0;
	virtual Expression *reduce() = 0;
	virtual Quad::reg_t gen(QuadProgram& prog) = 0;

	static const Expression *none;
private:
	type_t _type;
};

class ConstExpr: public Expression {
public:
	inline ConstExpr(value_t val)
		: Expression(CST), _val(val) {}
	inline value_t value() const { return _val; }
	void print(ostream& out) const override;
	optional<value_t> eval() const override;
	Expression *reduce() override;
	Quad::reg_t gen(QuadProgram& prog) override;

private:
	value_t _val;
};

class MemExpr: public Expression {
public:
	inline MemExpr(Declaration *dec): Expression(MEM), _dec(dec) { }
	void print(ostream& out) const override;
	optional<value_t> eval() const override;
	Expression *reduce() override;
	Quad::reg_t gen(QuadProgram& prog) override;
private:
	Declaration *_dec;
};

class BitFieldExpr: public Expression {
public:
	inline BitFieldExpr(Expression *expr, Expression *hi, Expression *lo)
		: Expression(BITFIELD), _expr(expr), _hi(hi), _lo(lo) { }
	~BitFieldExpr();
	void print(ostream& out) const override;
	optional<value_t> eval() const override;
	Expression *reduce() override;
	Quad::reg_t gen(QuadProgram& prog) override;
private:
	Expression *_expr, *_hi, *_lo;
};

class UnopExpr: public Expression {
public:
	typedef enum {
		NEG,
		INV
	} unop_t;

	inline UnopExpr(unop_t op, Expression *arg)
		: Expression(UNOP), _op(op), _arg(arg) {}
	~UnopExpr();
	void print(ostream& out) const override;
	optional<value_t> eval() const override;
	Expression *reduce() override;
	Quad::reg_t gen(QuadProgram& prog) override;
private:
	unop_t _op;
	Expression *_arg;
};

class BinopExpr: public Expression {
public:
	typedef enum {
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,
		BIT_AND,
		BIT_OR,
		XOR,
		SHL,
		SHR,
		ROL,
		ROR
	} op_t;

	inline BinopExpr(op_t op, Expression *arg1, Expression *arg2)
		: Expression(BINOP), _op(op), _arg1(arg1), _arg2(arg2) {}
	~BinopExpr();
	void print(ostream& out) const override;
	optional<value_t> eval() const override;
	Expression *reduce() override;
	Quad::reg_t gen(QuadProgram& prog) override;
private:
	op_t _op;
	Expression *_arg1, *_arg2;
};


/****** Condition classes ******/

class Condition: public AST {
public:
	typedef enum {
		COMP,
		NOT,
		AND,
		OR
	} type_t;

	inline Condition(type_t type): _type(type) {}
	~Condition();
	inline type_t type() const { return _type; }
	virtual void reduce() = 0;
	virtual void gen(Quad::lab_t lab_true, Quad::lab_t lad_false, QuadProgram& prog) const = 0;
private:
	type_t _type;
};

class CompCond: public Condition {
public:
	typedef enum {
		EQ,
		NE,
		LT,
		LE,
		GT,
		GE
	} comp_t;

	inline CompCond(comp_t comp, Expression *arg1, Expression *arg2)
		: Condition(COMP), _comp(comp), _arg1(arg1), _arg2(arg2) { }
	void print(ostream& out) const override;
	void reduce() override;
	void gen(Quad::lab_t lab_true, Quad::lab_t lab_false, QuadProgram& prog) const override;

private:
	comp_t _comp;
	Expression *_arg1, *_arg2;
};


class NotCond: public Condition {
public:
	inline NotCond(Condition *cond): Condition(NOT), _cond(cond) {}
	~NotCond();
	void print(ostream& out) const override;
	void reduce() override;
	void gen(Quad::lab_t lab_true, Quad::lab_t lab_false, QuadProgram& prog) const override;
private:
	Condition *_cond;
};


class BinCond: public Condition {
public:
	inline BinCond(type_t type, Condition *cond1, Condition *cond2)
		: Condition(type), _cond1(cond1), _cond2(cond2) { }
	~BinCond();
	void reduce() override;
protected:
	Condition *_cond1, *_cond2;
};

class AndCond: public BinCond {
public:
	inline AndCond(Condition *cond1, Condition *cond2)
		: BinCond(AND, cond1, cond2) {}
	void print(ostream& out) const override;
	void gen(Quad::lab_t lab_true, Quad::lab_t lab_false, QuadProgram& prog) const override;
};

class OrCond: public BinCond {
public:
	inline OrCond(Condition *cond1, Condition *cond2)
		: BinCond(OR, cond1, cond2) {}
	void print(ostream& out) const override;
	void gen(Quad::lab_t lab_true, Quad::lab_t lab_false, QuadProgram& prog) const override;
};


/****** Declarations ******/
class Declaration: public AST {
public:
	typedef enum {
		NONE,
		CST,
		VAR,
		REG,
		SIG,
		AUTO
	} type_t;

	Declaration(type_t type, string name);
	~Declaration();
	inline type_t type() const { return _type; }
	inline string name() const { return _name; }
	virtual void reduce();

	static Declaration& none;
	static Declaration *getSymbol(string name);
	static const map<string, Declaration *>& symbols();
	static void clearSymTab();

private:
	type_t _type;
	string _name;
};

class ConstDecl: public Declaration {
public:
	inline ConstDecl(string name, int val)
		: Declaration(CST, name), _val(val) { }
	void print(ostream& out) const override;
	inline int value() const { return _val; }
private:
	int _val;
};

class VarDecl: public Declaration {
public:
	inline VarDecl(string name): Declaration(VAR, name) {}
	void print(ostream& out) const override;
};

class RegDecl: public Declaration {
public:
	inline RegDecl(string name, value_t addr)
		: Declaration(REG, name), _addr(addr) { }
	void print(ostream& out) const override;
	inline value_t address() const { return _addr; }
private:
	value_t _addr;
};


class SigDecl: public Declaration {
public:
	inline SigDecl(string name, RegDecl *reg, int bit)
		: Declaration(SIG, name), _reg(reg), _bit(bit) { }
	void print(ostream& out) const override;
	inline RegDecl *reg() const { return _reg; }
	inline int bit() const { return _bit; }
private:
	RegDecl	*_reg;
	int _bit;
};

class When: public AST {
public:
	inline When(bool neg, SigDecl *sig, Statement *action)
		: _neg(neg), _sig(sig), _action(action) { }
	inline bool neg() const { return _neg; }
	inline SigDecl *sig() const { return _sig; }
	inline Statement *action() const { return _action; }
	void print(ostream& out) const override;
	void fix(const vector<State *>& states);
	void reduce();
	void gen(AutoDecl& automaton, QuadProgram& prog);
private:
	bool _neg;
	SigDecl *_sig;
	Statement *_action;
};

class State: public AST {
public:
	inline State(string name, Statement *action, const vector<When *> whens)
		: _name(name), _action(action), _whens(whens) { }
	~State();
	inline string name() const { return _name; }
	inline Statement *action() const { return _action; }
	inline const vector<When *>& whens() const { return _whens; }
 	void print(ostream& out) const override;
	void fix(const vector<State *>& states);
	void reduce();
	void gen(AutoDecl& automaton, QuadProgram& prog);
	inline Quad::lab_t label() const { return _label; }
	void setLabel(Quad::lab_t label);
private:
	string _name;
	Statement *_action;
	vector<When *> _whens;
	Quad::lab_t _label;
};


class AutoDecl: public Declaration {
public:
	inline AutoDecl(string name, Statement *init, const vector<State *>& states)
		: Declaration(AUTO, name), _init(init), _states(states) { }
	~AutoDecl();
	void print(ostream& out) const override;
	void reduce() override;
	void gen(QuadProgram& prog);
	inline Quad::lab_t stopLabel() const { return _stop_label; }
private:
	Statement *_init;
	vector<State *> _states;
	Quad::lab_t _stop_label;
};

#endif // IOC_AST_CPP

