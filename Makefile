CC=gcc
OPTS?=-O1
CFLAGS=-I. -march=armv8.2-a+sve
DEPS =
OBJ = test1.o sve_assembly-debug.os

%.os: %.S $(DEPS)
	$(CC) -c -o $@ $< $(OPTS) $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(OPTS) $(CFLAGS)

test1: $(OBJ)
	$(CC) -o $@ $^ $(OPTS) $(CFLAGS)

clean:
	rm -f *.o *.os test1
