all: pacc

lex.yy.c: flex.l y.tab.h
	flex flex.l
