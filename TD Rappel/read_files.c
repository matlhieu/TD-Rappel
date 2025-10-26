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

Prom *load_promotion_from_file(char *filename) {
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
            prom_add_student(promo, s);
        }
        else if (section == COURSES) {
            char name[50]; double coef;
            sscanf(line, "%49[^;];%lf", name, &coef);

        }
        else if (section == NOTES) {
            int id; char cname[50]; double grade;
            sscanf(line, "%d;%49[^;];%lf", &id, cname, &grade);

        }
    }
    fclose(f);
    return promo;
}
