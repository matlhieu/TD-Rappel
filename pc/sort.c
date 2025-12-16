#include "sort.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



// Tri par Nom de famille (croissant)
int compare_alpha_last_name(const void *a, const void *b) {
    Student *s1 = (Student*)a;
    Student *s2 = (Student*)b;
    return strcmp(s1->lname, s2->lname);
}

// Tri par Prénom (croissant)
int compare_alpha_first_name(const void *a, const void *b) {
    Student *s1 = (Student*)a;
    Student *s2 = (Student*)b;
    return strcmp(s1->fname, s2->fname);
}

// Tri par Moyenne Générale (décroissant)
int compare_average(const void *a, const void *b) {
    Student *s1 = (Student*)a;
    Student *s2 = (Student*)b;
    // Décroissant : si s2 > s1, on retourne positif
    if (s2->average > s1->average) return 1;
    if (s2->average < s1->average) return -1;
    return 0;
}

// Helper pour trouver la pire moyenne d'un étudiant 
static double get_min_avg(Student *s) {
    if (s->num_course == 0) return 0.0;
    double min = 20.0; 
    int has_grades = 0;
    
    for(size_t i = 0; i < s->num_course; i++) {
        if(s->courses[i].passmark < min) {
            min = s->courses[i].passmark;
            has_grades = 1;
        }
    }
    return has_grades ? min : 0.0;
}

// Tri par Note Minimale (décroissant : le meilleur est celui qui a la "meilleure" pire note)
int compare_minimum(const void *a, const void *b) {
    Student *s1 = (Student*)a;
    Student *s2 = (Student*)b;
    
    double min1 = get_min_avg(s1);
    double min2 = get_min_avg(s2);

    if (min2 > min1) return 1;
    if (min2 < min1) return -1;
    return 0;
}

// Change le pointeur de fonction dans la structure Prom
int API_set_sorting_mode(Prom* pClass, int mode) {
    if (!pClass) return 0; // False (échec)

    switch (mode) {
        case ALPHA_FIRST_NAME:
            pClass->current_compare_func = compare_alpha_first_name;
            break;
        case ALPHA_LAST_NAME:
            pClass->current_compare_func = compare_alpha_last_name;
            break;
        case AVERAGE:
            pClass->current_compare_func = compare_average;
            break;
        case MINIMUM:
            pClass->current_compare_func = compare_minimum;
            break;
        default:
            return 0; // Mode inconnu
    }
    return 1; // Succès
}

// Top 10
char** API_sort_students(Prom* pClass) {
    if (!pClass || !pClass->students || !pClass->current_compare_func) {
        return NULL;
    }

    qsort(pClass->students, pClass->numStudent, sizeof(Student), pClass->current_compare_func);

    // Allocation du tableau 
    int count = (pClass->numStudent < SIZE_TOP1) ? pClass->numStudent : SIZE_TOP1;
    char **result = malloc(SIZE_TOP1 * sizeof(char*));
    
    if (!result) return NULL;

    for (int i = 0; i < SIZE_TOP1; i++) {
        if (i < count) {
            // Allocation dynamique pour chaque ligne
            result[i] = malloc(100 * sizeof(char)); 
            
            Student *s = &pClass->students[i];
            
            // Exemple : "1. [Moy: 12.5] Nom Prenom"
            snprintf(result[i], 100, "%d. [Avg: %.2f | Min: %.2f] %s %s", 
                     i+1, s->average, get_min_avg(s), s->lname, s->fname);
        } else {
            // Si moins de 10 étudiants, on met NULL ou une chaîne vide
            result[i] = NULL; 
        }
    }

    return result;
}

/* --- 3. FONCTIONS DE COMPATIBILITÉ (POUR DISPLAY.C) --- */

// Fonction manquante qui causait l'erreur
Student *get_top_students(Prom *p, int n) {
    if (n > p->numStudent) n = p->numStudent;

    Student *copy = malloc(p->numStudent * sizeof(Student));
    // On copie tout pour ne pas modifier l'ordre original
    memcpy(copy, p->students, p->numStudent * sizeof(Student));
    
    // On utilise le tri par moyenne pour cette "vieille" fonction
    qsort(copy, p->numStudent, sizeof(Student), compare_average);
    
    return copy;
}

typedef struct {
    Student *student;
    double average;
} StudentScore;

static int compare_scores(const void *a, const void *b) {
    StudentScore *sc1 = (StudentScore*)a;
    StudentScore *sc2 = (StudentScore*)b;
    if (sc2->average > sc1->average) return 1;
    if (sc2->average < sc1->average) return -1;
    return 0;
}

static double get_course_average(Student *s, const char *course_name) {
    for (size_t i = 0; i < s->num_course; i++) {
        if (strcmp(s->courses[i].name, course_name) == 0) {
            Course *c = &s->courses[i];
            if (c->grades.size == 0) return -1;
            double sum = 0;
            for (size_t j = 0; j < c->grades.size; j++) sum += c->grades.values[j];
            return sum / c->grades.size;
        }
    }
    return -1;
}

void get_top3_in_course(Prom *p, const char *course_name) {
     if (!p || p->numStudent == 0) { printf("No student.\n"); return; }
    
    // Utilisation d'un VLA (tableau taille variable)
    StudentScore temp[p->numStudent];
    int count = 0;
    
    for (int i = 0; i < p->numStudent; i++) {
        double avg = get_course_average(&p->students[i], course_name);
        if (avg >= 0) {
            temp[count].student = &p->students[i];
            temp[count].average = avg;
            count++;
        }
    }
    
    if (count == 0) { printf("No student has the course '%s'.\n", course_name); return; }
    
    qsort(temp, count, sizeof(StudentScore), compare_scores);
    
    int top = (count < 3) ? count : 3;
    printf("\n=== Top %d in %s ===\n", top, course_name);
    for (int i = 0; i < top; i++) {
        printf("%d) %s %s — average : %.2f\n", i + 1, temp[i].student->fname, temp[i].student->lname, temp[i].average);
    }
}