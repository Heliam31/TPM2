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
	
	case Declaration::REG:{
		auto r = prog.newReg();
		prog.emit(Quad::load(r,static_cast<RegDecl *>(_dec)->address()));
		return r;
	}
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
	case INV:
		prog.emit(Quad::inv(r,ro));
		break;
	}
	return r;
}


///
Quad::reg_t BinopExpr::gen(QuadProgram& prog) {
	auto rd = prog.newReg();
	auto r = _arg1->gen(prog);
	auto ro = _arg2->gen(prog);
	switch(_op) {
	case ADD:
		prog.emit(Quad::add(rd, r, ro));
		break;
	case SUB:
		prog.emit(Quad::sub(rd, r, ro));
		break;
	case MUL:
		prog.emit(Quad::mul(rd, r, ro));
		break;
	case DIV:
		prog.emit(Quad::div(rd, r, ro));
		break;
	case MOD:
		prog.emit(Quad::mod(rd, r, ro));
		break;
	case BIT_AND:
		prog.emit(Quad::and_(rd,r, ro));
		break;
	case BIT_OR:
		prog.emit(Quad::or_(rd,r,ro));
		break;
	case XOR:
		prog.emit(Quad::xor_(rd, r, ro));
		break;
	case SHL:
		prog.emit(Quad::shl(rd, r, ro));
		break;
	case SHR:
		prog.emit(Quad::shr(rd, r, ro));
		break;
	case ROL:
		prog.emit(Quad::rol(rd, r, ro));
		break;
	case ROR:
		prog.emit(Quad::ror(rd, r, ro));
		break;
	}
	
	return rd;
}


///
Quad::reg_t BitFieldExpr::gen(QuadProgram& prog) {
	Quad::reg_t rExp = 0;
	Quad::reg_t rHi = 1;
	Quad::reg_t rLo = 2;

	auto e = _expr->gen(prog);
	auto hi = _hi->gen(prog);
	auto lo = _lo->gen(prog);

	if(hi == lo){ //bit field de taille 1
		Quad::reg_t un = prog.newReg();
		prog.emit(Quad::seti(un,1));
        prog.emit(Quad::shr(rExp,e,lo));
        prog.emit(Quad::and_(rExp,rExp,un));
	}
	else{ //cas par default
		//on met les e,hi,lo dans les registres 0,1,2
		prog.emit(Quad::set(rExp,e));
		prog.emit(Quad::set(rHi, hi));
		prog.emit(Quad::set(rLo,lo));
		//on appelle le sous-programme
		prog.emit(Quad::call(field_set_call));
	}
	return rExp;
}


///
void CompCond::gen(Quad::lab_t lab_true, Quad::lab_t lab_false, QuadProgram& prog) const {
	auto a1 = _arg1->gen(prog);
	auto a2 = _arg2->gen(prog);
	switch(_comp) {
	case EQ: prog.emit(Quad::goto_eq(lab_true, a1, a2)); break;
	case NE: prog.emit(Quad::goto_ne(lab_true, a1, a2)); break;
	case LT: prog.emit(Quad::goto_lt(lab_true, a1, a2)); break;
	case LE: prog.emit(Quad::goto_le(lab_true, a1, a2)); break;
	case GT: prog.emit(Quad::goto_gt(lab_true, a1, a2)); break;
	case GE: prog.emit(Quad::goto_ge(lab_true, a1, a2)); break;
	}
}

///
void NotCond::gen(Quad::lab_t lab_true, Quad::lab_t lab_false, QuadProgram& prog) const {
	_cond->gen(lab_false, lab_true, prog);
}

///
void AndCond::gen(Quad::lab_t lab_true, Quad::lab_t lab_false, QuadProgram& prog) const {
	Quad::lab_t lab_2 = prog.newLab();
	_cond1->gen(lab_2,lab_false,prog);
	prog.emit(Quad::lab(lab_2));
	_cond2->gen(lab_true,lab_false,prog);
}

///
void OrCond::gen(Quad::lab_t lab_true, Quad::lab_t lab_false, QuadProgram& prog) const {
	Quad::lab_t lab_2 = prog.newLab();
	_cond1->gen(lab_true,lab_2,prog);
	prog.emit(Quad::lab(lab_2));
	_cond2->gen(lab_true,lab_false,prog);
}


///
void NOPStatement::gen(AutoDecl& automaton, QuadProgram& prog) const {
}

///
void SeqStatement::gen(AutoDecl& automaton, QuadProgram& prog) const {
	prog.comment(pos);
	_stmt1->gen(automaton, prog);
	_stmt2->gen(automaton,prog);
}

///
void IfStatement::gen(AutoDecl& automaton, QuadProgram& prog) const {
	Quad::lab_t lab_true = prog.newLab();
	Quad::lab_t lab_false = prog.newLab();
	Quad::lab_t lab_endif = prog.newLab();
	_cond->gen(lab_true,lab_false,prog);
	prog.emit(Quad::goto_(lab_false));
	prog.emit(Quad::lab(lab_true));
	_stmt1->gen(automaton, prog);
	prog.emit(Quad::goto_(lab_endif));
	prog.emit(Quad::lab(lab_false));
	_stmt2->gen(automaton,prog);
	prog.emit(Quad::lab(lab_endif));
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

