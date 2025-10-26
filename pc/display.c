#include "struct.h"
#include <stdio.h>
#include <stdlib.h>

void display_promotion(Prom *p) {
    for (int i = 0; i < p->numStudent; i++) {
        Student *s = p->students[i];
        printf("ID : %d | %s %s | %d ans\n", s->id, s->fname, s->lname, s->age);

        for (size_t j = 0; j < s->num_course; j++) {
            Course *c = &s->courses[j];
            printf("%s | coef %.2f : ", c->name, c->coef);

            double sum = 0;
            for (size_t k = 0; k < c->grades.size; k++) {
                printf("%.1f ", c->grades.values[k]);
                sum += c->grades.values[k];
            }

            double passmark = (c->grades.size > 0) ? sum / c->grades.size : 0.0;
            printf("| Average : %.2f\n", passmark);
        }

        printf("=> Overall average : %.2f\n\n", s->average);
    }
}
