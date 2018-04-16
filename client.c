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

	inet_pton(AF_INET, server_name, &server_address.sin_addr);

	server_address.sin_port = htons(server_port);

  // Abre socket para conexao
	int sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Nao foi possivel criar socket\n");
		return 1;
	}

  // Estabelece conexao TCP com o servidor.
	if (connect(sock, (struct sockaddr*)&server_address,
	            sizeof(server_address)) < 0) {
		printf("Nao foi possivel conectar com o servidor\n");
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


  // Imprime menu com opcoes.
  printf("id\tOperacao\n");
  printf("1\tlistar todos os códigos de disciplinas com seus respectivos títulos\n");
  printf("2\tdado o código de uma disciplina, retornar a ementa\n");
  printf("3\tdado o código de uma disciplina, retornar todas as informações desta disciplina\n");
  printf("4\tlistar todas as informações de todas as disciplinas\n");
  printf("5\tescrever um texto de comentário sobre a próxima aula de uma disciplina\n");
  printf("6\tdado o código de uma disciplina, retornar o texto de comentário sobre a próxima aula\n");
  
  
  // Loop da conexao
  while (1) {
    
    // Objeto do tipo 'request' eh montado a partir das campos
    // indicados pelo usuario.
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
    // Converte request em string de char para ser enviado ao servidor.
    data_to_send = requestToString(req);
    
    
    // Envia mensagem.
    send(sock, data_to_send, strlen(data_to_send), 0);
    
    // Recebe resposta e printa na saida padrao.
    while ((n = recv(sock, pbuffer, maxlen, 0)) > 0) {
      buffer[n] = '\0';
      printf("%s\n", buffer);
      break;
    }
  }

	// Fecha socket.
	close(sock);
	return 0;
}
