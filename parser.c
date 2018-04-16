#include "parser.h"

request* createRequest(int id, char* code) {
  request *req;
  req = (request *)(malloc(sizeof(request)));
  req->id = id;
  req->code = (char *)(malloc((strlen(code) + 1) * sizeof(char)));
  strcpy(req->code, code);
  return req;
}

request* stringToRequest(char *req) {
  request *ret = (request *)(malloc(sizeof(request)));
  char **s = (char **)(malloc(3 * sizeof(char*)));
  int p = 0, j = 0;
  s[0] = (char *)(malloc((strlen(req) + 1) * sizeof(char)));
  for (int i = 0, n = strlen(req); i < n; i++) {
    if (req[i] == ';') {
      s[p++][j] = '\0';
      s[p] = (char *)(malloc((n + 1) * sizeof(char)));
      j = 0;
    } else {
      s[p][j++] = req[i];
    }
  }
  s[p][j] = '\0';
  ret->id = atoi(s[0]);
  ret->code = (char *)(malloc((strlen(s[1]) + 1) * sizeof(char)));
  strcpy(ret->code, s[1]);
  if (s[2]) {
    ret->comment = (char *)(malloc((strlen(s[2]) + 1) * sizeof(char)));
    strcpy(ret->comment, s[2]);
  }
  return ret;
}

char* requestToString(request *req) {
  char *ret;
  char num[12];
  int len = 1;
  sprintf(num, "%d", req->id);
  len += strlen(num) + 1;
  len += strlen(req->code) + 1;
  if (req->comment) {
    len += strlen(req->comment) + 1;
  }
  ret = (char *)(malloc(len * (sizeof(char))));
  ret[0] = '\0';
  strcat(ret, num);
  strcat(ret, ";");
  strcat(ret, req->code);
  if (req->comment) {
    strcat(ret, ";");
    strcat(ret, req->comment);
  }
  return ret;
}
