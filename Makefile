CC=gcc
CFLAGS=-O0 -I. -march=armv8.2-a+sve
DEPS = 
OBJ = test1.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o test1
