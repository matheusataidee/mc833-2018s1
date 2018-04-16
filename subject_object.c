#include "subject_object.h"

subject* createSubject(char *code, char *title, char *program, char *classroom, char *class_time) {
  subject *ret = (subject *)malloc(sizeof(subject));
  
  ret->code = (char *)malloc((strlen(code) + 1) * sizeof(char));
  strcpy(ret->code, code);
  ret->title = (char *)malloc((strlen(title) + 1) * sizeof(char));
  strcpy(ret->title, title);
  ret->program = (char *)malloc((strlen(program) + 1) * sizeof(char));
  strcpy(ret->program, program);
  ret->classroom = (char *)malloc((strlen(classroom) + 1) * sizeof(char));
  strcpy(ret->classroom, classroom);
  ret->class_time = (char *)malloc((strlen(class_time) + 1) * sizeof(char));
  strcpy(ret->class_time, class_time);
  
  return ret;
}

course* readCourse(char *file_name) {
  subject **subjects;
  course *course_;
  course_ = (course *)malloc(sizeof(course));
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  int n_lines = 0, it = 0;

  fp = fopen(file_name, "r");
  while ((read = getline(&line, &len, fp)) != -1) n_lines++;
  fclose(fp);
  if (line) free(line);
  
  subjects = (subject **)(malloc(n_lines * (sizeof(subject*))));
  
  fp = fopen(file_name, "r");
  while ((read = getline(&line, &len, fp)) != -1) {
    char **s = (char **)malloc(6 * (sizeof(char*)));
    
    int j = 0, p = 0;
    s[j] = (char *)(malloc(read * (sizeof(char))));
    for (int i = 0; i < read; i++) {
      if (line[i] != ';' && line[i] != '\n') {
        s[j][p++] = line[i];
      } else {
        s[j++][p] = '\0';
        s[j] = (char *)(malloc(read * (sizeof(char))));
        p = 0;
      }
    }
    
    subjects[it] = createSubject(s[0], s[1], s[2], s[3], s[4]);
    
    free(s);
    it++;
  }
  fclose(fp);
  if (line) free(line);
  
  course_->subjects = subjects;
  course_->n_subjects = n_lines;
  return course_;
}

char* getAllSubjects(course *course_) {
  char *ret;
  int len = 1;
  for (int i = 0; i < course_->n_subjects; i++) {
    len += strlen(course_->subjects[i]->code);
    len += strlen(course_->subjects[i]->title) + 4;
  }
  ret = (char *)(malloc(len * sizeof(char)));
  ret[0] = '\0';
  for (int i = 0; i < course_->n_subjects; i++) {
    strcat(ret, course_->subjects[i]->code);
    strcat(ret, " - ");
    strcat(ret, course_->subjects[i]->title);
    strcat(ret, "\n");
  }
  return ret;
}

char* getProgramByCode(course *course_, char *given_code) {
  char *ret;
  for (int i = 0; i < course_->n_subjects; i++) {
    if (strcmp(given_code, course_->subjects[i]->code) == 0) {
      ret = (char *)(malloc((strlen(course_->subjects[i]->program) + 1) * sizeof(char)));
      strcpy(ret, course_->subjects[i]->program);
      return ret;
    }
  }
  ret = (char *)(malloc(80 * sizeof(char)));
  sprintf(ret, "Materia %s nao existe.\n", given_code);
  return ret;
}

char* getAllInfoByCode(course *course_, char *given_code) {
  char *ret;
  for (int i = 0; i < course_->n_subjects; i++) {
    if (strcmp(given_code, course_->subjects[i]->code) == 0) {
      int len = strlen(course_->subjects[i]->code) + strlen(course_->subjects[i]->title) +
                strlen(course_->subjects[i]->program) + strlen(course_->subjects[i]->classroom) +
                strlen(course_->subjects[i]->class_time) + 5;
      ret = (char *)(malloc(len * sizeof(char)));
      ret[0] = '\0';
      strcat(ret, course_->subjects[i]->code);
      strcat(ret, ";");
      strcat(ret, course_->subjects[i]->title);
      strcat(ret, ";");
      strcat(ret, course_->subjects[i]->program);
      strcat(ret, ";");
      strcat(ret, course_->subjects[i]->classroom);
      strcat(ret, ";");
      strcat(ret, course_->subjects[i]->class_time);
      return ret;
    }
  }
  ret = (char *)(malloc(80 * sizeof(char)));
  sprintf(ret, "Materia %s nao existe.\n", given_code);
  return ret;
}

char* getEveryInfo(course *course_) {
  char *ret;
  int len = 1;
  for (int i = 0; i < course_->n_subjects; i++) {
    char *partial = getAllInfoByCode(course_, course_->subjects[i]->code);
    len += strlen(partial);
  }
  ret = (char *)(malloc(len * sizeof(char)));
  ret[0] = '\0';
  for (int i = 0; i < course_->n_subjects; i++) {
    strcat(ret, getAllInfoByCode(course_, course_->subjects[i]->code));
    strcat(ret, "\n");
  }
  return ret;
}

void writeComment(course *course_, char *given_code, char* comment) {
  for (int i = 0; i < course_->n_subjects; i++) {
    if (strcmp(given_code, course_->subjects[i]->code) == 0) {
      course_->subjects[i]->comment = (char *)(malloc((strlen(comment) + 1) * sizeof(char)));
      strcpy(course_->subjects[i]->comment, comment);
      break;
    }
  }
}

char* getCommentbyCode(course *course_, char *given_code) {
  char *ret;
  for (int i = 0; i < course_->n_subjects; i++) {
    if (strcmp(given_code, course_->subjects[i]->code) == 0) {
      if (course_->subjects[i]->comment) {
        ret = (char *)(malloc((strlen(course_->subjects[i]->comment) + 1) * sizeof(char)));
        strcpy(ret, course_->subjects[i]->comment);
      } else {
        ret = (char *)(malloc(5 * sizeof(char)));
        strcpy(ret, "NULL");
      }
      return ret;
    }
  }
  ret = (char *)(malloc(80 * sizeof(char)));
  sprintf(ret, "Materia %s nao existe.\n", given_code);
  return ret;
}
