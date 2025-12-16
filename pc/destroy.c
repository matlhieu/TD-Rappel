#include "struct.h"
#include <stdlib.h>
#include <string.h>

void grades_destroy(Grades *g) {
    if (!g) return;
    free(g->values);
    g->values = NULL;
    g->size = 0;
    g->capacity = 0;
}

void course_destroy(Course *c) {
    if (!c) return;
    free(c->name);
    grades_destroy(&c->grades);
    // ne pas free(c) ici : c peut être un élément d'un tableau
}

void student_destroy(Student *s) {
    if (!s) return;

    free(s->fname);
    free(s->lname);

    for (size_t i = 0; i < s->num_course; ++i) {
        course_destroy(&s->courses[i]);
    }
    free(s->courses);

    // NE PAS free(s) si s est un élément d'un tableau. prom_destroy free() le bloc.
}

void prom_destroy(Prom *p) {
    if (!p) return;

    for (int i = 0; i < p->numStudent; ++i) {
        student_destroy(&p->students[i]);
    }
    free(p->students);
    free(p);
}
