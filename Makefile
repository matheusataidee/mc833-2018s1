CC=gcc

default: clean base server client
	

clean:
	@rm -v -f subject_object.o
	@rm -v -f parser.o
	@rm -v -f client.o
	@rm -v -f server.o
	@rm -v -f client
	@rm -v -f server

base:
	$(CC) -c subject_object.c
	$(CC) -c parser.c
	
server: 
	$(CC) -c server.c
	$(CC) parser.o subject_object.o server.o -o server

client:
	$(CC) -c client.c
	$(CC) parser.o subject_object.o client.o -o client
	
