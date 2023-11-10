#include <map>
#include "AST.hpp"

extern "C" const char *lexer_file;
extern "C" int lexer_line;

using namespace std;

class Declaration;

/**
 * Symbol table.
 */
map<string, Declaration *> symtab;


/**
 * @class Position
 * Record a position in the source file.
 */

/**
 * Build a position from the current lexer configuration.
 */
Position::Position() {
	file = lexer_file;
	line = lexer_line;
}

///
Position::Position(int line_) {
	file = lexer_file;
	line = line_;
}

/**
 * Print the position, convenient to display error.
 */
ostream& operator<<(ostream& out, const Position& pos) {
	out << pos.file << ":" << pos.line;
	return out;
}


/**
 * @class AST
 * Base class of all AST. Group several facilities like
 * printing, source position, etc.
 */

/**
 * @fn void AST::print(ostream& out) const;
 * Print the AST.
 */


///
ostream& operator<<(ostream& out, const AST *ast) {
	ast->print(out);
	return out;
}


/**
 * @class Statement
 * Base class of statements.
 */

/**
 * @fn Statement::Statement(type_t type);
 * Build a statement for the given type.
 */

/**
 * @fn type_t Statement::type() const;
 * Get the type of a statement.
 * @return	Statement type.
 */

/**
 * Called to fix "goto" calles. Default implementaton does nothing.
 */
void Statement::fix(const vector<State *>& states) {

}

/**
 * @fn void Statement::gen(AutoDecl& automaton, QuadProgram& prog) const;
 * Called to generate the current statement.
 * @param automaton		Generated automaton.
 * @param prog			Program to emit quadruplets in.
 */


/**
 * @fn void Statement::reduce();
 * Reduce all constant expressions in the statement.
 */


/**
 * @class NOPStatement
 * Statement representing no operation.
 */

///
void NOPStatement::print(ostream& out) const {
	out << "NOP";
}


/**
 * @class SeqStatement
 * AST representing a sequence between statements.
 */

///
SeqStatement::~SeqStatement() {
	delete _stmt1;
	delete _stmt2;
}

///
void SeqStatement::print(ostream& out) const {
	out << "SEQ(";
	_stmt1->print(out);
	out << ", ";
	_stmt2->print(out);
	out << ")";
}

///
void SeqStatement::fix(const vector<State *>& states) {
	_stmt1->fix(states);
	_stmt2->fix(states);
}


/**
 * @class SetStatement
 * Statement representing an assignment.
 */


SetStatement::~SetStatement() {
	delete _expr;
}

void SetStatement::print(ostream& out) const {
	out << "SET(" << _dec->name() << ", ";
	_expr->print(out);
	out << ")";
}


/**
 * @fn class SetFieldStatement
 * Statement representing the assignment of a field.
 */

///
SetFieldStatement::~SetFieldStatement() {
	delete _hi;
	if(_hi != _lo)
		delete _lo;
	delete _expr;
}

void SetFieldStatement::print(ostream& out) const {
	out << "SET_FIELD(" << _dec->name() << ", ";
	_hi->print(out);
	out << ", ";
	_lo->print(out);
	out << ", ";
	_expr->print(out);
	out << ")";
}

/**
 * @class IfStatement
 * Represents an "if" statement.
 */

///
IfStatement::~IfStatement() {
	delete _cond;
	delete _stmt1;
	delete _stmt2;
}

///
void IfStatement::print(ostream& out) const {
	out << "IF(";
	_cond->print(out);
	out << ", ";
	_stmt1->print(out);
	out << ", ";
	_stmt2->print(out);
	out << ")";
}

///
void IfStatement::fix(const vector<State *>& states) {
	_stmt1->fix(states);
	_stmt2->fix(states);
}


/**
 * @class GotoStatement
 * Statement representing a goto.
 */

///
void GotoStatement::print(ostream& out) const {
	out << "GOTO(" << _state->name() << ")";
}

///
void GotoStatement::fix(const vector<State *>& states) {
	for(auto s: states)
		if(s->name() == _id) {
			_state = s;
			return;
		}
	throw ParseException(pos, "unknown state " + _id + "!");
}


/**
 * @class StopStatement
 * Statement representing a stop.
 */

///
void StopStatement::print(ostream& out) const {
	out << "STOP";
}


/****** Expressions ******/

static const class NoneExpr: public Expression {
public:
	NoneExpr(): Expression(NONE) {}
	void print(ostream& out) const override {
		out << "NONE";
	}
	optional<value_t> eval() const override {
		return {};
	}

	Expression *reduce() override { return this; }

	Quad::reg_t gen(QuadProgram& prog) override { return 0; }

} expr_none;

