#include "sort.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int compare_students(const void *a, const void *b) {
    Student *s1 = (Student*)a;
    Student *s2 = (Student*)b;
return (s2->average > s1->average) - (s2->average < s1->average);
}

Student *get_top_students(Prom *p, int n) {
    if (n > p->numStudent){
        n = p->numStudent;
    } 
    Student *copy = malloc(p->numStudent * sizeof(Student));
    memcpy(copy, p->students, p->numStudent * sizeof(Student));
    qsort(copy, p->numStudent, sizeof(Student), compare_students);
    return copy;
}

typedef struct {
    Student *student;
    double average;
} StudentScore;

// renvoie la moyenne d'un étudiant dans une matière
static double get_course_average(Student *s, const char *course_name) {
    for (size_t i = 0; i < s->num_course; i++) {
        if (strcmp(s->courses[i].name, course_name) == 0) {
            Course *c = &s->courses[i];
            if (c->grades.size == 0) return -1;

            double sum = 0;
            for (size_t j = 0; j < c->grades.size; j++)
                sum += c->grades.values[j];

            return sum / c->grades.size;
        }
    }
    return -1;
}

static int compare_scores(const void *a, const void *b) {
    double x = ((StudentScore*)a)->average;
    double y = ((StudentScore*)b)->average;

    return (y > x) - (y < x);
}

// ----------- LA FONCTION PRINCIPALE -------------
void get_top3_in_course(Prom *p, const char *course_name) {
    if (!p || p->numStudent == 0) {
        printf("No student.\n");
        return;
    }

    StudentScore temp[p->numStudent];
    size_t count = 0;

    for (int i = 0; i < p->numStudent; i++) {
        double avg = get_course_average(&p->students[i], course_name);
        if (avg >= 0) {
            temp[count].student = &p->students[i];
            temp[count].average = avg;
            count++;
        }
    }

    if (count == 0) {
        printf("No student has the course '%s'.\n", course_name);
        return;
    }

    qsort(temp, count, sizeof(StudentScore), compare_scores);

    size_t top = (count < 3) ? count : 3;
    printf("\n=== Top %zu in %s ===\n", top, course_name);

    for (size_t i = 0; i < top; i++) {
        printf("%zu) %s %s — average : %.2f\n",
               i + 1,
               temp[i].student->fname,
               temp[i].student->lname,
               temp[i].average);
    }
}