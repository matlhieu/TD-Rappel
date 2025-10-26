#include "struct.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* -----------------------------------
   Fonctions utilitaires
   ----------------------------------- */

/**
 * @brief Duplique une chaîne (comme strdup mais portable)
 */
int safe_strdup(char **dest, const char *src) {
    if (src == NULL) {
        *dest = NULL;
        return 0;
    }
    *dest = malloc(strlen(src) + 1);
    if (*dest == NULL) {
        return -1;
    }
    strcpy(*dest, src);
    return 0;
}

/* -----------------------------------
   Grades
   ----------------------------------- */

/**
 * @brief Crée un tableau de notes vide
 */
Grades *grades_create(void) {
    Grades *g = malloc(sizeof(Grades));
    if (g == NULL) return NULL;
    g->values = NULL;
    g->size = 0;
    return g;
}

/**
 * @brief Ajoute une note dans le tableau
 */
int grades_add(Grades *g, double value) {
    if (g == NULL) return -1;

    double *tmp = realloc(g->values, (g->size + 1) * sizeof(double));
    if (tmp == NULL) return -1;

    g->values = tmp;
    g->values[g->size] = value;
    g->size += 1;

    return 0;
}

/**
 * @brief Détruit la structure Grades
 */
void grades_destroy(Grades *g) {
    if (g == NULL) return;
    free(g->values);
    free(g);
}

/* -----------------------------------
   Course
   ----------------------------------- */

/**
 * @brief Crée une matière
 */
Course *course_create(const char *name, double coef) {
    Course *c = malloc(sizeof(Course));
    if (c == NULL) return NULL;

    safe_strdup(&c->name, name);
    c->coef = coef;
    c->grades.values = NULL;
    c->grades.size = 0;
    c->average = 0.0;

    return c;
}

/**
 * @brief Ajoute une note à la matière
 */
int course_add_grade(Course *c, double value) {
    if (c == NULL) return -1;

    grades_add(&c->grades, value);

    // Met à jour la moyenne
    double sum = 0.0;
    for (size_t i = 0; i < c->grades.size; i++) {
        sum += c->grades.values[i];
    }
    c->average = sum / c->grades.size;

    return 0;
}

/**
 * @brief Détruit une matière
 */
void course_destroy(Course *c) {
    if (c == NULL) return;
    free(c->name);
    free(c->grades.values);
    free(c);
}

/* -----------------------------------
   Student
   ----------------------------------- */

/**
 * @brief Crée un étudiant
 */
Student *student_create(int id, const char *fname, const char *lname, int age) {
    Student *s = malloc(sizeof(Student));
    if (s == NULL) return NULL;

    s->id = id;
    s->age = age;
    safe_strdup(&s->fname, fname);
    safe_strdup(&s->lname, lname);

    s->courses = NULL;
    s->num_courses = 0;
    s->average = 0.0;

    return s;
}

/**
 * @brief Ajoute une matière à un étudiant
 */
int student_add_course(Student *s, Course *c) {
    if (s == NULL || c == NULL) return -1;

    Course *tmp = realloc(s->courses, (s->num_courses + 1) * sizeof(Course));
    if (tmp == NULL) return -1;
    s->courses = tmp;

    Course *dest = &s->courses[s->num_courses];
    safe_strdup(&dest->name, c->name);
    dest->coef = c->coef;
    dest->grades.size = c->grades.size;

    if (c->grades.size > 0) {
        dest->grades.values = malloc(c->grades.size * sizeof(double));
        memcpy(dest->grades.values, c->grades.values, c->grades.size * sizeof(double));
    } else {
        dest->grades.values = NULL;
    }

    dest->average = c->average;
    s->num_courses++;

    return 0;
}

/**
 * @brief Met à jour la moyenne d'un étudiant
 */
void student_update_average(Student *s) {
    if (s == NULL) return;

    double sum = 0.0;
    double coef_sum = 0.0;

    for (size_t i = 0; i < s->num_courses; i++) {
        Course *c = &s->courses[i];
        sum += c->average * c->coef;
        coef_sum += c->coef;
    }

    if (coef_sum == 0.0)
        s->average = 0.0;
    else
        s->average = sum / coef_sum;
}

/**
 * @brief Détruit un étudiant
 */