/**
 * @class Expression
 * AST representing an expression.
 */

/**
 * @fn type_t Expression::type() const;
 * Get the type of the expression.
 */

/**
 * @fn optional<long> Expression::eval() const;
 * Evaluates the expression as a constant.
 * @return 	Evaluated value or none.
 */

/**
 * @fn Expression *Expression::reduce();
 * Call to reduce an expession as a constant.
 * @retrun	Itself or its reduced form.
 */

/**
 * @fn Quad::reg_t Expression::gen(QuadProgram& prog);
 * Generate the quadruplets for the expression in the given program.
 * @param prog	Program to generate code in.
 * @return		Virtual register number containing the result of the expression.
 */


/**
 * Singleton for the none expression.
 */
const Expression *Expression::none = &expr_none;

/**
 * @class ConstExpr
 * AST for constant expressions.
 */

///
void ConstExpr::print(ostream& out) const {
	if(_val > 10000)
		out << "CST(0x" << hex << _val << ")";
	else
		out << "CST(" << _val << ")";
}

/**
 * @class MemExpr
 * AST for expression accessing a memory (variable, register or constant).
 */

///
void MemExpr::print(ostream& out) const {
	out << "MEM(" << _dec->name() << ")";
}

/**
 * @class BitFieldExpr
 * AST to represent bit field expression.
 */

///
BitFieldExpr::~BitFieldExpr() {
	delete _expr;
	delete _hi;
	if(_hi != _lo)
		delete _lo;
}

///
void BitFieldExpr::print(ostream& out) const {
	out << "BITFIELD(";
	_expr->print(out);
	out << ", ";
	_hi->print(out);
	out << ", ";
	_lo->print(out);
	out << ")";

}


/**
 * @class UnopExpr
 * AST for unary operation expression.
 */

///
UnopExpr::~UnopExpr() {
	delete _arg;
}

///
void UnopExpr::print(ostream& out) const {
	out << "UNOP(";
	switch(_op) {
	case NEG: out << "NEG"; break;
	case INV: out << "INV"; break;
	}
	out << ", ";
	_arg->print(out);
	out << ")";
}


/**
 * @class BinopExpr
 * AST for binary operation expression.
 */

///
BinopExpr::~BinopExpr() {
	delete _arg1;
	delete _arg2;
}

///
void BinopExpr::print(ostream& out) const {
	out << "UNOP(";
	switch(_op) {
	case ADD: out << "ADD"; break;
	case SUB: out << "SUB"; break;
	case MUL: out << "MUL"; break;
	case DIV: out << "DIV"; break;
	case MOD: out << "MOD"; break;
	case BIT_OR: out << "BIT_OR"; break;
	case BIT_AND: out << "BIT_AND"; break;
	case XOR: out << "XOR"; break;
	case SHL: out << "SHL"; break;
	case SHR: out << "SHR"; break;
	case ROL: out << "ROL"; break;
	case ROR: out << "ROR"; break;
	}
	out << ", ";
	_arg1->print(out);
	out << ", ";
	_arg2->print(out);
	out << ")";
}


/****** Declarations ******/

static class NoneDecl: public Declaration {
public:
	NoneDecl(): Declaration(NONE, "") {}
	void print(ostream& out) const override {
		out << "NONE" << endl;
	}
} _none;


/**
 * @class Declaration
 * Representation of declarations in IOML.
 */

///
Declaration::Declaration(type_t type, string name)
	: _type(type), _name(name)
	{
		if(getSymbol(name) != nullptr) {
			cerr << "ERROR:" << pos << ": symbol already exists!" << endl;
			exit(1);
		}
		symtab[name] = this;

	}


/**
 * @class Condition
 * Base class of condition ASTs.
 */

///
Condition::~Condition() {}

/**
 * @fn void Condition::gen(Quad::lab_t lab_true, Quad::lab_t lad_false, QuadProgram& prog) const;
 * Function generating the code for the condition.
 * @param lab_true	Label to branch to when the condition is true.
 * @param lab_false	Label to branch to when the condition is false.
 * @param prog		Program to emit quadruplets in.
 */

/**
 * @fn void Condition::reduce();
 * Called to reduce constant expressions in the conditions.
 */

/**
 * @class CompCond
 * AST for comparison condition.
 */

