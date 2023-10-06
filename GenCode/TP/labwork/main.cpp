#include <vector>
#include <cstdint>
#include "AST.hpp"
#include "parser.hpp"
#include "Inst.hpp"
#include "RegAlloc.hpp"

// Only for compatibility with Flex
#include <stdio.h>
#include <map>
extern FILE *yyin;
extern const char *lexer_file;

/**
 * Generate the CFG of machine instructions from the CFG of quads.
 * @param g		CFG of quads.
 * @return		CFG of instructions.
 */
CFG<Inst> *selectInstructions(CFG<Quad> *g) {
	CFG<Inst> *r = new CFG<Inst>();

	// build BBs
	map<BB<Quad> *, BB<Inst> *> map;
	for(auto bb: g->basicBlocks())
		if(bb == g->entry())
			map[g->entry()] = r->entry();
		else if(bb == g->exit())
			map[g->exit()] = r->exit();
		else {
			auto rbb = new BB<Inst>();
			r->add(rbb);
			map[bb] = rbb;
			rbb->setInstructions(select(bb->instructions()));
		}

	// build edges
	for(auto bb: g->basicBlocks()) {
		auto rbb = map[bb];
		if(bb->next() != nullptr)
			rbb->setNext(map[bb->next()]);
		if(bb->target() != nullptr)
			rbb->setTarget(map[bb->target()]);
	}

	return r;
}


/**
 * Allocate the registers in the CFG of registers.
 * @param g		CFG of registers.
 * @param prog	Current program.
 */
void allocRegisters(CFG<Inst>& g, QuadProgram& prog) {

	// prepare mapper
	StackMapper map;
	for(auto d: Declaration::symbols())
		if(d.second->type() == Declaration::VAR)
			map.add(prog.regFor(static_cast<VarDecl *>(d.second)->name()));

	// allocate the registers
	for(auto v: g.basicBlocks()) {
		for(auto i: v->instructions())
			;
		map.rewind();
	}
}


/**
 * Print help message.
 */
void printHelp() {
	cerr << "SYNTAX: ioc [options] FILE.ioc\n"
		 << "Options may be:\n"
		 << "-h, --help    - display this message.\n"
		 << "-print-ast    - print AST and stop.\n"
		 << "-print-cfg    - print quadruplet CFG.\n"
		 << "-print-quads  - print the quadruplets.\n"
		 << "-print-select - print the selected instructions.\n"
		 << "-reduce-const - reduce constant expressions.\n";
}

/**
 * Compiler entry.
 */
int main(int argc, const char **argv) {
	string source;
	bool print_ast = false;
	bool reduce_const = false;
	bool print_quads = false;
	bool print_cfg = false;
	bool print_select = false;

	// parse arguments
	for(int i = 1; i < argc; i++) {
		string arg = argv[i];
		if(arg == "")
			continue;
		else if(arg[0] != '-') {
			if(source == "")
				source = arg;
			else {
				printHelp();
				cerr << "ERROR: several sources provided!" << endl;
				exit(1);
			}
		}
		else if(arg == "-print-ast")
			print_ast = true;
		else if(arg == "-reduce-const")
			reduce_const = true;
		else if(arg == "-print-quads")
			print_quads = true;
		else if(arg == "-print-cfg")
			print_cfg = true;
		else if(arg == "-print-select")
			print_select = true;
		else if(arg == "-h" || arg == "--help") {
			printHelp();
			return 1;
		}
		else {
			printHelp();
			cerr << "ERROR: unknown argument " << arg << endl;
			return 2;
		}
	}

	// perform analaysis
	try {
		if(source == "")
			yyparse();
		else {
			yyin = fopen(source.c_str(), "r");
			if(yyin == NULL) {
				cerr << "ERROR: cannot open '" << source << "'" << endl;
				return 2;
			}
			lexer_file = source.c_str();
			yyparse();
			fclose(yyin);
		}
	}
	catch(const ParseException& e) {
		cerr << "ERROR:" << e.pos() << ": " << e.msg() << endl;
		return 1;
	}

	// reduce constant
	if(reduce_const)
		for(auto s: Declaration::symbols())
			s.second->reduce();

	// perform post-processing
	if(print_ast)
		for(auto s: Declaration::symbols())
			s.second->print(cout);

	// compile in quadruplets
	QuadProgram quads;
	AutoDecl *automaton = nullptr;
	for(auto s: Declaration::symbols())
		if(s.second->type() == Declaration::VAR)
			quads.declare(s.first);
		else if(s.second->type() == Declaration::AUTO)
			automaton = static_cast<AutoDecl *>(s.second);
	if(automaton == nullptr) {
		cerr << "ERROR: no automaton in this file: '" << source << "'" << endl;
		return 3;
	}
	automaton->gen(quads);

	// print quads if need
	if(print_quads)
		quads.print(cout);

	// build CFG
	auto cfg = quads.makeCFG();

	// print CFG if needed
	if(print_cfg)
		cfg->print(cout);

	// select instructions
	auto inst_cfg = selectInstructions(cfg);

	// print instructions if needed
	if(print_select)
		inst_cfg->print(cout);

	// clean all
	Declaration::clearSymTab();
	return 0;
}
