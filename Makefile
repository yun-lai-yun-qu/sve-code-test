CC=gcc
CFLAGS=-O0 -I. -march=armv8.2-a+sve
DEPS =
OBJ = test1.o sve_assembly-debug.os

%.os: %.S $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


clean:
	rm -f *.o *.os test1
