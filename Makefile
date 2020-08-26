all: server client
server: server.c 
	gcc -o server -w server.c
client: client.c
	gcc -o client -w client.c
clear:
	rm -f server client
