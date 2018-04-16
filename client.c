#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "parser.h"

int main() {
  const char* server_name = "localhost";
  const int server_port = 8888;
  
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;

	// creates binary representation of server name
	// and stores it as sin_addr
	inet_pton(AF_INET, server_name, &server_address.sin_addr);

	// htons: port in network order format
	server_address.sin_port = htons(server_port);

	// open a stream socket
	int sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("could not create socket\n");
		return 1;
	}

	// TCP is connection oriented, a reliable connection
	// **must** be established before any data is exchanged
	if (connect(sock, (struct sockaddr*)&server_address,
	            sizeof(server_address)) < 0) {
		printf("could not connect to server\n");
		return 1;
	} else {
    printf("Conexao com servidor estabelecida.\n");
  }


	int n = 0;
	int len = 0, maxlen = 3000;
	char buffer[maxlen];
	char* pbuffer = buffer;
  char* data_to_send = (char *)(malloc(1025 * sizeof(char)));
  request *req = (request *)(malloc(sizeof(request)));

	// will remain open until the server terminates the connection
  
  printf("id\tOperacao\n");
  printf("1\tlistar todos os códigos de disciplinas com seus respectivos títulos\n");
  printf("2\tdado o código de uma disciplina, retornar a ementa\n");
  printf("3\tdado o código de uma disciplina, retornar todas as informações desta disciplina\n");
  printf("4\tlistar todas as informações de todas as disciplinas\n");
  printf("5\tescrever um texto de comentário sobre a próxima aula de uma disciplina\n");
  printf("6\tdado o código de uma disciplina, retornar o texto de comentário sobre a próxima aula\n");
  
  
  while (1) {
    
    req->code = (char *)(malloc(50 * sizeof(char)));
    printf("Digite id da operacao: ");
    scanf("%d", &req->id);
    if (req->id != 1 && req->id != 4) {
      printf("Digite codigo da disciplina: ");
      scanf("%s", req->code);
      if (req->id == 5) {
        printf("Digite comentario: ");
        req->comment = (char *)(malloc(300 * sizeof(char)));
        getchar();
        fgets(req->comment, 300, stdin);
        req->comment[strlen(req->comment) - 1] = '\0';
      }
    }
    data_to_send = requestToString(req);
    
    
    
    send(sock, data_to_send, strlen(data_to_send), 0);
    
    while ((n = recv(sock, pbuffer, maxlen, 0)) > 0) {
      buffer[n] = '\0';
      printf("%s\n", buffer);
      break;
    }
  }

	// close the socket
	close(sock);
	return 0;
}
