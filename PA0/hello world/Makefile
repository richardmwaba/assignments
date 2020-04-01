# More complicated Makefile

# Macros to make life easier
CC=g++
CFLAGS=
LIBS=
OBJ=hello_world.o
DEPS=

%.o: %.c %.h $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hello_world: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o hello_world

run:
	./hello_world