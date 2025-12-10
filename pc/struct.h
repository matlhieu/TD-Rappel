#ifndef STRUCT_H
#define STRUCT_H
#include <stdlib.h>

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
}Prom;

//functions destroyyy


void grades_destroy(Grades *g);
void course_destroy(Course *c);
void student_destroy(Student *s);
void prom_destroy(Prom *p);



#endif