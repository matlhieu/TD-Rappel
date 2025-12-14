#ifndef STRUCT_H
#define STRUCT_H
#include <stdlib.h>
#include <stdint.h> // Pour uint64_t

// --- EXERCICE BITWISE : Les matieres ---
// Enum simple pour les index (0 à 16)
typedef enum {
    MATHS = 0, PHYSIQUE, BIOLOGIE, INFORMATIQUE, ECONOMIE, // Sciences
    FRANCAIS, HISTOIRE, GEOGRAPHIE, PHILOSOPHIE, ANGLAIS, ESPAGNOL, LATIN, 
    MUSIQUE, ARTS_PLASTIQUES, SOCIOLOGIE, SCIENCES_SOCIALES, EPS
} CourseIndex;

// Masques pour les poles (1ULL << X permet de decaler le bit 1 de X cases)
#define SCIENCES_MASK ( (1ULL << MATHS) | (1ULL << PHYSIQUE) | (1ULL << BIOLOGIE) | (1ULL << INFORMATIQUE) | (1ULL << ECONOMIE) )
#define HUMANITIES_MASK ( (1ULL << FRANCAIS) | (1ULL << HISTOIRE) | (1ULL << GEOGRAPHIE) | (1ULL << PHILOSOPHIE) | (1ULL << ANGLAIS) | (1ULL << ESPAGNOL) | (1ULL << LATIN) | (1ULL << MUSIQUE) | (1ULL << ARTS_PLASTIQUES) | (1ULL << SOCIOLOGIE) | (1ULL << SCIENCES_SOCIALES) )
#define YEAR_MASK (SCIENCES_MASK | HUMANITIES_MASK)

// --- EXERCICE POINTEURS : Modes de tri ---
#define ALPHA_FIRST_NAME 1
#define ALPHA_LAST_NAME 2
#define AVERAGE 3
#define MINIMUM 4

#define SIZE_TOP1 10

typedef struct {
    double *values;
    size_t size;
    size_t capacity;
} Grades;

typedef struct {
    char *name;
    double coef;
    Grades grades;
    float passmark; 
} Course;

typedef struct {
    int id;
    char *fname;
    char *lname;
    int age;
    Course* courses;
    size_t num_course;
    size_t capacity;
    float average;

    // Ajout Exercice Bitwise : stockage des validations (64 bits)
    uint64_t validated_courses; 
} Student;

typedef struct {
    Student *students;
    int numStudent;
    
    // Ajout Exercice Pointeurs : variable stockant l'adresse de la fonction de tri
    int (*current_compare_func)(const void *, const void *);
} Prom;

// Fonctions destroy
void grades_destroy(Grades *g);
void course_destroy(Course *c);
void student_destroy(Student *s);
void prom_destroy(Prom *p);

#endif
