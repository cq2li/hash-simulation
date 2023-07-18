CC = g++

CFLAGS = -O3

FILES = simulate.o

all: $(FILES)

simulate.o: simulate.C
			$(CC) $(CFLAGS) simulate.C -o ./bin/simulate.o

.PHONY: clean
clean:
			rm -f ./bin/simulate.o

