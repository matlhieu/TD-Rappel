/**
 * @file struct.h
 * @brief Définition des structures de données et des constantes pour la gestion de la promotion.
 * @author Mathieu Nguyen
 * @date 2025
 */

#ifndef STRUCT_H
#define STRUCT_H
#include <stdlib.h>
#include <stdint.h> 


/** @brief bits pour chaque matieres */ 
typedef enum {
    MATHS = 0, /**< Mathématiques - Bit 0 */
    PHYSIQUE,
    BIOLOGIE,
    INFORMATIQUE,
    ECONOMIE,   /** @brief Pôle Science */

    FRANCAIS,
    HISTOIRE,
    GEOGRAPHIE,
    PHILOSOPHIE,
    ANGLAIS,
    ESPAGNOL,
    LATIN,
    MUSIQUE,
    ARTS_PLASTIQUES,
    SOCIOLOGIE,
    SCIENCES_SOCIALES,
    EPS,                /** @brief Pôle Humanité */
    
    COURSE_INDEX_MAX
} CourseIndex;

/** @brief Masques binaires pour les Pôles */ 
#define SCIENCES_MASK ( (1ULL << MATHS) | (1ULL << PHYSIQUE) | (1ULL << BIOLOGIE) | (1ULL << INFORMATIQUE) | (1ULL << ECONOMIE) )
#define HUMANITIES_MASK ( (1ULL << FRANCAIS) | (1ULL << HISTOIRE) | (1ULL << GEOGRAPHIE) | (1ULL << PHILOSOPHIE) | (1ULL << ANGLAIS) | (1ULL << ESPAGNOL) | (1ULL << LATIN) | (1ULL << MUSIQUE) | (1ULL << ARTS_PLASTIQUES) | (1ULL << SOCIOLOGIE) | (1ULL << SCIENCES_SOCIALES) )
#define MATHS_MASK (1ULL << MATHS)
#define YEAR_MASK (SCIENCES_MASK | HUMANITIES_MASK)

#define ALPHA_FIRST_NAME 1
#define ALPHA_LAST_NAME 2
#define AVERAGE 3
#define MINIMUM 4

#define SIZE_TOP1 10

typedef struct{
    double *values;
    size_t size;
    size_t capacity;
} Grades;

typedef struct{
    char *name;
    double coef;
    Grades grades;
    float passmark; 
} Course;

typedef struct{
    int id;
    char *fname;
    char *lname;
    int age;
    Course* courses;
    size_t num_course;
    size_t capacity;
    float average;

    uint64_t validated_courses; 

} Student;

typedef struct{
    Student *students;
    int numStudent;
    int (*current_compare_func)(const void *, const void *);
} Prom;

void grades_destroy(Grades *g);
void course_destroy(Course *c);
void student_destroy(Student *s);
void prom_destroy(Prom *p);

#endif