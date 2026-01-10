/**
* @file sort.h
* @brief Fct de tri et algorithmes de classments
*/
#ifndef SORT_H
#define SORT_H
#include "struct.h"

/** @brief fct de comparaison */
int compare_alpha_first_name(const void *a, const void *b);
int compare_alpha_last_name(const void *a, const void *b);
int compare_average(const void *a, const void *b);
int compare_minimum(const void *a, const void *b);

int API_set_sorting_mode(Prom* pClass, int mode);
char** API_sort_students(Prom* pClass);

int compare_students(const void *a, const void *b); 
Student *get_top_students(Prom *p, int n);
void get_top3_in_course(Prom *p, const char *course_name);

#endif