void student_destroy(Student *s) {
    if (s == NULL) return;

    free(s->fname);
    free(s->lname);

    for (size_t i = 0; i < s->num_courses; i++) {
        free(s->courses[i].name);
        free(s->courses[i].grades.values);
    }
    free(s->courses);
    free(s);
}

/* -----------------------------------
   Promotion
   ----------------------------------- */

/**
 * @brief Crée une promotion vide
 */
Prom *prom_create(void) {
    Prom *p = malloc(sizeof(Prom));
    if (p == NULL) return NULL;
    p->students = NULL;
    p->num_students = 0;
    return p;
}

/**
 * @brief Ajoute un étudiant à la promotion
 */
int prom_add_student(Prom *p, Student *s) {
    if (p == NULL || s == NULL) return -1;

    Student *tmp = realloc(p->students, (p->num_students + 1) * sizeof(Student));
    if (tmp == NULL) return -1;
    p->students = tmp;

    Student *dest = &p->students[p->num_students];
    safe_strdup(&dest->fname, s->fname);
    safe_strdup(&dest->lname, s->lname);
    dest->id = s->id;
    dest->age = s->age;
    dest->average = s->average;

    // Copie des cours
    dest->num_courses = s->num_courses;
    if (s->num_courses > 0) {
        dest->courses = malloc(s->num_courses * sizeof(Course));
        for (size_t i = 0; i < s->num_courses; i++) {
            Course *src = &s->courses[i];
            Course *d = &dest->courses[i];
            safe_strdup(&d->name, src->name);
            d->coef = src->coef;
            d->grades.size = src->grades.size;
            if (src->grades.size > 0) {
                d->grades.values = malloc(src->grades.size * sizeof(double));
                memcpy(d->grades.values, src->grades.values, src->grades.size * sizeof(double));
            } else {
                d->grades.values = NULL;
            }
            d->average = src->average;
        }
    } else {
        dest->courses = NULL;
    }

    p->num_students++;
    return 0;
}

/**
 * @brief Détruit la promotion
 */
void prom_destroy(Prom *p) {
    if (p == NULL) return;

    for (size_t i = 0; i < p->num_students; i++) {
        student_destroy(&p->students[i]);
    }
    free(p->students);
    free(p);
}

/* -----------------------------------
   Lecture et écriture (simplifiées)
   ----------------------------------- */

/**
 * @brief Lecture du fichier texte data.txt
 */
Prom *prom_from_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir %s\n", filename);
        return NULL;
    }

    Prom *p = prom_create();
    char section[32];

    while (fscanf(f, "%31s", section) == 1) {
        if (strcmp(section, "ETUDIANTS") == 0) {
            int id, age;
            char prenom[64], nom[64];
            while (fscanf(f, "%d %s %s %d", &id, prenom, nom, &age) == 4) {
                Student *s = student_create(id, prenom, nom, age);
                prom_add_student(p, s);
                student_destroy(s);
            }
        } else if (strcmp(section, "NOTES") == 0) {
            int id;
            char mat[64];
            double note;
            while (fscanf(f, "%d %s %lf", &id, mat, &note) == 3) {
                for (size_t i = 0; i < p->num_students; i++) {
                    Student *s = &p->students[i];
                    if (s->id == id) {
                        // cherche si la matière existe déjà
                        size_t j;
                        for (j = 0; j < s->num_courses; j++) {
                            if (strcmp(s->courses[j].name, mat) == 0) break;
                        }
                        // si pas trouvée, on la crée
                        if (j == s->num_courses) {
                            Course *c = course_create(mat, 1.0);
                            student_add_course(s, c);
                            course_destroy(c);
                        }
                        course_add_grade(&s->courses[j], note);
                        student_update_average(s);
                        break;
                    }
                }
            }
        }
    }

    fclose(f);
    return p;
}

/**
 * @brief Sauvegarde binaire de la promotion (simplifiée)
 */
int prom_save_binary(const char *filename, const Prom *p) {
    FILE *f = fopen(filename, "wb");
    if (f == NULL) return -1;
    fwrite(p, sizeof(Prom), 1, f);
    fclose(f);
    return 0;
}

/**
 * @brief Lecture binaire (exemple simplifié)
 */
Prom *prom_load_binary(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL) return NULL;
    Prom *p = malloc(sizeof(Prom));
    fread(p, sizeof(Prom), 1, f);
    fclose(f);
    return p;
}
