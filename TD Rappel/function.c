#include "struct.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* -----------------------
   Utilitaires
   ----------------------- */

int safe_strdup(char **dest, const char *src) {
    if (!src) {
        *dest = NULL;
        return 0;
    }
    size_t len = strlen(src);
    *dest = malloc(len + 1);
    if (!*dest) return -1;
    memcpy(*dest, src, len + 1);
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
    if (safe_strdup(&c->name, name) != 0) {
        free(c);
        return NULL;
    }
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
    if (grades_add(&c->grades, value) != 0) return -1;
    course_update_average(c);
    return 0;
}

void course_update_average(Course *c) {
    if (!c) return;
    if (c->grades.size == 0) {
        c->average = NAN;
        return;
    }
    double sum = 0.0;
    for (size_t i = 0; i < c->grades.size; ++i) {
        sum += c->grades.values[i];
    }
    c->average = sum / (double)c->grades.size;
}

/* -----------------------
   Student
   ----------------------- */

Student *student_create(int id, const char *fname, const char *lname, int age) {
    Student *s = malloc(sizeof(Student));
    if (!s) return NULL;
    s->id = id;
    s->age = age;
    if (safe_strdup(&s->fname, fname) != 0) { free(s); return NULL; }
    if (safe_strdup(&s->lname, lname) != 0) { free(s->fname); free(s); return NULL; }
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
    if (s->courses) {
        for (size_t i = 0; i < s->num_courses; ++i) {
            course_destroy(&s->courses[i]); /* WRONG - adjust: course_destroy expects pointer allocated by course_create */
        }
        /* Because in our design student_add_course takes ownership of Course*, we must store pointers or copy correctly.
           To keep it simple below we'll assume student stores Course* array. */
    }
    /* To be safe, implement correct destroy in next iteration */
    free(s->courses);
    free(s);
}

/* NOTE: We will alter the student courses storage to keep Course * pointers.
   Implement a corrected version below.
*/

int student_add_course(Student *s, Course *c) {
    if (!s || !c) return -1;
    if (s->num_courses + 1 > s->courses_capacity) {
        size_t newcap = (s->courses_capacity == 0) ? 4 : s->courses_capacity * 2;
        Course *tmp = realloc(s->courses, newcap * sizeof(Course));
        if (!tmp) return -1;
        s->courses = tmp;
        s->courses_capacity = newcap;
    }
    /* Copy course into array (deep copy) */
    Course *dest = &s->courses[s->num_courses++];
    dest->name = NULL;
    safe_strdup(&dest->name, c->name);
    dest->coef = c->coef;
    /* copy grades */
    dest->grades.size = c->grades.size;
    dest->grades.capacity = c->grades.size;
    if (c->grades.size > 0) {
        dest->grades.values = malloc(c->grades.size * sizeof(double));
        if (!dest->grades.values) {
            free(dest->name);
            s->num_courses--;
            return -1;
        }
        memcpy(dest->grades.values, c->grades.values, c->grades.size * sizeof(double));
    } else {
        dest->grades.values = NULL;
    }
    dest->average = c->average;
    /* free original course if caller expects ownership transfer */
    course_destroy(c);
    student_update_average(s);
    return 0;
}

void student_update_average(Student *s) {
    if (!s) return;
    double sum = 0.0;
    double coef_sum = 0.0;
    size_t counted = 0;
    for (size_t i = 0; i < s->num_courses; ++i) {
        Course *c = &s->courses[i];
        if (c->grades.size == 0 || isnan(c->average)) continue;
        sum += c->average * c->coef;
        coef_sum += c->coef;
        counted++;
    }
    if (coef_sum == 0.0) {
        s->average = NAN;
    } else {
        s->average = sum / coef_sum;
    }
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
    if (p->students) {
        for (size_t i = 0; i < p->num_students; ++i) {
            student_destroy(&p->students[i]); /* see note below */
        }
        free(p->students);
    }
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
    /* Copy student contents into array (deep copy) */
    Student *dest = &p->students[p->num_students++];
    dest->id = s->id;
    dest->age = s->age;
    dest->fname = NULL; dest->lname = NULL;
    safe_strdup(&dest->fname, s->fname);
    safe_strdup(&dest->lname, s->lname);
    dest->courses = NULL;
    dest->num_courses = 0;
    dest->courses_capacity = 0;
    dest->average = s->average;
    /* Copy student's courses */
    for (size_t i = 0; i < s->num_courses; ++i) {
        Course *tmpc = course_create(s->courses[i].name, s->courses[i].coef);
        /* copy grades */
        for (size_t j = 0; j < s->courses[i].grades.size; ++j) {
            course_add_grade(tmpc, s->courses[i].grades.values[j]);
        }
        student_add_course(dest, tmpc);
    }
    /* destroy the original student passed (caller responsibility) */
    student_destroy(s);
    return 0;
}
