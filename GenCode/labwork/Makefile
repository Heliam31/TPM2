SOURCES = \
	main.cpp \
	parser.cpp \
	lexer.cpp \
	AST.cpp \
	Quad.cpp \
	eval.cpp \
	reduce.cpp \
	gen.cpp \
	CFG.cpp \
	Inst.cpp \
	RegAlloc.cpp

OBJECTS = $(SOURCES:.cpp=.o)

#CXX = clang++
CXXFLAGS = -g

all: ioc

clean:
	rm -rf $(OBJECTS)

ioc: $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

main.o: AST.hpp parser.hpp
AST.o: AST.hpp Quad.hpp
parser.o: AST.hpp Quad.hpp
eval.o: AST.hpp Quad.hpp
Quad.o: Quad.hpp
reduce.o: AST.hpp Quad.hpp
gen.o: AST.hpp Quad.hpp
CFG.o: CFG.hpp
Inst.o: Inst.hpp
RegAlloc.o: Inst.hpp AST.hpp

parser.cpp parser.hpp: parser.yy
	bison -v $< -o parser.cpp -H

lexer.cpp: lexer.ll
	flex -o $@ $<


# Distribution building
FILES = \
	test/ \
	Makefile \
	TP1.md TP2.md TP3.md \
	AST.cpp AST.hpp \
	CFG.cpp CFG.hpp \
	Inst.hpp \
	lexer.ll \
	main.cpp \
	parser.yy \
	Quad.cpp Quad.hpp \
	RegAlloc.hpp
TO_FILTER = \
	eval.cpp \
	gen.cpp \
	Inst.cpp \
	reduce.cpp \
	RegAlloc.cpp
DIST_NAME=labwork

dist: dist_make dist_check dist_pack

dist_make:
	-rm -rf $(DIST_NAME)
	mkdir $(DIST_NAME)
	for f in $(FILES); do \
		cp -R $$f $(DIST_NAME); \
	done
	for f in $(TO_FILTER); do \
		autofilter.py < $$f > $(DIST_NAME)/$$f; \
	done

dist_check: dist_make
	cd $(DIST_NAME); make

dist_pack: dist_make
	tar cvfz $(DIST_NAME).tgz $(DIST_NAME)


# Assignment delivery
TP1_FILES = eval.cpp reduce.cpp
TP2_FILES = gen.cpp
TP3_FILES = Inst.cpp
TP4_FILES = RegAlloc.cpp

DATE = $(shell date +"%y%m%d")

tp1:
	tar cvfz $@-$(DATE).tgz $(TP1_FILES)

tp2:
	tar cvfz $@-$(DATE).tgz $(TP2_FILES)

tp3:
	tar cvfz $@-$(DATE).tgz $(TP3_FILES)

tp4:
	tar cvfz $@-$(DATE).tgz $(TP4_FILES)

