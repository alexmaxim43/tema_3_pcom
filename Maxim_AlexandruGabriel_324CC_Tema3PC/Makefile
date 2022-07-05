CC=g++
CFLAGS=-I.

client: client.cpp requests.c helpers.c buffer.c
	$(CC) $(CFLAGS) -o client client.cpp requests.c helpers.c buffer.c -Wall

run: client
	./client

clean:
	rm -f *.o client
