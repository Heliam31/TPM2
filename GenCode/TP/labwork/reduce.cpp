#include "AST.hpp"
/*make clean; make; ./ioc -print-quads test/zero.io*/
/*make clean; make; ./ioc -print-ast -reduce-const test/eval.io*/
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
	auto a2 = _arg2->reduce();
	if((_arg1->type() == CST) && (_arg2->type() == CST)){
		return new ConstExpr(*eval());
	}
	else 
	return this;
	/*
	auto a1Eval = a1->eval();
	auto a2Eval = a2->eval();	
	if(!a1Eval)
		return {};
	if (!a2Eval)
		return {};
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
	else{
		return{};
	}*/
}

Expression *BitFieldExpr::reduce() {
	auto exR = _expr -> reduce();
	if(exR->type() == BITFIELD){
		return new ConstExpr(*eval());
	}
	else
		return this;
	// if(!ex)
	// 	return {};
	// auto hiR = _hi->reduce();
	// auto loR = _lo->reduce();
	// auto hi = _hi -> eval();
	// auto lo = _lo -> eval();
	// if(!hi && !lo)
	// 	return {};
	// if(hi == lo)
	// 	return new ConstExpr((*ex >> *hi) & 1);
	// return new ConstExpr((*ex >> *lo) & ((1 <<(*hi -*lo + 1)) -1)); 
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


