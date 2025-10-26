#ifdef STRUCT_H
#define STRUCT_H
#include <stlidb.h>

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

typedef struct{
    int id;
    char *fname;
    char *lname;
    int age;
    Course* courses;
    size_t num_course;
    size_t courses_capacity;
    float average;
}Student;

typedef struct{
    Student *students;
    int numStudent;
}Prom;

//functions


void grades_destroy(Grades *g);
int grades_add(Grades *g, double value); 

void course_destroy(Course *c);
int course_add_grade(Course *c, double value);

void student_destroy(Student *s);
int student_add_course(Student *s, Course *c); 
void student_update_average(Student *s);

void prom_destroy(Prom *p);
int prom_add_student(Prom *p, Student *s); 


#endif