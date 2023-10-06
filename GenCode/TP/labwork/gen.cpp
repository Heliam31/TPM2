#include "AST.hpp"
#include "Quad.hpp"

#include <assert.h>

const Quad::lab_t
	base_call = 10000,
	field_get_call = 10000,	// R0=expr, R1=high bit, R2=low bit
	field_set_call = 100001;	// R0=expr, R1=high bit, R2=low bit, R2=assigned value


///
Quad::reg_t ConstExpr::gen(QuadProgram& prog) {
	auto r = prog.newReg();
	prog.emit(Quad::seti(r, _val));
	return r;
}

///
Quad::reg_t MemExpr::gen(QuadProgram& prog) {
	switch(_dec->type()) {

	case Declaration::CST: {
			auto r = prog.newReg();
			prog.emit(Quad::seti(r, static_cast<ConstDecl *>(_dec)->value()));
			return r;
		}

	case Declaration::VAR:
		return prog.regFor(static_cast<VarDecl *>(_dec)->name());


	default:
		assert(false);
		return 0;
	}
}


///
Quad::reg_t UnopExpr::gen(QuadProgram& prog) {
	auto ro = _arg->gen(prog);
	auto r = prog.newReg();
	switch(_op) {
	case NEG:
		prog.emit(Quad::neg(r, ro));
		break;
	}
	return r;
}


///
Quad::reg_t BinopExpr::gen(QuadProgram& prog) {
	auto rd = prog.newReg();
	return rd;
}


///
Quad::reg_t BitFieldExpr::gen(QuadProgram& prog) {
}


///
void CompCond::gen(Quad::lab_t lab_true, Quad::lab_t lab_false, QuadProgram& prog) const {
	auto a1 = _arg1->gen(prog);
	auto a2 = _arg2->gen(prog);
	switch(_comp) {
	case EQ: prog.emit(Quad::goto_eq(lab_true, a1, a2)); break;
	}
}

///
void NotCond::gen(Quad::lab_t lab_true, Quad::lab_t lab_false, QuadProgram& prog) const {
	_cond->gen(lab_false, lab_true, prog);
}

///
void AndCond::gen(Quad::lab_t lab_true, Quad::lab_t lab_false, QuadProgram& prog) const {
}

///
void OrCond::gen(Quad::lab_t lab_true, Quad::lab_t lab_false, QuadProgram& prog) const {
}


///
void NOPStatement::gen(AutoDecl& automaton, QuadProgram& prog) const {
}

///
void SeqStatement::gen(AutoDecl& automaton, QuadProgram& prog) const {
}

///
void IfStatement::gen(AutoDecl& automaton, QuadProgram& prog) const {
	prog.comment(pos);
}

///
void SetStatement::gen(AutoDecl& automaton, QuadProgram& prog) const {
	prog.comment(pos);
	auto r = _expr->gen(prog);
	switch(_dec->type()) {
	case Declaration::VAR:
		prog.emit(Quad::set(prog.regFor(static_cast<VarDecl *>(_dec)->name()), r));
		break;
	case Declaration::REG: {
			auto ra = prog.newReg();
			prog.emit(Quad::seti(ra, static_cast<RegDecl *>(_dec)->address()));
			prog.emit(Quad::store(ra, r));
		}
		break;
	default:
		assert(false);
		break;
	}
}


///
void SetFieldStatement::gen(AutoDecl& automaton, QuadProgram& prog) const {
	prog.comment(pos);
}

///
void GotoStatement::gen(AutoDecl& automaton, QuadProgram& prog) const {
	prog.comment(pos);
}

///
void StopStatement::gen(AutoDecl& automaton, QuadProgram& prog) const {
	prog.comment(pos);
	prog.emit(Quad::goto_(automaton.stopLabel()));
}


/**
 * Generate the code to implement a "when" directive.
 * @param automaton		Current automaton.
 * @param prog			Program to generate quadruplets in.
 */
void When::gen(AutoDecl& automaton, QuadProgram& prog) {
	prog.comment(pos);
}


/**
 * Generate the code for a state.
 * @param automaton		Current automaton.
 * @param prog			Program to generate quadruplets in.
 */
void State::gen(AutoDecl& automaton, QuadProgram& prog) {
	prog.emit(Quad::lab(_label));
	_action->gen(automaton, prog);;
	auto loop = prog.newLab();
	prog.emit(Quad::lab(loop));
	for(auto when: _whens)
		when->gen(automaton, prog);
	prog.emit(Quad::goto_(loop));
}


/**
 * Generate the code for the automatin.
 * @param prog	Program to generate in.
 */
void AutoDecl::gen(QuadProgram& prog) {
	_stop_label = prog.newLab();
	for(auto state: _states)
		state->setLabel(prog.newLab());
	_init->gen(*this, prog);
	for(auto state: _states)
		state->gen(*this, prog);
	prog.emit(Quad::lab(_stop_label));
	prog.emit(Quad::return_());
}

