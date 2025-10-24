#ifndef STRUCT_H
#define STRUCT_H

#include <stddef.h>

typedef struct {
    double *values;
    size_t size;
    size_t capacity;
} Grades;

typedef struct {
    char *name;
    double coef;
    Grades grades;
    double average;
} Course;

typedef struct {
    int id;
    char *fname;
    char *lname;
    int age;
    Course *courses;
    size_t num_courses;
    size_t courses_capacity;
    double average;
} Student;

typedef struct {
    Student *students;
    size_t num_students;
    size_t students_capacity;
} Prom;

/* --------------- Constructeurs / Destructeurs --------------- */
Grades *grades_create(void);
void grades_destroy(Grades *g);
Course *course_create(const char *name, double coef);
void course_destroy(Course *c);
Student *student_create(int id, const char *fname, const char *lname, int age);
void student_destroy(Student *s);
Prom *prom_create(void);
void prom_destroy(Prom *p);

/* --------------- Fonctions utilitaires --------------- */
int grades_add(Grades *g, double value);
void course_update_average(Course *c);
int course_add_grade(Course *c, double value);
int student_add_course(Student *s, Course *c);
void student_update_average(Student *s);
int prom_add_student(Prom *p, Student *s);
int safe_strdup(char **dest, const char *src);

/* --------------- Lecture et écriture fichiers --------------- */
Prom *prom_from_file(const char *filename);
int prom_save_binary(const char *filename, const Prom *p);
Prom *prom_load_binary(const char *filename);

#endif