///
void CompCond::print(ostream& out) const {
	out << "COMP(";
	switch(_comp) {
	case EQ: out << "EQ"; break;
	case NE: out << "NE"; break;
	case LT: out << "LT"; break;
	case LE: out << "LE"; break;
	case GT: out << "GT"; break;
	case GE: out << "GE"; break;
	}
	out << ", ";
	_arg1->print(out);
	out << ", ";
	_arg2->print(out);
	out << ")";
}


/**
 * @class NotCond
 * AST for a not condition.
 */

///
NotCond::~NotCond() {
	delete _cond;
}


///
void NotCond::print(ostream& out) const {
	out << "NOT(";
	_cond->print(out);
	out << ")";
}


/**
 * @class BinCond: public Condition
 * Base class AST for binary operation condition (AND and OR);
 */

///
BinCond::~BinCond() {
	delete _cond1;
	delete _cond2;
}


/**
 * @class AndCond
 * AST for AND condition.
 */

void AndCond::print(ostream& out) const {
	out << "AND(";
	_cond1->print(out);
	out << ", ";
	_cond2->print(out);
	out << ")";
}


/**
 * @class OrCond
 * AST for OR condition.
 */

void OrCond::print(ostream& out) const {
	out << "OR(";
	_cond1->print(out);
	out << ", ";
	_cond2->print(out);
	out << ")";
}


///
Declaration::~Declaration() {
}

/**
 * Reduce constant expressions in the declaration.
 */
void Declaration::reduce() {
}

/**
 * @fn type_t Declaration::type() const;
 * Get the type of the declaration.
 */

/** Empty declaration. */
Declaration& Declaration::none = _none;

/**
 * Get a symbol from the symbol table.
 * @param name	Name of the looked symbol.
 * @return		Found symbol or a null pointer.
 */
Declaration *Declaration::getSymbol(string name) {
	auto r = symtab.find(name);
	if(r == symtab.end())
		return nullptr;
	else
		return (*r).second;
}

/**
 * Retrieve the symbol table.
 */
const map<string, Declaration *>& Declaration::symbols() {
	return symtab;
}

void Declaration::clearSymTab() {
	for(auto s: symtab) {
		if(s.second != &none)
			delete s.second;
	}
}


/**
 * @class ConstDecl
 * Declaration for constants.
 */

///
void ConstDecl::print(ostream& out) const {
	out << name() << ": CONST(" << _val << ")" << endl;
}


/**
 * @class VarDecl
 * Declaration for a variable.
 */

///
void VarDecl::print(ostream& out) const {
	out << name() << ": VAR" << endl;
}


/**
 * @class RegDecl
 * A declaration representing an I/O register.
 */

///
void RegDecl::print(ostream& out) const {
	out << name() << ": REG(0x" << hex << _addr << ")" << endl;
}


/**
 * @class SigDecl
 * Declaration for a signal.
 */

///
void SigDecl::print(ostream& out) const {
	out << name() << ": SIG(" << _reg->name() << ", " << _bit << ")" << endl;
}


/**
 * @class AutoDecl
 * Declaration for an automaton.
 */

///
AutoDecl::~AutoDecl() {
	delete _init;
	for(auto s: _states)
		delete s;
}

///
void AutoDecl::print(ostream& out) const {
	out << name() << ": AUTO" << endl;
	out << "\t"; _init->print(out); out << endl;
	for(auto s: _states)
		s->print(out);
}

///
void AutoDecl::reduce() {
	_init->reduce();
	for(auto s: _states)
		s->reduce();
}


/**
 * @class State
 * State in an automaton.
 */

///
State::~State() {
	delete _action;
	for(auto w: _whens)
		delete w;
}

/**
 * Set the label to branch to implement the state.
 * @param label	Label to set.
 */
void State::setLabel(Quad::lab_t label) {
	_label = label;
}


///
void State::print(ostream& out) const {
	out << "\tSTATE " << name() << ":" << endl;
	out << "\t\t"; _action->print(out); out << endl;
	for(auto w: _whens)
		w->print(out);
}

///
void State::reduce() {
	_action->reduce();
	for(auto w: _whens)
		w->reduce();
}

/**
 * Fix the when operations.
 */
void State::fix(const vector<State *>& states) {
	_action->fix(states);
	for(auto w: _whens)
		w->fix(states);
}


/**
 * @class When
 * Representation of when condition.
 */

///
void When::print(ostream& out) const {
	out << "\t\tWHEN ";
	if(_neg)
		out << "!";
	out << _sig->name() << ":" << endl;
	out << "\t\t\t";
	_action->print(out);
	out << endl;
}

/**
 * Fix the when operations.
 */
void When::fix(const vector<State *>& states) {
	_action->fix(states);
}

///
void When::reduce() {
	_action->reduce();
}

