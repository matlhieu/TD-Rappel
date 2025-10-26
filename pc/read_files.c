#include "read_files.h"
#include "create.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void trim(char *s) {
    char *p = s;
    while (*p && (*p == ' ' || *p == '\n' || *p == '\r')) p++;
    memmove(s, p, strlen(p) + 1);
}

Prom *load_promotion_from_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;

    Prom *promo = prom_create();
    char line[256];
    enum { NONE, STUDENTS, COURSES, NOTES } section = NONE;

    while (fgets(line, sizeof(line), f)) {
        trim(line);
        if (strlen(line) == 0) continue;

        if (strcmp(line, "ETUDIANTS") == 0) { section = STUDENTS; continue; }
        if (strcmp(line, "MATIERES") == 0)  { section = COURSES; continue; }
        if (strcmp(line, "NOTES") == 0)     { section = NOTES; continue; }

        if (section == STUDENTS) {
            int id, age;
            char fname[50], lname[50];
            sscanf(line, "%d;%49[^;];%49[^;];%d", &id, fname, lname, &age);

            Student *s = student_create(id, fname, lname, age);
            promo->students = realloc(promo->students, (promo->numStudent + 1) * sizeof(Student));
            promo->students[promo->numStudent++] = *s;
            free(s);
        }
    }

    fclose(f);
    return promo;
}
