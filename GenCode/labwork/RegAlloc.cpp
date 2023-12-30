#include <cassert>
#include <vector>
using namespace std;
#include "RegAlloc.hpp"
/*make clean; make; ./ioc -print-alloc test/zero.io*/
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
	return p != _offsets.end() && (*p).second < _global;
}

/**
 * Rewind the size of the stack to the given size to remove temporary allocation,
 * to keep only global variables.
 */
void StackMapper::rewind() {
	_offset = _global;
	vector<int32_t> to_remove;
	for(auto p: _offsets)
		if(p.second < _global)
			to_remove.push_back(p.first);
	for(auto v: to_remove)
		_offsets.erase(v);
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
	for (int i = 0; i < inst.param_num; i++){
        Param& param = inst[i];
        if(param.type() == Param::READ)
            processRead(param);
        else if(param.type() == Param::WRITE)
            processWrite(param);

        inst[i] = param;
    }
	// add the fixed instruction
	_insts.push_back(inst);
	_fried.clear();
}

/**
 * Complete the allocation of a BB by generating store of modified global variables.
 */
void RegAlloc::complete() {
	for (Quad::reg_t r : _written)
        store(r);
}

/**
 * Allocate a read register.
 * @param param		Parameter to fix.
 */
void RegAlloc::processRead(Param& param) {
	assert("parameter should be a read parameter!" && param.type() == Param::READ);
	Quad::reg_t rVal = param.value();
    Quad::reg_t reg = allocate(rVal);
    param = param.read(reg);
    load(reg);
}

/**
 * Allocata write register.
 * @param param		Parameter to fix.
 */
void RegAlloc::processWrite(Param& param) {
	assert("parameter should be a write parameter!" && param.type() == Param::WRITE);
    Quad::reg_t rVal = param.value();
    Quad::reg_t reg = allocate(rVal);
    param = param.write(reg);
    if (isVar(reg))
        _written.push_back(reg);
}

/**
 * Allocate an hardware register through the free ones or spill a register
 * to get a new free hardware register.
 */
Quad::reg_t RegAlloc::allocate(Quad::reg_t reg) {
	if (isVar(reg) == 1){
		return _map[reg];
	}
	else{
		if (_avail.empty()){
			 assert("no more registers available!" && false);
		}
		else{
			_map[reg] = _avail.back();
			_avail.pop_back();
			return _map[reg];
		}
	}
}

/**
 * Generate code to spill the given virtual register.
 * @param reg	Virtual register to spill.
 */
void RegAlloc::spill(Quad::reg_t reg) {
	store(reg);
	_avail.push_front(_map[reg]);
	_map.erase(reg);
}

/**
 * Free the given virtual register.
 * @param reg	Virtual register to free.
 */
void RegAlloc::free(Quad::reg_t reg) {
	_avail.push_back(_map[reg]);
	_map.erase(reg);
}

/**
 * Generate a store instruction to the stack.
 * @param reg		Virtual register to store.
 */
void RegAlloc::store(Quad::reg_t reg) {
	auto hreg = _map[reg];
	auto offset = _mapper.offsetOf(reg);
	_insts.push_back(Inst("\tstr R%0, [SP, #%1]", Param::read(hreg), Param::cst(offset)));
}

/**
 * Generate a load from the stack.
 * @param reg		Virtual register to load to.
 * @param offset	Offset in the stack of the value to load.
 */
void RegAlloc::load(Quad::reg_t reg) {
	auto hreg = _map[reg];
	auto offset = _mapper.offsetOf(reg);
	_insts.push_back(Inst("\tldr R%0, [writeSP, #%1]", Param::write(hreg), Param::cst(offset)));
}

/**
 * Test if a virtual register contains a variable.
 * @param reg	Virtual register to test.
 * @return		True if reg contains a IOML variable false else.
 */
bool RegAlloc::isVar(Quad::reg_t reg) const {
	return _mapper.isGlobal(reg);
}
