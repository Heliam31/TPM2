
#include <cstdint>
#include <assert.h>
#include "Inst.hpp"

typedef enum {
	IGNORE = 0x00000,
	RECORD = 0x10000,
	EQUAL  = 0x20000,
	POW2   = 0x30000,
	ISIMM  = 0x40000,
	NOVAR  = 0x50000
} check_t;

typedef enum {
	COPY = 0x10000,
	LOG2 = 0x20000
} action_t;

typedef struct select_t {
	Quad quads[5];
	Inst insts[];
} select_t;

inline Param pread(uint32_t x) { return Param::read(x); }
inline Param pwrite(uint32_t x) { return Param::read(x); }
inline Param pcst(uint32_t x) { return Param::cst(x); }


/**
 * @class Param
 * Parameter of a machine instruction.
 */

/**
 * Print the parameter.
 */
void Param::print(ostream& out) const {
	switch(_type) {
	case NONE:
		break;
	case CST:
		out << '#' << _val;
		break;
	case READ:
		out << "read " << Quad::reg(_val);
		break;
	case WRITE:
		out << "write " << Quad::reg(_val);
		break;
	default:
		assert(false);
		break;
	}
}


/**
 * @class Inst
 * Represents a machine instruction.
 */

/**
 * Print a machine instruction.
 */
void Inst::print(ostream& out) const {
	for(auto p = _fmt; *p != '\0'; p++) {
		if(*p != '%')
			out << *p;
		else {
			int n = (*++p) - '0';
			out << _params[n].value();
		}
	}
}


/**
 * Instruction end marker.
 */
Inst Inst::end;


