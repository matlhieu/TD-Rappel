#include "struct.h"

#include "struct.h"
#include "create.h"
#include <stdio.h>

int save_promotion_binary(const char *filename, Prom *p) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Erreur fopen (écriture)");
        return -1;
    }

    fwrite(&p->numStudent, sizeof(int), 1, f);
    /*for (int i = 0; i < p->numStudent; i++) {
        Student *s = &p->students[i];

        fwrite(&s->id, sizeof(int), 1, f);
        fwrite(&s->age, sizeof(int), 1, f);

        int len_fname = strlen(s->fname) + 1;
        int len_lname = strlen(s->lname) + 1;

        fwrite(&len_fname, sizeof(int), 1, f);
        fwrite(s->fname, sizeof(char), len_fname, f);

        fwrite(&len_lname, sizeof(int), 1, f);
        fwrite(s->lname, sizeof(char), len_lname, f);

        fwrite(&s->num_course, sizeof(size_t), 1, f);
        for (size_t j = 0; j < s->num_course; j++) {
            Course *c = &s->courses[j];
            int len_name = strlen(c->name) + 1;

            fwrite(&len_name, sizeof(int), 1, f);
            fwrite(c->name, sizeof(char), len_name, f);
            fwrite(&c->coef, sizeof(double), 1, f);

            fwrite(&c->grades.size, sizeof(size_t), 1, f);
            fwrite(c->grades.values, sizeof(double), c->grades.size, f);
        }

        fwrite(&s->average, sizeof(float), 1, f);
    }*/

    fclose(f);
    return 0;

}

void load_promotion_binary(char* filename, Prom* p) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        perror("open binary");
        return;
    }

    if (fread(&p->numStudent, sizeof(int), 1, f) != 1) {
        perror("read size student binary");
        return;
    }
    /*p = malloc(sizeof(Prom) * p->numStudent);
    for (int i) {

    }*/

    printf("Size student binary: %d\n", p->numStudent);
    fclose(f);
}
