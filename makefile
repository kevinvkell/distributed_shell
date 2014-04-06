#Kevin Kell
#3/25/13

all: client server test_client

server: server.c shell.c
	gcc -Wall -g -lcrypt server.c shell.c -o server -I .

client: client.c
	gcc -Wall -g -lcrypt client.c -o client

test_client: test_client.c
	gcc -Wall -g -lcrypt test_client.c -o test_client

clean:
	rm server client test_client 