// instruction selectors
select_t
	select_add = {
		{ Quad::add(RECORD|0, RECORD|1, RECORD|2) },
		{ Inst("\tadd R%0, R%1, R%2", pwrite(COPY|0), pread(COPY|1), pread(COPY|2)), Inst::end }
	},
	select_addi = {
		{ Quad::seti(RECORD|2, ISIMM|3), Quad::add(RECORD|0, RECORD|1, EQUAL|2) },
		{ Inst("\tadd R%0, R%1, #%2", pwrite(COPY|0), pread(COPY|1), pcst(COPY|3)) }
	},
	select_addi2 = {
		{ Quad::seti(RECORD|2, ISIMM|3), Quad::add(RECORD|0, EQUAL|2, RECORD|1) },
		{ Inst("\tadd R%0,R%1, #%2", pwrite(COPY|0), pread(COPY|1), pcst(COPY|3)) }
	},
	select_sub = {
		{ Quad::sub(RECORD|0, RECORD|1, RECORD|2) },
		{ Inst("\tsub R%0, R%1, R%2", pwrite(COPY|0), pread(COPY|1), pread(COPY|2)), Inst::end }
	},
	select_subi = {
		{ Quad::seti(RECORD|2, ISIMM|3), Quad::sub(RECORD|0, RECORD|1, EQUAL|2) },
		{ Inst("\tsub R%0, R%1, #%2", pwrite(COPY|0), pread(COPY|1), pcst(COPY|3)) }
	},
	select_subi2 = {
		{ Quad::seti(RECORD|2, ISIMM|3), Quad::sub(RECORD|0, EQUAL|2, RECORD|1) },
		{ Inst("\tsub R%0,R%1,#%2", pwrite(COPY|0), pread(COPY|1), pcst(COPY|3)) }
	},
	select_mul = {
		{ Quad::mul(RECORD|0, RECORD|1, RECORD|2) },
		{ Inst("\tmul R%0, R%1, R%2", pwrite(COPY|0), pread(COPY|1), pread(COPY|2)), Inst::end }
	},
	select_div = {
		{ Quad::div(RECORD|0, RECORD|1, RECORD|2) },
		{ Inst("\tdiv R%0, R%1, R%2", pwrite(COPY|0), pread(COPY|1), pread(COPY|2)), Inst::end }
	},
	select_eor = {
		{ Quad::xor_(RECORD|0, RECORD|1, RECORD|2) },
		{ Inst("\teor R%0, R%1, R%2", pwrite(COPY|0), pread(COPY|1), pread(COPY|2)), Inst::end }
	},
	select_eori = {
		{ Quad::seti(RECORD|2, ISIMM|3), Quad::xor_(RECORD|0, RECORD|1, EQUAL|2) },
		{ Inst("\teor R%0, R%1, #%2", pwrite(COPY|0), pread(COPY|1), pcst(COPY|3)) }
	},
	select_eori2 = {
		{ Quad::seti(RECORD|2, ISIMM|3), Quad::xor_(RECORD|0, EQUAL|2, RECORD|1) },
		{ Inst("\teor R%0,R%1,#%2", pwrite(COPY|0), pread(COPY|1), pcst(COPY|3)) }
	},
	select_and = {
		{Quad::and_(RECORD|0,RECORD|1,RECORD|2)},
		{Inst("\tand R%0, R%1, R%2", pwrite(COPY|0), pread(COPY|1), pread(COPY|2)), Inst::end}
	},
	select_andi = {
		{Quad::seti(RECORD|2,ISIMM|3),Quad::and_(RECORD|0,RECORD|1,EQUAL|2)},
		{Inst("\tand R%0, R%1, #%2", pwrite(COPY|0), pread(COPY|1), pcst(COPY|3)), Inst::end}
	},
	select_andi2 = {
		{Quad::seti(RECORD|2,ISIMM|3),Quad::and_(RECORD|0,EQUAL|2,RECORD|1)},
		{Inst("\tand R%0,R%1,#%2", pwrite(COPY|0), pread(COPY|1), pcst(COPY|3)), Inst::end}
	},	
	select_label = {
		{ Quad::lab(RECORD|0) },
		{ Inst("L%0:", pcst(COPY|0)), Inst::end }
	},
	select_ldreq = {
		{ Quad::seti(RECORD|0, RECORD|1) },
		{ Inst("\tldr R%0, =%1", pwrite(COPY|0), pcst(COPY|1)), Inst::end }
	},
	select_mov = {
		{ Quad::set(RECORD|0, RECORD|1) },
		{ Inst("\tmov R%0, R%1", pwrite(COPY|0), pread(COPY|1)), Inst::end }
	},
	select_movi = {
		{ Quad::seti(RECORD|0, ISIMM|1) },
		{ Inst("\tmov R%0, #%1", pwrite(COPY|0), pcst(COPY|1)), Inst::end }
	},
	select_shl= {
		{ Quad::shl(RECORD|0, RECORD|1, RECORD|2) },
		{ Inst("\tmov R%0, R%1, lsl R%2", pwrite(COPY|0), pread(COPY|1), pread(COPY|2)), Inst::end }
	},
	select_shli = {
		{ Quad::seti(RECORD|2, ISIMM|3), Quad::shl(RECORD|0, RECORD|1, EQUAL|2) },
		{ Inst("\tmov R%0, R%1, lsl #%2", pwrite(COPY|0), pread(COPY|1), pcst(COPY|3)) }
	},
	select_shli2 = {
		{ Quad::seti(RECORD|2, ISIMM|3), Quad::shl(RECORD|0, EQUAL|2, RECORD|1) },
		{ Inst("\tmov R%0, R%1, lsl #%2", pwrite(COPY|0), pread(COPY|1), pcst(COPY|3)) }
	},
	select_shr= {
		{ Quad::shr(RECORD|0, RECORD|1, RECORD|2) },
		{ Inst("\tmov R%0, R%1, lsr R%2", pwrite(COPY|0), pread(COPY|1), pread(COPY|2)), Inst::end }
	},
	select_shri = {
		{ Quad::seti(RECORD|2, ISIMM|3), Quad::shr(RECORD|0, RECORD|1, EQUAL|2) },
		{ Inst("\tmov R%0, R%1, lsr #%2", pwrite(COPY|0), pread(COPY|1), pcst(COPY|3)) }
	},
	select_shri2 = {
		{ Quad::seti(RECORD|2, ISIMM|3), Quad::shr(RECORD|0, EQUAL|2, RECORD|1) },
		{ Inst("\tmov R%0, R%1, lsr #%2", pwrite(COPY|0), pread(COPY|1), pcst(COPY|3)) }
	},
	select_goto = {
		{Quad::goto_(RECORD|0)},
		{Inst("\tgoto %0", pread(COPY|0))}
	},
	select_call = {
		{Quad::call(RECORD|0)},
		{Inst("\tb %0", pread(COPY|0))}
	},
	select_return = {
		{ Quad::return_() },
		{ Inst("\tbx LR"), Inst::end }
	}

;


