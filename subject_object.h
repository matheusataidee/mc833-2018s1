#ifndef SUBJECT_OBJECT
#define SUBJECT_OBJECT

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct subject subject;
typedef struct course course;

struct subject {
  char *code;
  char *title;
  char *program;
  char *classroom;
  char *class_time;
  char *comment;
};

struct course {
  int n_subjects;
  subject **subjects;
};

subject* createSubject(char *code, char *title, char *program, char *classroom, char *class_time);

course* readCourse(char *file_name);

char* getAllSubjects(course *course_);

char* getProgramByCode(course *course_, char *given_code);

char* getAllInfoByCode(course *course_, char *given_code);

char* getEveryInfo(course *course_);

void writeComment(course *course_, char *given_code, char* comment);

char* getCommentbyCode(course *course_, char *given_code);

#endif
