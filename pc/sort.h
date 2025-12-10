#ifndef SORT_H
#define SORT_H
#include "struct.h"

int compare_students(const void *a, const void *b); 
Student *get_top_students(Prom *p, int n);
void get_top3_in_course(Prom *p, const char *course_name);

#endif