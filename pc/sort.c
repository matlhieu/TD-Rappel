#include "sort.h"
#include <stdlib.h>

int compare_students(const void *a, const void *b) {
    Student *s1 = (Student*)a;
    Student *s2 = (Student*)b;
return (s2->average > s1->average) - (s2->average < s1->average);
}

Student *get_top_students(Prom *p, int n) {
    if (n > p->numStudent){
        n = p->numStudent;
    } 
    Student *copy = malloc(p->numStudent * sizeof(Student));
    memcpy(copy, p->students, p->numStudent * sizeof(Student));
    qsort(copy, p->numStudent, sizeof(Student), compare_students);
    return copy;
}