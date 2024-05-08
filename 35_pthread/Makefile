CC := gcc
SRC=$(wildcard *.c)
BIN=$(SRC:%.c=%)

all:$(BIN)

%:%.c
	$(CC) $^ -o $@ -lpthread

.PHONY:
clean:
	rm $(BIN)
