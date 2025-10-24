#include "struct.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/* -----------------------
   Utils
   ----------------------- */
int safe_strdup(char **dest, const char *src) {
    if (!src) { *dest = NULL; return 0; }
    *dest = malloc(strlen(src) + 1);
    if (!*dest) return -1;
    strcpy(*dest, src);
    return 0;
}

/* -----------------------
   Grades
   ----------------------- */
Grades *grades_create(void) {
    Grades *g = malloc(sizeof(Grades));
    if (!g) return NULL;
    g->values = NULL;
    g->size = 0;
    g->capacity = 0;
    return g;
}

void grades_destroy(Grades *g) {
    if (!g) return;
    free(g->values);
    free(g);
}

int grades_add(Grades *g, double value) {
    if (!g) return -1;
    if (g->size + 1 > g->capacity) {
        size_t newcap = (g->capacity == 0) ? 4 : g->capacity * 2;
        double *tmp = realloc(g->values, newcap * sizeof(double));
        if (!tmp) return -1;
        g->values = tmp;
        g->capacity = newcap;
    }
    g->values[g->size++] = value;
    return 0;
}

/* -----------------------
   Course
   ----------------------- */
Course *course_create(const char *name, double coef) {
    Course *c = malloc(sizeof(Course));
    if (!c) return NULL;
    safe_strdup(&c->name, name);
    c->coef = coef;
    c->grades.values = NULL;
    c->grades.size = 0;
    c->grades.capacity = 0;
    c->average = NAN;
    return c;
}

void course_destroy(Course *c) {
    if (!c) return;
    free(c->name);
    free(c->grades.values);
    free(c);
}

int course_add_grade(Course *c, double value) {
    if (!c) return -1;
    grades_add(&c->grades, value);
    course_update_average(c);
    return 0;
}

void course_update_average(Course *c) {
    if (!c) return;
    if (c->grades.size == 0) { c->average = NAN; return; }
    double sum = 0;
    for (size_t i = 0; i < c->grades.size; i++) sum += c->grades.values[i];
    c->average = sum / c->grades.size;
}

/* -----------------------
   Student
   ----------------------- */
Student *student_create(int id, const char *fname, const char *lname, int age) {
    Student *s = malloc(sizeof(Student));
    if (!s) return NULL;
    s->id = id;
    s->age = age;
    safe_strdup(&s->fname, fname);
    safe_strdup(&s->lname, lname);
    s->courses = NULL;
    s->num_courses = 0;
    s->courses_capacity = 0;
    s->average = NAN;
    return s;
}

void student_destroy(Student *s) {
    if (!s) return;
    free(s->fname);
    free(s->lname);
    for (size_t i = 0; i < s->num_courses; ++i) {
        free(s->courses[i].name);
        free(s->courses[i].grades.values);
    }
    free(s->courses);
    free(s);
}

int student_add_course(Student *s, Course *c) {
    if (!s || !c) return -1;
    if (s->num_courses + 1 > s->courses_capacity) {
        size_t newcap = (s->courses_capacity == 0) ? 4 : s->courses_capacity * 2;
        Course *tmp = realloc(s->courses, newcap * sizeof(Course));
        if (!tmp) return -1;
        s->courses = tmp;
        s->courses_capacity = newcap;
    }
    Course *dest = &s->courses[s->num_courses++];
    safe_strdup(&dest->name, c->name);
    dest->coef = c->coef;
    dest->average = c->average;
    dest->grades.size = c->grades.size;
    dest->grades.capacity = c->grades.size;
    if (c->grades.size > 0) {
        dest->grades.values = malloc(c->grades.size * sizeof(double));
        memcpy(dest->grades.values, c->grades.values, c->grades.size * sizeof(double));
    } else dest->grades.values = NULL;
    return 0;
}

void student_update_average(Student *s) {
    if (!s) return;
    double sum = 0, coef_sum = 0;
    for (size_t i = 0; i < s->num_courses; i++) {
        Course *c = &s->courses[i];
        if (!isnan(c->average)) {
            sum += c->average * c->coef;
            coef_sum += c->coef;
        }
    }
    s->average = (coef_sum == 0) ? NAN : sum / coef_sum;
}

/* -----------------------
   Prom
   ----------------------- */
Prom *prom_create(void) {
    Prom *p = malloc(sizeof(Prom));
    if (!p) return NULL;
    p->students = NULL;
    p->num_students = 0;
    p->students_capacity = 0;
    return p;
}

void prom_destroy(Prom *p) {
    if (!p) return;
    for (size_t i = 0; i < p->num_students; i++) {
        student_destroy(&p->students[i]);
    }
    free(p->students);
    free(p);
}

