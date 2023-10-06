#ifndef IOC_QUAD_HPP
#define IOC_QUAD_HPP

#include <iostream>
#include <list>
#include <map>
using namespace std;

#include "CFG.hpp"

class Quad {
public:
	typedef unsigned long arg_t;
	typedef arg_t reg_t;
	typedef arg_t lab_t;
	typedef arg_t val_t;

	static const reg_t
		HARD_COUNT = 17,
		SP = 13,
		LR = 14,
		PC = 15,
		ALLOC_COUNT = SP;

	static string reg(int i);

	typedef enum {
		NOP,
		SETI,
		SETL,
		SET,
		NEG,
		INV,
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,
		AND,
		OR,
		XOR,
		SHL,
		SHR,
		ROL,
		ROR,
		LAB,
		GOTO,
		GOTO_EQ,
		GOTO_NE,
		GOTO_LT,
		GOTO_LE,
		GOTO_GT,
		GOTO_GE,
		CALL,
		RETURN,
		LOAD,
		STORE,
		PUSH,
		POP
	} type_t;

	inline Quad(): type(NOP), d(0), a(0), b(0) {}
	inline Quad(type_t type_, arg_t d_ = 0, arg_t a_ = 0, arg_t b_ = 0)
		: type(type_), d(d_), a(a_), b(b_) {}
	inline Quad(const Quad& q): type(q.type), d(q.d), a(q.a), b(q.b) {}

	lab_t label() const { return d; }
	val_t cst() const { return a; }
	reg_t addr() const { return a; }

	type_t type;
	arg_t d, a, b;

	inline static Quad nop() { return Quad(NOP); }
	inline static Quad seti(arg_t d, arg_t c) { return Quad(SETI, d, c); }
	inline static Quad setl(arg_t d, arg_t l) { return Quad(SETL, d, l); }
	inline static Quad set(reg_t d, reg_t a) { return Quad(SET, d, a); }
	inline static Quad neg(reg_t d, reg_t a) { return Quad(NEG, d, a); }
	inline static Quad inv(reg_t d, reg_t a) { return Quad(INV, d, a); }
	inline static Quad add(reg_t d, reg_t a, reg_t b) { return Quad(ADD, d, a, b); }
	inline static Quad sub(reg_t d, reg_t a, reg_t b) { return Quad(SUB, d, a, b); }
	inline static Quad mul(reg_t d, reg_t a, reg_t b) { return Quad(MUL, d, a, b); }
	inline static Quad div(reg_t d, reg_t a, reg_t b) { return Quad(DIV, d, a, b); }
	inline static Quad mod(reg_t d, reg_t a, reg_t b) { return Quad(MOD, d, a, b); }
	inline static Quad and_(reg_t d, reg_t a, reg_t b) { return Quad(AND, d, a, b); }
	inline static Quad or_(reg_t d, reg_t a, reg_t b) { return Quad(OR, d, a, b); }
	inline static Quad xor_(reg_t d, reg_t a, reg_t b) { return Quad(XOR, d, a, b); }
	inline static Quad shl(reg_t d, reg_t a, reg_t b) { return Quad(SHL, d, a, b); }
	inline static Quad shr(reg_t d, reg_t a, reg_t b) { return Quad(SHR, d, a, b); }
	inline static Quad rol(reg_t d, reg_t a, reg_t b) { return Quad(ROL, d, a, b); }
	inline static Quad ror(reg_t d, reg_t a, reg_t b) { return Quad(ROR, d, a, b); }
	inline static Quad lab(lab_t l) { return Quad(LAB, l); }
	inline static Quad goto_(lab_t l) { return Quad(GOTO, l); }
	inline static Quad goto_eq(lab_t l, reg_t a, reg_t b) { return Quad(GOTO_EQ, l, a, b); }
	inline static Quad goto_ne(lab_t l, reg_t a, reg_t b) { return Quad(GOTO_NE, l, a, b); }
	inline static Quad goto_lt(lab_t l, reg_t a, reg_t b) { return Quad(GOTO_LT, l, a, b); }
	inline static Quad goto_le(lab_t l, reg_t a, reg_t b) { return Quad(GOTO_LE, l, a, b); }
	inline static Quad goto_gt(lab_t l, reg_t a, reg_t b) { return Quad(GOTO_GT, l, a, b); }
	inline static Quad goto_ge(lab_t l, reg_t a, reg_t b) { return Quad(GOTO_GE, l, a, b); }
	inline static Quad call(lab_t l) { return Quad(CALL, l); }
	inline static Quad return_() { return Quad(RETURN); }
	inline static Quad load(reg_t d, reg_t a) { return Quad(LOAD, d, a); }
	inline static Quad store(reg_t a, reg_t b) { return Quad(STORE, 0, a, b); }
	inline static Quad push(reg_t a) { return Quad(PUSH, 0, a); }
	inline static Quad pop(reg_t d) { return Quad(POP, d); }

	void print(ostream& out) const;
};
inline ostream& operator<<(ostream& out, const Quad& q) { q.print(out); return out; }


class QuadProgram {
public:
	QuadProgram();
	void emit(const Quad& q);
	Quad::reg_t newReg();
	Quad::lab_t newLab();
	Quad::reg_t declare(string name);
	Quad::reg_t regFor(string name);
	void print(ostream& out);
	void comment(string com);
	CFG<Quad> *makeCFG();
private:
	unsigned _vreg;
	unsigned _lab;
	list<Quad> _quads;
	map<string, Quad::reg_t> _map;
	list<pair<int, string> > _coms;
};

#endif // IOC_QUAD_HPP
