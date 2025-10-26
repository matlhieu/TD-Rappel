#include "struct.h"
#include <stdlib.h>
#include <string.h>

void grades_destroy(Grades *g) {
    if (g == NULL) {
        return;
    }
    free(g->values);
    g->values = NULL;
    g->size = 0;
    g->capacity = 0;
}

void course_destroy(Course *c) {
    if (c == NULL) {
        return;
    }
    free(c->name);
    grades_destroy(&c->grades);
}

void student_destroy(Student *s) {
    if (s == NULL) {
        return;
    }
    free(s->fname);
    free(s->lname);
    for (size_t i = 0; i < s->num_course; i++) {
        course_destroy(&s->courses[i]);
    }
    free(s->courses);
}

void prom_destroy(Prom *p) {
    if (p == NULL) {
        return NULL;
    }
    for (int i = 0; i < p->numStudent; i++) {
        student_destroy(&p->students[i]);
    }
    free(p->students);
}