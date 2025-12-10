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
    if (!promo) { fclose(f); return NULL; }

    char line[256];
    enum { NONE, STUDENTS, COURSES, NOTES } section = NONE;

    Course *courses_ref = NULL;
    size_t nb_courses = 0;

    int header_students = 0;
    int header_courses = 0;
    int header_notes = 0;

    while (fgets(line, sizeof(line), f)) {
        trim(line);
        if (strlen(line) == 0) continue;

        if (strcmp(line, "ETUDIANTS") == 0) { section = STUDENTS; header_students = 0; continue; }
        if (strcmp(line, "MATIERES") == 0)  { section = COURSES;  header_courses = 0;  continue; }
        if (strcmp(line, "NOTES") == 0)     { section = NOTES;    header_notes = 0;    continue; }

        if (section == STUDENTS) {
            if (!header_students) { header_students = 1; continue; } // skip header
            int id, age;
            char fname[50], lname[50];
            if (sscanf(line, "%d;%49[^;];%49[^;];%d", &id, fname, lname, &age) == 4) {
                Student *s = student_create(id, fname, lname, age);
                if (!s) continue; // allocation failed

                Student *tmp = realloc(promo->students, (promo->numStudent + 1) * sizeof(Student));
                if (!tmp) {
                    // allocation failed: clean up and abort load
                    free(s->fname); free(s->lname); free(s);
                    // free courses_ref memory properly
                    for (size_t k = 0; k < nb_courses; ++k) free(courses_ref[k].name);
                    free(courses_ref);
                    prom_destroy(promo);
                    fclose(f);
                    return NULL;
                }
                promo->students = tmp;

                // copy struct content (shallow copy of fname/lname pointers is intended)
                promo->students[promo->numStudent] = *s; 
                promo->numStudent++;

                // we can free the temporary Student struct (it doesn't free fname/lname)
                free(s);
            }
        }

        else if (section == COURSES) {
            if (!header_courses) { header_courses = 1; continue; }
            char name[50]; double coef;
            if (sscanf(line, "%49[^;];%lf", name, &coef) == 2) {
                Course *ctmp = course_create(name, coef);
                if (!ctmp) continue;

                Course *tmp = realloc(courses_ref, (nb_courses + 1) * sizeof(Course));
                if (!tmp) {
                    // cleanup on failure
                    course_destroy(ctmp);
                    free(ctmp);
                    for (size_t k = 0; k < nb_courses; ++k) free(courses_ref[k].name);
                    free(courses_ref);
                    prom_destroy(promo);
                    fclose(f);
                    return NULL;
                }
                courses_ref = tmp;

                // copy the Course struct (copies the pointer to name, grades, etc.)
                courses_ref[nb_courses] = *ctmp;
                nb_courses++;

                // free the temporary Course struct (not its fields)
                free(ctmp);
            }
        }

        else if (section == NOTES) {
            if (!header_notes) { header_notes = 1; continue; }
            int id; char cname[50]; double grade;
            if (sscanf(line, "%d;%49[^;];%lf", &id, cname, &grade) == 3) {
                Student *s = find_student_by_id(promo, id);
                if (!s) continue;

                Course *c = find_course_in_student(s, cname);
                if (!c) {
                    for (size_t i = 0; i < nb_courses; i++) {
                        if (strcmp(courses_ref[i].name, cname) == 0) {
                            Course *tmp = realloc(s->courses, (s->num_course + 1) * sizeof(Course));
                            if (!tmp) {
                                // allocation failed: ignore this note (or handle gracefully)
                                c = NULL;
                                break;
                            }
                            s->courses = tmp;
                            s->courses[s->num_course] = courses_ref[i]; // copy course struct (shallow)
                            c = &s->courses[s->num_course];
                            s->num_course++;
                            break;
                        }
                    }
                }

                if (c) {
                    double *vals = realloc(c->grades.values, (c->grades.size + 1) * sizeof(double));
                    if (!vals) continue; // allocation failed
                    c->grades.values = vals;
                    c->grades.values[c->grades.size++] = grade;
                    student_update_average(s);
                }
            }
        }
    }

    // NOTE: we must NOT free the name pointers here because they are used in s->courses.
    // We only free the courses_ref array container.
    free(courses_ref);
    fclose(f);
    return promo;
}
