#include <set>
#include <vector>
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
		list<Inst> nlist;
		RegAlloc alloc(map, nlist);
		for(auto i: v->instructions())
			alloc.process(i);
		alloc.complete();
		v->setInstructions(nlist);
		map.rewind();
	}
}


/**
 * Output assembly from the CFG from the given stream.
 * @param g		Instruction CFG to output.
 * @param out	Output stream to output to.
 */
void outputAssembly(CFG<Inst>& g, ostream& out) {

	// generate prolog
	out << "\t.global main\n"
		<< "\n"
		<< "_main:" << endl;

	// generate body
	set<BB<Inst> *> todo, done;
	todo.insert(g.entry());
	while(!todo.empty()) {
		auto bb = *todo.begin();
		while(bb != nullptr) {
			for(auto i: bb->instructions())
				out << i << endl;
			todo.erase(bb);
			done.insert(bb);
			if(bb->target() != nullptr
			&& done.find(bb->target()) == done.end())
				todo.insert(bb->target());
			bb = bb->next();
		}
	}

	// generate epilog
	out << "\tbx LR" << endl;

	// generate run-time
	out << endl
		<< "@ R0 = e, R1 = u, R2 = l\n"
		<< "get_field:\n"
		<< "L10000:\n"
		<< "	stmfd sp!, {R1, R2}\n"
		<< "	mov R0, R0, lsr R2\n"
		<< "	sub R1, R1, R2\n"
		<< "	add R1, R1, #1\n"
		<< "	mov R2, #1\n"
		<< "	mov R2, R2, lsl R1\n"
		<< "	sub R2, R2, #1\n"
		<< "	and R0, R0, R2\n"
		<< "	ldmfd sp!, {R1, R2}\n"
		<< "	bx  LR\n"
		<< endl
		<< "@ R0 = i, R1 = u, R2 = l, R3 = e\n"
		<< "set_field:\n"
		<< "L10001:\n"
		<< "	stmfd sp!, {R1, R3, R4}\n"
		<< "	sub R1, R1, R2\n"
		<< "	add R1, R1, #1\n"
		<< "	mov R4, #1\n"
		<< "	mov R4, R4, lsl R1\n"
		<< "	and R3, R3, R4\n"
		<< "	mov R3, R3, lsl R2\n"
		<< "	mov R4, R4, lsl R2\n"
		<< "	mvn R4, R4\n"
		<< "	and R0, R0, R4\n"
		<< "	orr R0, R0, R3\n"
		<< "	ldmfd sp!, {R1, R3, R4}\n"
		<< "	bx  LR\n"
	;
}


/**
 * Print help message.
 */
void printHelp() {
	cerr << "SYNTAX: ioc [options] FILE.ioc\n"
		 << "Options may be:\n"
		 << "-h, --help     - display this message.\n"
		 << "-S, --assembly - generate assembly.\n"
		 << "-print-alloc   - print the instructions after register allocation.\n"
		 << "-print-ast     - print AST and stop.\n"
		 << "-print-cfg     - print quadruplet CFG.\n"
		 << "-print-quads   - print the quadruplets.\n"
		 << "-print-select  - print the selected instructions.\n"
		 << "-reduce-const  - reduce constant expressions.\n";
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
	bool print_alloc = false;
	bool assembly = false;

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
		else if(arg == "-print-alloc")
			print_alloc = true;
		else if(arg == "-S" || arg == "--assembly")
			assembly = true;
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
	if(print_select)
		inst_cfg->print(cout);

	// allocate registers
	allocRegisters(*inst_cfg, quads);
	if(print_alloc)
		inst_cfg->print(cout);

	// output machine instructions
	if(assembly)
		outputAssembly(*inst_cfg, cout);

	// clean all
	Declaration::clearSymTab();
	return 0;
}
