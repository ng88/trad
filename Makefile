
ifdef DEBUG
    DEBUGFLAGS=-g -ggdb -dH -D_DEBUG_=1
    STRIP=@echo
else
    DEBUGFLAGS=
    STRIP=strip
endif

ifdef NOASSERT
    ASSERTFLAGS=-DDISABLE_ASSERT=1
else
    ASSERTFLAGS=
endif

CC=gcc
CFLAGS=-W -Wall -Wno-unused $(DEBUGFLAGS) $(ASSERTFLAGS)
LDFLAGS=-lm
SRC=arbre.c arbre_classe.c arbre_compile.c arbre_expr.c arbre_instr.c arbre_printer.c arbre_sem.c error.c hashtable.c hashtable_itr.c lexique.c main.c stack.c tds.c vector.c
OBJS=$(SRC:.c=.o)
EXE=tradV1




all: $(EXE)

$(EXE): $(OBJS)
	$(CC) *.o -o $@ $(LDFLAGS)
	$(STRIP) $@ > /dev/null

main.o: anasyn.tab.o lex.yy.o
anasyn.tab.o: anasyn.h arbre_printer.h assert.h lexique.h
arbre.o: arbre_expr.h arbre_instr.h arbre_classe.h assert.h arbre.h
arbre_classe.o: arbre_instr.h tds.h spname.h assert.h arbre_classe.h anasyn.h
arbre_compile.o: arbre.h arbre_compile.h arbre_printer.h tds.h assert.h lexique.h anasyn.h error.h
arbre_expr.o: vector.h tds.h assert.h arbre_expr.h lexique.h
arbre_instr.o: arbre_expr.h tds.h assert.h arbre_instr.h lexique.h
arbre_printer.o: arbre.h assert.h arbre_printer.h lexique.h
arbre_sem.o: arbre.h assert.h arbre_sem.h error.h anasyn.h arbre_printer.h lexique.h spname.h
error.o: error.h
hashtable.o: hashtable.h hashtable_private.h
hashtable_itr.o: hashtable.h hashtable_private.h hashtable.h hashtable_private.h hashtable_itr.h
lexique.o: hashtable.h vector.h lexique.h assert.h
main.o: anasyn.tab.h anasyn.h lexique.h arbre_printer.h arbre_compile.h arbre_sem.h
stack.o: stack.h assert.h
tds.o: lexique.h vector.h bool.h tds.h assert.h arbre.h error.h
vector.o: vector.h assert.h

anasyn.tab.c: projet.yacc
	bison -dv projet.yacc
	@mv projet.tab.cacc anasyn.tab.c
	@mv projet.tab.hacc anasyn.tab.h

lex.yy.c: projet.lex
	flex projet.lex

.PHONY: clean mrproper test

clean:
	@rm -f *.o */*.o *~ core *.core core.* *.tmp lex.yy.c anasyn.tab.{h,c} projet.output

mrproper: clean
	@rm -f $(EXE)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)


