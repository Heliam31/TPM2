#include <cassert>
using namespace std;
#include "RegAlloc.hpp"

/**
 * @class StackMapper
 * Map the variable to stack offset.
 */

/**
 */
StackMapper::StackMapper(): _offset(0), _global(0) {
}

/**
 * Add a variable to the map.
 * @param reg	Assign a stack offset to the register.
 */
void StackMapper::add(Quad::reg_t reg) {
	_offset -= 4;
	_offsets[reg] = _offset;
}

/**
 * Get the offset of a register.
 * @param reg	Register to get offset of.
 * @return		Get an offset for the register, possibly allocate it.
 */
int32_t StackMapper::offsetOf(Quad::reg_t reg) {
	auto x =_offsets.find(reg);
	if(x != _offsets.end())
		return (*x).second;
	else {
		_offset -= 4;
		_offsets[reg] = _offset;
		return _offset;
	}
}

/**
 * @fn uint32_t StackMapper::stackSize() const;
 * Get the size to allocate in the stack.
 */

/**
 * Mark the current stack position as being the end of the global variable save area.
 */
void StackMapper::markGlobal() {
	_global = _offset;
}

/**
 * Test if a virtual register is a global variable register.
 * @param reg	Virtual register to look.
 * @return		True if it matches a global variable, false else.
 */
bool StackMapper::isGlobal(Quad::reg_t reg) {
	auto p = _offsets.find(reg);
	return p == _offsets.end() || (*p).second >= _global;
}

/**
 * Rewind the size of the stack to the given size to remove temporary allocation,
 * to keep only global variables.
 */
void StackMapper::rewind() {
	_offset = _global;
	for(auto p: _offsets)
		if(p.second < _global)
			_offsets.erase(p.first);
}


/**
 * @class RegAlloc
 * Supports allocation of register for a BB.
 */

/**
 * Build a register allocator.
 * @param mapper	Mapper for stack allocation (when variable have been allocated).
 * @param insts		List of instruction to complete with allocated instructions
 * 					and stack store/load instructions.
 */
RegAlloc::RegAlloc(StackMapper& mapper, list<Inst>& insts)
: _mapper(mapper), _insts(insts) {
	for(int i = 0; i < Quad::ALLOC_COUNT; i++)
		_avail.push_back(i);
}

/**
 * Perform allocation in one instruction and add the instruction to the list.
 * @param inst		Instruction sto process.
 */
void RegAlloc::process(Inst inst) {
}

/**
 * Complete the allocation of a BB by generating store of modified global variables.
 */
void RegAlloc::complete() {
}

/**
 * Allocate a read register.
 * @param param		Parameter to fix.
 */
void RegAlloc::processRead(Param& param) {
}

/**
 * Allocata write register.
 * @param param		Parameter to fix.
 */
void RegAlloc::processWrite(Param& param) {
}

/**
 * Allocate an hardware register through the free ones or spill a register
 * to get a new free hardware register.
 */
Quad::reg_t RegAlloc::allocate(Quad::reg_t reg) {

}

void RegAlloc::spill(Quad::reg_t reg) {

}

void RegAlloc::free(Quad::reg_t reg) {

}


/**
 * Generate a store instruction to the stack.
 * @param reg		Hardware register to store.
 * @param offset	Offset in the stack to store to.
 */
void RegAlloc::store(Quad::reg_t reg, int offset) {
	assert("stored register must be a virtual register" && reg >= Quad::HARD_COUNT);
	_insts.push_back(Inst("str %0, [SP, #%1]", Param::read(reg), Param::cst(offset)));
}


/**
 * Generate a load from the stack.
 * @param reg		Hardware register to load to.
 * @param offset	Offset in the stack of the value to load.
 */
void RegAlloc::load(Quad::reg_t reg, int offset) {
	assert("loaded register must be an hardware register" && reg < Quad::HARD_COUNT);
	_insts.push_back(Inst("ldr %0, [SP, #%1]", Param::write(reg), Param::cst(offset)));
}
