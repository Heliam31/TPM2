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


