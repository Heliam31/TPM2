#include "AST.hpp"

/****** Reduce for expressions ******/

Expression *ConstExpr::reduce() {
	return this;
}

Expression *MemExpr::reduce() {
	if(_dec->type() != Declaration::CST)
		return this;
	else
		return new ConstExpr(static_cast<ConstDecl *>(_dec)->value());
}


Expression *UnopExpr::reduce() {
	_arg = _arg->reduce();
	if(_arg->type() == CST)
		return new ConstExpr(*eval());
	else
		return this;
}

Expression *BinopExpr::reduce() {
	auto a1 = _arg1->reduce();
	if(!a1)
		return {};
	auto a2 = _arg2->reduce();
	if (!_arg2)
		return {};

	auto a1Eval = a1->eval();
	auto a2Eval = a2->eval();	
	switch(_op) {
		case ADD:
			return new ConstExpr(*a1Eval + *a2Eval);
		case SUB:
			return new ConstExpr(*a1Eval - *a2Eval);
		case MUL:
			return new ConstExpr(*a1Eval * *a2Eval);
		case DIV:
			return new ConstExpr(*a1Eval / *a2Eval);
		case MOD:
			return new ConstExpr(*a1Eval % *a2Eval);
		case BIT_AND:
			return new ConstExpr(*a1Eval && *a2Eval);
		case BIT_OR:
			return new ConstExpr(*a1Eval || *a2Eval);
		case XOR:
			return new ConstExpr(*a1Eval ^ *a2Eval);
		case SHL:
			return new ConstExpr(*a1Eval << *a2Eval);
		case SHR:
			return new ConstExpr(*a1Eval >> *a2Eval);
		case ROL:
			return new ConstExpr((*a1Eval << *a2Eval) | (*a1Eval >> (to_string(*a1Eval).size() - *a2Eval)));
		case ROR:
			return new ConstExpr((*a1Eval >> *a2Eval) | (*a1Eval << (to_string(*a1Eval).size() - *a2Eval))); 
		default:
			return {};
		}
}

Expression *BitFieldExpr::reduce() {
}


/****** reduce for statements ******/

void NOPStatement::reduce() {
}

void SeqStatement::reduce() {
	_stmt1->reduce();
	_stmt2->reduce();
}

void SetStatement::reduce() {
	_expr = _expr->reduce();
}

void SetFieldStatement::reduce() {
}

void IfStatement::reduce() {
}

void GotoStatement::reduce() {
}

void StopStatement::reduce() {
}


/****** Reduction for conditions ******/

void CompCond::reduce() {
	_arg1 = _arg1->reduce();
	_arg2 = _arg2->reduce();
}

void NotCond::reduce() {
	_cond->reduce();
}

void BinCond::reduce() {
	_cond1->reduce();
	_cond2->reduce();
}


