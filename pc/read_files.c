#include "read_files.h"
#include "create.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------------------------------------------------------
 * Fonctions utilitaires
 * ------------------------------------------------------------------------- */

static void trim(char *s) {
    // Supprime BOM (UTF-8) + espaces / retours à la ligne
    unsigned char *p = (unsigned char *)s;

    // saute le BOM si présent (0xEF 0xBB 0xBF)
    if (p[0] == 0xEF && p[1] == 0xBB && p[2] == 0xBF) {
        p += 3;
    }

    // saute espaces, tabulations et retours à la ligne
    while (*p && (*p == ' ' || *p == '\n' || *p == '\r' || *p == '\t')) {
        p++;
    }

    // déplace le reste de la chaîne au début
    memmove(s, p, strlen((char *)p) + 1);

    // retire les espaces de fin
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\n' || s[len - 1] == '\r' || s[len - 1] == '\t')) {
        s[--len] = '\0';
    }
}

static Student *find_student_by_id(Prom *p, int id) {
    for (int i = 0; i < p->numStudent; i++)
        if (p->students[i].id == id)
            return &p->students[i];
    return NULL;
}

static Course *find_course_in_student(Student *s, const char *name) {
    for (size_t i = 0; i < s->num_course; i++)
        if (strcmp(s->courses[i].name, name) == 0)
            return &s->courses[i];
    return NULL;
}

/* ---------------------------------------------------------------------------
 * Calcul des moyennes
 * ------------------------------------------------------------------------- */

void student_update_average(Student *s) {
    if (!s || s->num_course == 0) return;

    double total = 0.0, coef_sum = 0.0;
    for (size_t i = 0; i < s->num_course; i++) {
        Course *c = &s->courses[i];
        double sum = 0.0;
        for (size_t j = 0; j < c->grades.size; j++)
            sum += c->grades.values[j];
        if (c->grades.size > 0)
            c->passmark = sum / c->grades.size;
        total += c->passmark * c->coef;
        coef_sum += c->coef;
    }
    if (coef_sum > 0)
        s->average = total / coef_sum;
}

/* ---------------------------------------------------------------------------
 * Lecture du fichier data.txt complet (étudiants, matières, notes)
 * ------------------------------------------------------------------------- */

Prom *load_promotion_from_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Erreur ouverture fichier");
        return NULL;
    }

    Prom *promo = prom_create();
    char line[256];
    enum { NONE, STUDENTS, COURSES, NOTES } section = NONE;

    Course *courses_ref = NULL;
    size_t nb_courses = 0;

    // drapeaux pour ignorer les lignes d’en-tête
    int header_students = 0;
    int header_courses = 0;
    int header_notes = 0;

    while (fgets(line, sizeof(line), f)) {
        trim(line);
        if (strlen(line) == 0) continue;

        if (strcmp(line, "ETUDIANTS") == 0) { 
            section = STUDENTS; 
            header_students = 0; 
            continue; 
        }
        if (strcmp(line, "MATIERES") == 0)  { 
            section = COURSES; 
            header_courses = 0; 
            continue; }
        if (strcmp(line, "NOTES") == 0)     { section = NOTES; header_notes = 0; continue; }

        if (section == STUDENTS) {
            if (!header_students) { header_students = 1; continue; } // ignore "numero;prenom;nom;age"
            int id, age;
            char fname[50], lname[50];
            if (sscanf(line, "%d;%49[^;];%49[^;];%d", &id, fname, lname, &age) == 4) {
                Student *s = student_create(id, fname, lname, age);
                promo->students = realloc(promo->students, (promo->numStudent + 1) * sizeof(Student));
                promo->students[promo->numStudent++] = *s;
                
            }
        }

        else if (section == COURSES) {
            if (!header_courses) { header_courses = 1; continue; } // ignore "nom;coef"
            char name[50]; double coef;
            if (sscanf(line, "%49[^;];%lf", name, &coef) == 2) {
                courses_ref = realloc(courses_ref, (nb_courses + 1) * sizeof(Course));
                courses_ref[nb_courses] = *course_create(name, coef);
                nb_courses++;
            }
        }

        else if (section == NOTES) {
            if (!header_notes) { header_notes = 1; continue; } // ignore "id;nom;note"
            int id; char cname[50]; double grade;
            if (sscanf(line, "%d;%49[^;];%lf", &id, cname, &grade) == 3) {
                Student *s = find_student_by_id(promo, id);
                if (!s) continue;

                Course *c = find_course_in_student(s, cname);
                if (!c) {
                    for (size_t i = 0; i < nb_courses; i++) {
                        if (strcmp(courses_ref[i].name, cname) == 0) {
                            s->courses = realloc(s->courses, (s->num_course + 1) * sizeof(Course));
                            s->courses[s->num_course] = courses_ref[i];
                            c = &s->courses[s->num_course];
                            s->num_course++;
                            break;
                        }
                    }
                }

                if (c) {
                    c->grades.values = realloc(c->grades.values, (c->grades.size + 1) * sizeof(double));
                    c->grades.values[c->grades.size++] = grade;
                    student_update_average(s);
                }
            }
        }
    }

    free(courses_ref);
    fclose(f);
    return promo;
}