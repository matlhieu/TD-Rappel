#ifndef SORT_H
#define SORT_H
#include "struct.h"

int compare_students(const void *a, const void *b); 
Student *get_top_students(Prom *p, int n);

#endif