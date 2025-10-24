#include "struct.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    Prom *p = prom_create();
    if (!p) {
        fprintf(stderr, "Allocation prom failed\n");
        return EXIT_FAILURE;
    }

    /* Exemple basique: créer un étudiant, une matière, ajouter une note, l'ajouter à la promo */
    Student *s = student_create(226345678, "Alexander", "Muller", 19);
    if (!s) return EXIT_FAILURE;

    Course *c = course_create("Mathematiques", 2.25);
    course_add_grade(c, 15.0);
    course_add_grade(c, 12.5);

    student_add_course(s, c);
    student_update_average(s);

    prom_add_student(p, s);

    /* Affichage basique */
    printf("Promotion has %zu student(s).\n", p->num_students);
    if (p->num_students > 0) {
        Student *st = &p->students[0];
        printf("Student %d: %s %s, age %d, average %.2f\n",
            st->id, st->fname ? st->fname : "(null)", st->lname ? st->lname : "(null)", st->age,
            isnan(st->average) ? 0.0 : st->average);
    }

    prom_destroy(p);
    return EXIT_SUCCESS;
}
