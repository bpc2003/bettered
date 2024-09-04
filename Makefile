CC = gcc

bettered:
	$(CC) -O2 -std=gnu11 src/*.c -o bettered
install: bettered
	mv bettered /usr/bin
	ln -s /usr/bin/bettered /usr/bin/ed
uninstall:
	rm /usr/bin/bettered && rm /usr/bin/ed
dev:
	cc -g src/*.c
