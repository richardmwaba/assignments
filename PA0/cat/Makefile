# More complicated Makefile

# Macros to make life easier
CC=g++
CFLAGS=
LIBS=
OBJ=my_cat.o
DEPS=

%.o: %.c %.h $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

my_cat: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o my_cat

run:
	./my_cat