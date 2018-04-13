CC=gcc

clean:
	@rm -v -f subject_object.o

clean_server: clean
	@rm -v -f server.o
	@rm -v -f server

base_server: clean_server
	$(CC) -c subject_object.c
	
server: base_server
	$(CC) -c server.c
	$(CC) subject_object.o server.o -o server
