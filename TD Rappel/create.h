#ifdef CREATE_H
#define CREATE_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

Grades* grades_create();
Course *course_create(char *name, double *coef);
Student *student_create(int id, char *fname, char *lname, int age);
Prom *prom_create();
#endif