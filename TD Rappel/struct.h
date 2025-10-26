#ifdef STRUCT_H
#define STRUCT_H

//struct

typedef struct{
    double* values;
    size_t size;
    size_t capacity;
}Grades;

typedef struct{
    char* name;
    double coef;
    Grades grades;
    float passmark; 
}Course;

typedef struct{
    int id;
    char* fname;
    char* lname;
    int age;
    Course* courses;
    size_t num_course;
    size_t courses_capacity;
    float average;
}Student;

typedef struct{
    Student* students;
    int numStudent;
}Prom;

//functions

Grades *grades_create();
void grades_destroy(Grades *g);
int grades_add(Grades *g, double value); 

Course *course_create( char *name, double coef);
void course_destroy(Course *c);
int course_add_grade(Course *c, double value);


Student *student_create(int id,  char *fname,  char *lname, int age);
void student_destroy(Student *s);
int student_add_course(Student *s, Course *c); 
void student_update_average(Student *s);


Prom *prom_create();
void prom_destroy(Prom *p);
int prom_add_student(Prom *p, Student *s); 


#endif