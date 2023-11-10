#include "Quad.hpp"
#include <string>
using namespace std;


/**
 * @class Quad
 * Represents a quadruplet made of an operation, a destination operand,
 * an operand A and an operand B.
 *
 * Quadruplet can be built with static construction Quad::add(), Quad::goto_, ...
 *
 * The quadruplet values can be directrly accessed Quad::type, Quad::d, Quad::a
 * and Quad::b or dedicated accessors for special types: Quad::label() for
 * branch related instructions, Quad::addr() for memory-related instructions and
 * Quad::cst() for constant-related instructions.
 */

/**
 * Generate string for the givenr register number.
 */
string Quad::reg(int i) {
	if(i >= 16)
		return "v" + to_string(i);
	else
		switch(i) {
		case 15: return "PC";
		case 14: return "LR";
		case 13: return "SP";
		case 12: return "FP";
		default: return "R" + to_string(i);
		}
}


/**
 * Print the given quad.
 */
void Quad::print(ostream& out) const {
	switch(type) {
	case NOP: out << "nop"; break;
	case SETI: out << reg(d) << " <- " << a; break;
	case SETL: out << reg(d) << " <- L" << a; break;
	case SET: out << reg(d) << " <- " << reg(a); break;
	case NEG: out << reg(d) << " <- -" << reg(a); break;
	case INV: out << reg(d) << " <- ~" << reg(a); break;
	case ADD: out << reg(d) << " <- " << reg(a) << " + " << reg(b); break;
	case SUB: out << reg(d) << " <- " << reg(a) << " - " << reg(b); break;
	case MUL: out << reg(d) << " <- " << reg(a) << " * " << reg(b); break;
	case DIV: out << reg(d) << " <- " << reg(a) << " / " << reg(b); break;
	case MOD: out << reg(d) << " <- " << reg(a) << " % " << reg(b); break;
	case AND: out << reg(d) << " <- " << reg(a) << " & " << reg(b); break;
	case OR: out << reg(d) << " <- " << reg(a) << " | " << reg(b); break;
	case XOR: out << reg(d) << " <- " << reg(a) << " ^ " << reg(b); break;
	case SHL: out << reg(d) << " <- " << reg(a) << " << " << reg(b); break;
	case SHR: out << reg(d) << " <- " << reg(a) << " >> " << reg(b); break;
	case ROL: out << reg(d) << " <- " << reg(a) << " <<< " << reg(b); break;
	case ROR: out << reg(d) << " <- " << reg(a) << " >>> " << reg(b); break;
	case LAB: out << "label L" << d; break;
	case GOTO: out << "goto L" << d; break;
	case GOTO_EQ: out << "if " << reg(a) << " == " << reg(b) << " goto L" << label(); break;
	case GOTO_NE: out << "if " << reg(a) << " != " << reg(b) << " goto L" << label(); break;
	case GOTO_LT: out << "if " << reg(a) << " < " << reg(b) << " goto L" << label(); break;
	case GOTO_LE: out << "if " << reg(a) << " <= " << reg(b) << " goto L" << label(); break;
	case GOTO_GT: out << "if " << reg(a) << " > " << reg(b) << " goto L" << label(); break;
	case GOTO_GE: out << "if " << reg(a) << " >= " << reg(b) << " goto L" << label(); break;
	case CALL: out << "call L" << label(); break;
	case RETURN: out << "return"; break;
	case LOAD: out << reg(d) << " <- M[" << reg(addr()) << "]"; break;
	case STORE: out << "M[" << reg(addr()) << "] <- " << reg(b); break;
	case PUSH: out << "push " << reg(a); break;
	case POP: out << "pop " << reg(d); break;
	}
}


/**
 * @class QuadProgram
 * Class representing a program made of quadruplets.
 */

///
QuadProgram::QuadProgram(): _vreg(Quad::HARD_COUNT), _lab(1) {}

/**
 * Add a quadruplet to the program.
 * @param q	Added quadruplet.
 */
void QuadProgram::emit(const Quad& q) {
	_quads.push_back(q);
}

/**
 * Obtain a new non-used virtual register number.
 * @return		New register number.
 */
