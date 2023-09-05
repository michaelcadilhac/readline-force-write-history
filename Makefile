SRC := readline_force_write_history.c
LIB := $(patsubst %.c,%.so,$(SRC))

%.so: %.c
	gcc -O3 -shared $< -o $@ -ldl -Wall -fPIC

all: $(LIB)