select_t *selectors[] = {
	&select_add,
	&select_addi,
	&select_addi2,
	&select_sub,
	&select_subi,
	&select_subi2,
	&select_mul,
	&select_div,
	&select_eor,
	&select_eori,
	&select_eori2,
	&select_shl,
	&select_shli,
	&select_shli2,
	&select_shr,
	&select_shri,
	&select_goto,
	&select_shri2,
	&select_and,
	&select_andi,
	&select_andi2,
	&select_label,
	&select_call,
	&select_mov,
	&select_mov,
	&select_movi,
	&select_ldreq,
	&select_return,
	nullptr
};


/* useful functions */
inline check_t check(uint32_t x)
	{ return static_cast<check_t>(x & 0xffff0000); }
inline action_t action(uint32_t x)
	{ return static_cast<action_t>(x & 0xffff0000); }
inline uint32_t value(uint32_t x)
	{ return static_cast<uint32_t>(x & 0x0000ffff); }

int bitcount(int32_t v) {
	int cnt = 0;
	for(int i = 0; v && i < 32; i++, v >>= 1)
		if((i & 1) != 0)
			cnt++;
	return cnt;
}

uint32_t rightmostbit(uint32_t x) {
	for(int i = 0; i < 32; i++)
		if((i & 1) != 0)
			return i;
	return -1;
}

bool isImmediate(uint32_t x) {
	if(x == 0)
		return true;
	for(int i = 0; i < 16 && ((x & 0b11) == 0); i++, x >>= 2);
	return (x & 0xffffff00) == 0;
}


/**
 * Test of a match of an argument with a given template.
 * @param tmp	Template argument.
 * @param arg	Actual argument.
 * @param vars	Template variables.
 * @return		True if this match, false else.
 */
bool matchParam(uint32_t tmp, uint32_t arg, uint32_t vars[]) {
	switch(check(tmp)) {
	case IGNORE:
		return true;
	case POW2:
		if(bitcount(arg) != 1)
			return false;
	case RECORD:
		vars[value(tmp)] = arg;
		return true;
	case EQUAL:
		return vars[value(tmp)] == arg;
	case ISIMM:
		if(!isImmediate(arg))
			return false;
		else {
			vars[value(tmp)] = arg;
			return true;
		}
	default:
		assert(false);
		break;
	}
	return true;
}


/**
 * Test of a match of a quadruplet with a given template.
 * @param temp	Template quadruplet.
 * @param quad	Quadruplet to match with.
 * @param arg	Argument to compare with and to update.
 * @return		True if this match, false else.
 */
bool matchQuad(const Quad& temp, const Quad& quad, uint32_t vars[]) {
	return temp.type == quad.type
		&& matchParam(temp.d, quad.d, vars)
		&& matchParam(temp.a, quad.a, vars)
		&& matchParam(temp.b, quad.b, vars);
}


/**
 * Make an instruction from instruction template and variables.
 * @param temp	Instruction template.
 * @param vars	Template variable.
 * @return		Made instruction.
 */
Inst makeInst(const Inst& temp, uint32_t vars[]) {
	Inst inst = Inst(temp.format());
	for(int i = 0; i < 4 && temp[i].type() != Param::NONE; i++)
		switch(action(temp[i].value())) {
		case COPY:
			inst[i] = Param(temp[i].type(), vars[value(temp[i].value())]);
			break;
		case LOG2:
			inst[i] = Param(temp[i].type(), rightmostbit(vars[value(temp[i].value())]));
			break;
		default:
			assert(false);
			break;
		}
	return inst;
}


/**
 * Select instruction in the given sequence.
 * @param quads	List of quadruplets to select in.
 * @return		List of corresponding instructions.
 */
list<Inst> select(const list<Quad>& quads) {
	list<Inst> insts;
	uint32_t vars[16];

	// traverse all instructions
	for(auto i = quads.begin(); i != quads.end();) {
		//cerr << "DEBUG: " << *i << endl;

		// traverse all selectors
		select_t *selector = nullptr;
		auto j = i;
		for(auto s = selectors; selector == nullptr && *s != nullptr; s++) {
			j = i;
			selector = *s;
			//cerr << "DEBUG:\t\tcheck " << (*s)->insts[0].format() << endl;
			for(int x = 0; j != quads.end() && (*s)->quads[x].type != Quad::NOP; ++x, ++j)
				if(!matchQuad((*s)->quads[x], *j, vars)) {
					selector = nullptr;
					break;
				}
		}

		// apply the selector
		if(selector == nullptr) {
			cerr << "WARNING: cannot translate " << *i << endl;
			++i;
		}
		else {
			for(int x = 0; selector->insts[x].format() != nullptr; x++)
				insts.push_back(makeInst(selector->insts[x], vars));
			i = j;
		}
	}
	return insts;
}
