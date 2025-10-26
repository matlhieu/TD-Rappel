#include "struct.h"

int save_promotion_binary(const char *filename, Prom *p) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;
    fwrite(&p->numStudent, sizeof(int), 1, f);
    fwrite(p->students, sizeof(Student), p->numStudent, f);
    fclose(f);
    return 0;
}

Prom *load_promotion_binary(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return NULL;
    Prom *p = prom_create();
    fread(&p->numStudent, sizeof(int), 1, f);
    p->students = malloc(p->numStudent * sizeof(Student));
    fread(p->students, sizeof(Student), p->numStudent, f);
    fclose(f);
    return p;
}
