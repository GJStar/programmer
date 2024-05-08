CC := gcc
CC_OPTION := -fPIC -shared -g
all: add sym demo
add:add.c
	$(CC) $(CC_OPTION) -o libadd.so add.c

sym:sym.c 
	$(CC) $(CC_OPTION) -o libsym.so sym.c

demo:demo.c
	$(CC) -o $@ $^ -ldl

.PHONY:
clean:
	rm -rf *.so demo