int prom_add_student(Prom *p, Student *s) {
    if (!p || !s) return -1;
    if (p->num_students + 1 > p->students_capacity) {
        size_t newcap = (p->students_capacity == 0) ? 4 : p->students_capacity * 2;
        Student *tmp = realloc(p->students, newcap * sizeof(Student));
        if (!tmp) return -1;
        p->students = tmp;
        p->students_capacity = newcap;
    }
    Student *dest = &p->students[p->num_students++];
    dest->id = s->id;
    dest->age = s->age;
    safe_strdup(&dest->fname, s->fname);
    safe_strdup(&dest->lname, s->lname);
    dest->courses = NULL;
    dest->num_courses = 0;
    dest->courses_capacity = 0;
    dest->average = s->average;
    for (size_t i = 0; i < s->num_courses; i++) {
        Course *tmpc = course_create(s->courses[i].name, s->courses[i].coef);
        for (size_t j = 0; j < s->courses[i].grades.size; j++)
            course_add_grade(tmpc, s->courses[i].grades.values[j]);
        student_add_course(dest, tmpc);
        course_destroy(tmpc);
    }
    return 0;
}

/* -----------------------
   Question 2 – Lecture fichier texte
   ----------------------- */
Prom *prom_from_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror("open file"); return NULL; }
    Prom *p = prom_create();
    char section[32];
    while (fscanf(f, "%31s", section) == 1) {
        if (strcmp(section, "ETUDIANTS") == 0) {
            int id, age;
            char nom[64], prenom[64];
            while (fscanf(f, "%d %s %s %d", &id, prenom, nom, &age) == 4) {
                Student *s = student_create(id, prenom, nom, age);
                prom_add_student(p, s);
                student_destroy(s);
            }
        } else if (strcmp(section, "MATIERES") == 0) {
            /* ignore: handled inside NOTES section */
            char name[64]; double coef;
            while (fscanf(f, "%s %lf", name, &coef) == 2) { }
        } else if (strcmp(section, "NOTES") == 0) {
            int id; char mat[64]; double note;
            while (fscanf(f, "%d %s %lf", &id, mat, &note) == 3) {
                for (size_t i = 0; i < p->num_students; i++) {
                    Student *s = &p->students[i];
                    if (s->id == id) {
                        size_t j;
                        for (j = 0; j < s->num_courses; j++)
                            if (strcmp(s->courses[j].name, mat) == 0) break;
                        if (j == s->num_courses) {
                            Course *c = course_create(mat, 1.0);
                            student_add_course(s, c);
                            course_destroy(c);
                        }
                        course_add_grade(&s->courses[j], note);
                        course_update_average(&s->courses[j]);
                        student_update_average(s);
                        break;
                    }
                }
            }
        }
    }
    fclose(f);
    return p;
}

/* -----------------------
   Question 4 – Sauvegarde binaire
   ----------------------- */
int prom_save_binary(const char *filename, const Prom *p) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;
    fwrite(&p->num_students, sizeof(size_t), 1, f);
    for (size_t i = 0; i < p->num_students; i++) {
        Student *s = &p->students[i];
        fwrite(&s->id, sizeof(int), 1, f);
        fwrite(&s->age, sizeof(int), 1, f);
        size_t len;
        len = strlen(s->fname) + 1; fwrite(&len, sizeof(size_t), 1, f); fwrite(s->fname, 1, len, f);
        len = strlen(s->lname) + 1; fwrite(&len, sizeof(size_t), 1, f); fwrite(s->lname, 1, len, f);
        fwrite(&s->num_courses, sizeof(size_t), 1, f);
        for (size_t j = 0; j < s->num_courses; j++) {
            Course *c = &s->courses[j];
            len = strlen(c->name) + 1; fwrite(&len, sizeof(size_t), 1, f); fwrite(c->name, 1, len, f);
            fwrite(&c->coef, sizeof(double), 1, f);
            fwrite(&c->grades.size, sizeof(size_t), 1, f);
            fwrite(c->grades.values, sizeof(double), c->grades.size, f);
        }
    }
    fclose(f);
    return 0;
}

Prom *prom_load_binary(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return NULL;
    Prom *p = prom_create();
    fread(&p->num_students, sizeof(size_t), 1, f);
    p->students = malloc(p->num_students * sizeof(Student));
    for (size_t i = 0; i < p->num_students; i++) {
        Student *s = &p->students[i];
        fread(&s->id, sizeof(int), 1, f);
        fread(&s->age, sizeof(int), 1, f);
        size_t len;
        fread(&len, sizeof(size_t), 1, f); s->fname = malloc(len); fread(s->fname, 1, len, f);
        fread(&len, sizeof(size_t), 1, f); s->lname = malloc(len); fread(s->lname, 1, len, f);
        fread(&s->num_courses, sizeof(size_t), 1, f);
        s->courses = malloc(s->num_courses * sizeof(Course));
        for (size_t j = 0; j < s->num_courses; j++) {
            Course *c = &s->courses[j];
            fread(&len, sizeof(size_t), 1, f); c->name = malloc(len); fread(c->name, 1, len, f);
            fread(&c->coef, sizeof(double), 1, f);
            fread(&c->grades.size, sizeof(size_t), 1, f);
            c->grades.capacity = c->grades.size;
            c->grades.values = malloc(c->grades.size * sizeof(double));
            fread(c->grades.values, sizeof(double), c->grades.size, f);
            course_update_average(c);
        }
        student_update_average(s);
    }
    fclose(f);
    return p;
}
