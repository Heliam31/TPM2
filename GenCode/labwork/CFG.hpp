#ifndef IOC_CFG_HPP
#define IOC_CFG_HPP

#include <list>
#include <iostream>
using namespace std;

template <class T>
class BB {
public:

	inline BB(): _next(nullptr), _target(nullptr), _number(-1) {}

	~BB() {
		if(_next != nullptr)
			_next->removePred(this);
		if(_target != nullptr)
			_target->removePred(this);
	}

	inline const list<T>& instructions() const { return _insts; }
	inline const list<BB<T> *>& predecessors() const { return _preds; }
	inline BB<T> *next() const { return _next; }
	inline BB<T> *target() const { return _target; }
	inline int number() const { return _number; }

	void setNext(BB<T> *bb) {
		if(_next != nullptr)
			_next->removePred(this);
		bb->_preds.push_front(this);
		_next = bb;
	}

	void setTarget(BB<T> *bb) {
		if(_target != nullptr)
			_target->removePred(this);
		bb->_preds.push_front(this);
		_target = bb;
	}

	inline void setInstructions(const list<T>& insts) { _insts = insts; }

	inline void setNumber(int n) { _number = n; }

	void print(ostream& out) {
		out << "BB " << _number << ":" << endl;
		for(auto i: _insts)
			out << '\t' << i << endl;
	}

private:

	void removePred(BB<T> *bb) {
		for(auto i = _preds.begin(); i != _preds.end(); ++i)
			if(*i == bb ) {
				_preds.erase(i);
				break;
			}
	}

	list<T> _insts;
	list<BB<T> *> _preds;
	BB<T> *_next, *_target;
	int _number;
};

template <class T>
ostream& operator<<(ostream& out, BB<T> *bb)
	{ bb->print(out); return out; }

template <class T>
class CFG {
public:

	inline CFG() { add(&_entry); add(&_exit); }

	inline BB<T> *entry() { return &_entry; }
	inline BB<T> *exit() { return &_exit; }
	inline const list<BB<T> *>& basicBlocks() const { return _bbs; }

	void add(BB<T> *bb) {
		bb->setNumber(_bbs.size());
		_bbs.push_back(bb);
	}

	void print(ostream& out) {
		for(auto bb: _bbs) {
			if(bb == &_entry)
				out << "ENTRY" << endl;
			else if(bb == &_exit)
				out << "EXIT" << endl;
			else
				out << bb;
			if(bb->next() != nullptr)
				out << "\tNEXT BB" << bb->next()->number() << endl;
			if(bb->target() != nullptr)
				out << "\tTARGET BB" << bb->target()->number() << endl;
		}
	}

private:
	BB<T> _entry, _exit;
	list<BB<T> *> _bbs;
};

#endif	// IOC_CFG_HPP

