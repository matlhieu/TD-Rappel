#ifndef STRUCT_H
#define STRUCT_H

#include <stddef.h>

/**
 * @file struct.h
 * @brief Structures pour l'exercice - définitions et prototypes
 */

/* -----------------------
   Structures
   ----------------------- */

/**
 * @brief Table de notes dynamiques
 */
typedef struct {
    double *values;   /**< tableau de notes */
    size_t size;      /**< nombre d'éléments réellement présents */
    size_t capacity;  /**< capacité allouée (internelle) */
} Grades;

/**
 * @brief Matière (cours) contenant ses notes
 */
typedef struct {
    char *name;    /**< nom (allocated as needed) */
    double coef;   /**< coefficient de la matière */
    Grades grades; /**< notes pour la matière */
    double average;/**< moyenne de la matière (NaN ou 0 s'il n'y a pas de note) */
} Course;

/**
 * @brief Étudiant
 */
typedef struct {
    int id;
    char *fname;    /**< prénom (malloc'd) */
    char *lname;    /**< nom (malloc'd) */
    int age;
    Course *courses;/**< tableau dynamique de matières */
    size_t num_courses;
    size_t courses_capacity;
    double average; /**< moyenne générale */
} Student;

/**
 * @brief Promotion (ensemble d'étudiants)
 */
typedef struct {
    Student *students;
    size_t num_students;
    size_t students_capacity;
} Prom;

/* -----------------------
   Prototypes - Grades
   ----------------------- */

Grades *grades_create(void);
void grades_destroy(Grades *g);
int grades_add(Grades *g, double value); /* retourne 0 si ok, -1 erreur */

/* -----------------------
   Prototypes - Course
   ----------------------- */

Course *course_create(const char *name, double coef);
void course_destroy(Course *c);
int course_add_grade(Course *c, double value);
void course_update_average(Course *c);

/* -----------------------
   Prototypes - Student
   ----------------------- */

Student *student_create(int id, const char *fname, const char *lname, int age);
void student_destroy(Student *s);
int student_add_course(Student *s, Course *c); /* prend ownership du Course* */
void student_update_average(Student *s);

/* -----------------------
   Prototypes - Prom (Class)
   ----------------------- */

Prom *prom_create(void);
void prom_destroy(Prom *p);
int prom_add_student(Prom *p, Student *s); /* prend ownership du Student* */

/* -----------------------
   Utilitaires
   ----------------------- */

/* retourne 0 si ok, -1 sinon */
int safe_strdup(char **dest, const char *src);

#endif /* STRUCT_H */
