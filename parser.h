#ifndef PARSER
#define PARSER

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct request request;

struct request {
  int id;
  char *code;
  char *comment;
};

request* stringToRequest(char *req);

char* requestToString(request *req);

#endif
