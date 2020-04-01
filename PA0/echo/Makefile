# More complicated Makefile

# Macros to make life easier
CC=g++
CFLAGS=
LIBS=
OBJ=my_echo.o
DEPS=

%.o: %.c %.h $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

my_echo: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o my_echo

run:
	./my_echo