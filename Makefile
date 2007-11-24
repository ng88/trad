
CC=gcc
CFLAGS=-W -Wall
LDFLAGS=-lm
OBJS= $(SRC:.c=.o)
EXE=monCompilo


test:
	bison -dv anasyn.y
	flex analex.lex
	gcc  lex.yy.c anasyn.tab.c main.c  -lm -o test


all: $(EXE)

$(EXE): $(OBJS)

main.o: anasyn.tab.c lex.yy.c

anasyn.tab.c: anasyn.y
	 bison -dv anasyn.y

lex.yy.c: analex.lex
	flex analex.lex

.PHONY: clean mrproper test

clean:
	@rm -f *.o *~ core *.core core.* *.tmp

mrproper: clean
	@rm -f $(EXE)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

