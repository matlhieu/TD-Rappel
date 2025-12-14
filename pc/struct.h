#ifndef STRUCT_H
#define STRUCT_H
#include <stdlib.h>

//Pointeurs de fct
#define ALPHA_FIRST_NAME 1
#define ALPHA_LAST_NAME 2
#define AVERAGE 3
#define MINIMUM 4

#define SIZE_TOP1 10
//struct

typedef struct{
    double *values;
    size_t size;
    size_t capacity;
}Grades;

typedef struct{
    char *name;
    double coef;
    Grades grades;
    float passmark; 
}Course;

/*
typedef enum{
    Mathematiques = 0,
    EPS,
    Physique,
    Biologie,
    Latin,
    Musique,
    Economie,
    Philosophie,
    Histoire,
    Geographie,
    Français,
    Espagnol,
    Biologie,
    Informatique,
    Anglais,
    Sociologie,
    Arts Plastiques,
    Sciences Sociales,

} CourseIndex;*/

typedef struct{
    int id;
    char *fname;
    char *lname;
    int age;
    Course* courses;
    size_t num_course;
    size_t capacity;
    float average;

}Student;

typedef struct{
    Student *students;
    int numStudent;
    int (*current_compare_func)(const void *, const void *);
}Prom;

//functions destroyyy


void grades_destroy(Grades *g);
void course_destroy(Course *c);
void student_destroy(Student *s);
void prom_destroy(Prom *p);


#endif