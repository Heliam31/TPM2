#ifndef IOC_INST_HPP
#define IOC_INST_HPP

#include <string>
#include <cstdint>
using namespace std;

#include "Quad.hpp"

class Param {
public:

	typedef enum {
		NONE,
		CST,
		READ,
		WRITE
	} type_t;

	inline Param(): _type(NONE), _val(0) {}
	inline Param(type_t type, uint32_t val): _type(type), _val(val) {}

	static Param none;
	static inline Param read(int n) { return Param(READ, n); }
	static inline Param write(int n) { return Param(WRITE, n); }
	static inline Param cst(uint32_t v) { return Param(CST, v); }

	inline type_t type() const { return _type; }
	inline int32_t value() const { return _val; }
	void print(ostream& out) const;

private:
	type_t _type;
	uint32_t _val;
};
inline ostream& operator<<(ostream& out, const Param& p)
	{ p.print(out); return out; }


class Inst {
public:
	static const int param_num = 4;

	inline Inst(): _fmt(nullptr) {}
	inline Inst(const char *fmt): _fmt(fmt) {}
	inline Inst(const char *fmt, const Param& p0): _fmt(fmt)
		{ _params[0] = p0; }
	inline Inst(const char *fmt, const Param& p0, const Param& p1): _fmt(fmt)
		{ _params[0] = p0; _params[1] = p1; }
	inline Inst(const char *fmt, const Param& p0, const Param& p1, const Param& p2): _fmt(fmt)
		{ _params[0] = p0; _params[1] = p1;_params[2] = p2;  }
	inline Inst(const char *fmt, const Param& p0, const Param& p1, const Param& p2, const Param& p3): _fmt(fmt)
		{ _params[0] = p0; _params[1] = p1; _params[2] = p2; _params[3] = p3;  }

	inline const char *format() const { return _fmt; }
	inline const Param& operator[](int i) const { return _params[i]; }
	inline Param& operator[](int i) { return _params[i]; }

	void print(ostream& out) const;
	static Inst end;

private:
	const char *_fmt;
	Param _params[param_num];
};
inline ostream& operator<<(ostream& out, const Inst& i)
	{ i.print(out); return out; }


list<Inst> select(const list<Quad>& quads);

#endif // IOC_INST_HPP
