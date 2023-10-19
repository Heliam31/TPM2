#include "AST.hpp"
#include <string>
using namespace std;

///
optional<value_t> ConstExpr::eval() const {
	return _val;
}

///
optional<value_t> MemExpr::eval() const {
	switch(_dec->type()) {
	case Declaration::CST:
		return static_cast<ConstDecl *>(_dec)->value();
	default:
		return {};
	}
}


///
optional<value_t> UnopExpr::eval() const {
	auto a = _arg->eval();
	if(!a)
		return {};
	switch(_op) {
	case NEG: return -*a;
	case INV: return ~*a;
	default: return {};
	}
}


///
optional<value_t> BinopExpr::eval() const {
	auto a1 = _arg1->eval();
	if(!a1)
		return {};
	auto a2 = _arg2->eval();
	if(!a2)
		return {};
	switch(_op) {
	case ADD:
		return *a1 + *a2;
	case SUB:
		return *a1 - *a2;
	case MUL:
		return *a1 * *a2;
	case DIV:
		return *a1 / *a2;
	case MOD:
		return *a1 % *a2;
	case BIT_AND:
		return *a1 && *a2;
	case BIT_OR:
		return *a1 || *a2;
	case XOR:
		return *a1 ^ *a2;
	case SHL:
		return *a1 << *a2;
	case SHR:
		return *a1 >> *a2;
	case ROL:
		return (*a1 << *a2) | (*a1 >> (to_string(*a1).size() - *a2)); 
	case ROR:
		return (*a1 >> *a2) | (*a1 << (to_string(*a1).size() - *a2)); 
	default:
		return {};
	}
}


///
optional<value_t> BitFieldExpr::eval() const {
	auto ex = _expr->eval();
	if(!ex)
		return {};
	auto hi = _hi->eval();
	auto lo = _lo->eval();
	if(!hi && !lo)
		return {};
	if(hi == lo)
		return ((*ex >> *hi) & 1);
	return ((*ex >> *lo) & ((1 <<(*hi -*lo + 1)) -1)); 
}