Quad::reg_t QuadProgram::newReg() {
	return _vreg++;
}

/**
 * Obtain a new non-used lable number.
 * @return	New label number.
 */
Quad::lab_t QuadProgram::newLab() {
	return _lab++;
}

/**
 * Assocate a name with a new virtual register.
 * @param name		Name to associate with.
 * @return			Corresponding virtual register.
 */
Quad::reg_t QuadProgram::declare(string name) {
	auto r = newReg();
	_map[name] = r;
	return r;
}

/**
 * Get the virtual register number for a name.
 * @param name		Looked name.
 * @return			Corresponding register number.
 */
Quad::reg_t QuadProgram::regFor(string name) {
	return _map[name];
}

/**
 * Print the program.
 * @param out	Stream to output to.
 */
void QuadProgram::print(ostream& out) {

	// print variable assignement
	for(const auto& p: _map)
		out << "@ " << p.first << " -> " << Quad::reg(p.second) << endl;

	// print quadruplets
	auto c = _coms.begin();
	int i = 0;
	for(const auto& q: _quads) {
		if(c != _coms.end() && c->first == i) {
			out << "\t@ " << c->second << endl;
			++c;
		}
		if(q.type == Quad::LAB)
			out << 'L' << q.label() << endl;
		else
			out << '\t' << q << endl;
		i++;
	}

}

/**
 * Add a comment to the next generated quadruplet.
 * @param com	Added comment.
 */
void QuadProgram::comment(std::string com) {
	_coms.push_back(pair(_quads.size(), com));
}

/**
 * Build a CFG from the quad list.
 * @return	Built CFG (ownership transferred to the caller).
 */
CFG<Quad> *QuadProgram::makeCFG() {
	auto g = new CFG<Quad>();

	// fix branches
	map<Quad::lab_t, BB<Quad> *> map;

	// build BB
	bool labels = true;
	BB<Quad> *bb = nullptr;
	list<Quad> qs;
	BB<Quad> *prev = g->entry();

	for(auto q: _quads) {

		// create BB
		if(bb == nullptr) {
			bb = new BB<Quad>();
			g->add(bb);
			if(prev != nullptr)
				prev->setNext(bb);
		}

		switch(q.type) {

		case Quad::LAB:
			if(!labels) {
				bb->setInstructions(qs);
				prev = bb;
				qs.clear();
				labels = true;
				bb = new BB<Quad>();
				g->add(bb);
				prev->setNext(bb);
			}
			map[q.label()] = bb;
			qs.push_back(q);
			break;

		case Quad::RETURN:
			qs.push_back(q);
			bb->setInstructions(qs);
			bb->setNext(g->exit());
			qs.clear();
			prev = nullptr;
			bb = nullptr;
			labels = true;
			break;

		case Quad::GOTO:
			qs.push_back(q);
			bb->setInstructions(qs);
			qs.clear();
			prev = nullptr;
			bb = nullptr;
			labels = true;
			break;

		case Quad::GOTO_EQ:
		case Quad::GOTO_NE:
		case Quad::GOTO_LT:
		case Quad::GOTO_LE:
		case Quad::GOTO_GT:
		case Quad::GOTO_GE:
			qs.push_back(q);
			bb->setInstructions(qs);
			qs.clear();
			prev = bb;
			bb = nullptr;
			labels = true;
			break;

		default:
			qs.push_back(q);
			labels = false;
			break;
		}

	}

	// finish current (shouldn't arise)
	if(bb != nullptr)
		bb->setInstructions(qs);

	// fix the branches
	for(auto bb: g->basicBlocks())
		if(!bb->instructions().empty()) {
			auto q = bb->instructions().back();
			switch(q.type) {
			case Quad::GOTO:
			case Quad::GOTO_EQ:
			case Quad::GOTO_NE:
			case Quad::GOTO_LT:
			case Quad::GOTO_LE:
			case Quad::GOTO_GT:
			case Quad::GOTO_GE:
				bb->setTarget(map[q.label()]);
				break;
			default:
				break;
			}
		}

	return g;
}
