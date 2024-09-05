CC = gcc
SRC = src/*.c
BIN_NAME = bettered.out

all: $(SRC)
	$(CC) -O -std=gnu11 -c $(SRC)
	$(CC) -O2 -std=gnu11 *.o -o $(BIN_NAME)
	rm *.o
install: all
	cp $(BIN_NAME) /usr/bin/bettered
	ln -s /usr/bin/bettered /usr/bin/ed
	rm $(BIN_NAME)
uninstall:
	rm /usr/bin/bettered && rm /usr/bin/ed
remove: uninstall
dev: $(SRC)
	$(CC) -g $(SRC) -o $(BIN_NAME)

.PHONY: all dev
