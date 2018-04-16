#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> 
#include <unistd.h>   //close 
#include <arpa/inet.h>    //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
#include "subject_object.h"
#include "parser.h"

#define TRUE   1 
#define FALSE  0 
#define PORT 8888 

// Recebe objeto do tipoe 'request' e retorna string com mensagem resposta.
char* handleNewMessage(request *req, course *course_) {
  char *ret;
  switch (req->id) {
    case 1:
      ret = getAllSubjects(course_);
      break;
    case 2:
      ret = getProgramByCode(course_, req->code);
      break;
    case 3:
      ret = getAllInfoByCode(course_, req->code);
      break;
    case 4:
      ret = getEveryInfo(course_);
      break;
    case 5:
      writeComment(course_, req->code, req->comment);
      ret = (char *)(malloc(500 * sizeof(char)));
      sprintf(ret, "Comentario '%s' adicionado com sucesso para a materia %s.", req->comment, req->code);
      break;
    case 6:
      ret = getCommentbyCode(course_, req->code);
      break;
  }
  return ret;
}

int main() {
  // Le arquivo estatico com informacoes das materias.
  course *course_ = readCourse("materias.txt");

  int opt = TRUE;  
  int master_socket , addrlen , new_socket , client_socket[30] , 
        max_clients = 30 , activity, i , valread , sd;  
  int max_sd;  
  struct sockaddr_in address;  
      
  char buffer[2049];  //data buffer de 2K 
      
  //set of socket descriptors 
  fd_set readfds;  
  
  // Inicializa todos client_socket[] para 0.
  for (i = 0; i < max_clients; i++)  
  {  
      client_socket[i] = 0;  
  }  
      
  // Cria socket master. 
  if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
  {  
      perror("socket failed");  
      exit(EXIT_FAILURE);  
  }  
  
  // Seta socket master para permitir multiplas conexoes.
  if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
        sizeof(opt)) < 0 )  
  {  
      perror("setsockopt");  
      exit(EXIT_FAILURE);  
  }  
  
  // Tipo de socket criado.
  address.sin_family = AF_INET;  
  address.sin_addr.s_addr = INADDR_ANY;  
  address.sin_port = htons( PORT );  
      
  // Bind do socket com localhost por 8888
  if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)  
  {  
      perror("bind failed");  
      exit(EXIT_FAILURE);  
  }  
  printf("Listener on port %d \n", PORT);  
      
  // Especifica maximo de 3 conexoes pendentes para socket master.
  if (listen(master_socket, 3) < 0)  
  {  
      perror("listen");  
      exit(EXIT_FAILURE);  
  }  
      
  // Estado de espera por novas conexoes;
  addrlen = sizeof(address);  
  puts("Waiting for connections ...");  
      
  while(TRUE)  
  {
      // limpa socket set
      FD_ZERO(&readfds);  
  
      // adiciona socket master para set.
      FD_SET(master_socket, &readfds);  
      max_sd = master_socket;  
          
      // Adiciona sockets filhos no set
      for ( i = 0 ; i < max_clients ; i++)  
      {  
          sd = client_socket[i];  
              
          // Se socket descriptor for valido entao adicionar a lista.
          if(sd > 0)  
              FD_SET( sd , &readfds);  
              
          if(sd > max_sd)  
              max_sd = sd;  
      }  
  
      // Espera indefinidamente por atividade em um dos sockets.
      activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
    
      if ((activity < 0) && (errno!=EINTR))  
      {  
          printf("select error");  
      }  
          
      // Se algo acontece no socket master entao eh conexao chegando.
      if (FD_ISSET(master_socket, &readfds))  
      {  
          if ((new_socket = accept(master_socket, 
                  (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  
          {  
              perror("accept");  
              exit(EXIT_FAILURE);  
          }  
          
          // Informa client socket number usado por comandos de send e receive
          printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
                (address.sin_port));  
              
          // Adiciona novo socket a array de sockets.
          for (i = 0; i < max_clients; i++)  
          {  
              if( client_socket[i] == 0 )  
              {  
                  client_socket[i] = new_socket;  
                  printf("Adding to list of sockets as %d\n" , i);  
                      
                  break;  
              }  
          }  
      }  
          
      //else IO em algum socket
      for (i = 0; i < max_clients; i++)  
      {  
          sd = client_socket[i];  
              
          if (FD_ISSET( sd , &readfds))  
          {  
              // Checa se eh mensagem para fechar conexao, e le mensagem.
              if ((valread = read( sd , buffer, 2048)) <= 0)  
              {  
                  getpeername(sd , (struct sockaddr*)&address , \
                      (socklen_t*)&addrlen);  
                  printf("Host disconnected , ip %s , port %d \n" , 
                        inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                      
                  // Fecha socket.
                  close( sd );  
                  client_socket[i] = 0;  
              }  
                  
              else
              {  
                  buffer[valread] = '\0';  
                  // Monta array de char com resposta.
                  char *ans = handleNewMessage(stringToRequest(buffer), course_);
                  // Usa socket para responder para cliente.
                  send(sd , ans , strlen(ans) , 0 );  
              }  
          }  
      }  
  }
  
  return 0;
}
