
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
SRC=$(wildcard *.c) $(wildcard hashtable/*.c) lex.yy.c anasyn.tab.c
OBJS=$(SRC:.c=.o)
EXE=monCompilo




all: $(EXE)

$(EXE): $(OBJS)
	$(CC) `echo $(OBJS) | tr ' ' '\n' | sort -u` -o $@ $(LDFLAGS)
	$(STRIP) $@ > /dev/null

main.o: anasyn.tab.o lex.yy.o
anasyn.tab.o: anasyn.h
arbre.o: arbre_expr.h assert.h arbre.h
arbre_expr.o: assert.h arbre_expr.h
lex.yy.o: anasyn.tab.h anasyn.h
lexique.o: hashtable/hashtable.h lexique.h assert.h
main.o: anasyn.tab.h anasyn.h
hashtable/hashtable.o: hashtable/hashtable.h hashtable/hashtable_private.h
hashtable/hashtable_itr.o: hashtable/hashtable.h hashtable/hashtable_private.h hashtable/hashtable.h hashtable/hashtable_private.h hashtable/hashtable_itr.h


anasyn.tab.c: anasyn.y
	bison -dv anasyn.y

lex.yy.c: analex.lex
	flex analex.lex

.PHONY: clean mrproper test

clean:
	@rm -f *.o */*.o *~ core *.core core.* *.tmp lex.yy.c anasyn.tab.{h,c} anasyn.output

mrproper: clean
	@rm -f $(EXE)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

