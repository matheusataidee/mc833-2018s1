#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subject_object.h"

int main() {
  course *course_ = read("materias.txt");
  for (int i = 0; i < course_->n_subjects; i++) {
    printf("%s %s %s %s %s\n", course_->subjects[i]->code, course_->subjects[i]->title, course_->subjects[i]->program, course_->subjects[i]->classroom, course_->subjects[i]->class_time);
  }
  char *s = getAllSubjects(course_);
  printf("%s\n", s);
  s = getProgramByCode(course_, "MC202");
  printf("%s\n", s);
  s = getAllInfoByCode(course_, "MC404");
  printf("%s\n", s);
  s = getEveryInfo(course_);
  printf("%s\n", s);
  writeComment(course_, "MC102", "fala galerinhaaa");
  s = getCommentbyCode(course_, "MC102");
  printf("%s\n", s);
  return 0;
}
