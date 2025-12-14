#include "sort.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* --- EXERCICE POINTEURS : Fonctions de comparaison --- */

// Tri par Nom (Attention: le PDF dit "First Name" pour nom de famille parfois, je suis la logique standard lname=Nom)
int compare_alpha_last_name(const void *a, const void *b) {
    Student *s1 = (Student*)a;
    Student *s2 = (Student*)b;
    return strcmp(s1->lname, s2->lname);
}

// Tri par Prénom
int compare_alpha_first_name(const void *a, const void *b) {
    Student *s1 = (Student*)a;
    Student *s2 = (Student*)b;
    return strcmp(s1->fname, s2->fname);
}

// Tri par Moyenne (Décroissant)
int compare_average(const void *a, const void *b) {
    Student *s1 = (Student*)a;
    Student *s2 = (Student*)b;
    if (s2->average > s1->average) return 1;
    if (s2->average < s1->average) return -1;
    return 0;
}

// Helper pour trouver la pire moyenne
double get_min_grade(Student *s) {
    if (s->num_course == 0) return 0;
    double min = 20.0;
    int has_grade = 0;
    for (int i=0; i < s->num_course; i++) {
        if (s->courses[i].passmark < min) {
            min = s->courses[i].passmark;
            has_grade = 1;
        }
    }
    return has_grade ? min : 0;
}

// Tri par Note Minimale (Décroissant)
int compare_minimum(const void *a, const void *b) {
    Student *s1 = (Student*)a;
    Student *s2 = (Student*)b;
    double min1 = get_min_grade(s1);
    double min2 = get_min_grade(s2);
    if (min2 > min1) return 1;
    if (min2 < min1) return -1;
    return 0;
}

/* --- EXERCICE POINTEURS : Fonctions API --- */

// Change la fonction de tri utilisée
int API_set_sorting_mode(Prom* pClass, int mode) {
    if (pClass == NULL) return 0;

    switch (mode) {
        case ALPHA_FIRST_NAME: // Prénom
            pClass->current_compare_func = compare_alpha_first_name;
            break;
        case ALPHA_LAST_NAME: // Nom
            pClass->current_compare_func = compare_alpha_last_name;
            break;
        case AVERAGE:
            pClass->current_compare_func = compare_average;
            break;
        case MINIMUM:
            pClass->current_compare_func = compare_minimum;
            break;
        default:
            return 0; // Erreur
    }
    return 1; // Ok
}

// Effectue le tri et renvoie les top 10 chaines
char** API_sort_students(Prom* pClass) {
    if (!pClass || !pClass->students || !pClass->current_compare_func) return NULL;

    // On trie toute la promo avec le pointeur configuré
    qsort(pClass->students, pClass->numStudent, sizeof(Student), pClass->current_compare_func);

    // On prépare le tableau de résultats
    char **tab = malloc(SIZE_TOP1 * sizeof(char*));
    
    int max = (pClass->numStudent < SIZE_TOP1) ? pClass->numStudent : SIZE_TOP1;

    for (int i = 0; i < SIZE_TOP1; i++) {
        if (i < max) {
            tab[i] = malloc(200 * sizeof(char));
            Student *s = &pClass->students[i];
            // Format d'affichage libre
            sprintf(tab[i], "%d. %s %s (Moy: %.2f | Min: %.2f)", 
                    i+1, s->lname, s->fname, s->average, get_min_grade(s));
        } else {
            tab[i] = NULL;
        }
    }
    return tab;
}

// --- Fonctions pour le Top 3 Matière (Ancien exercice) ---

typedef struct {
    Student *s;
    double avg;
} Score;

int cmp_score(const void *a, const void *b) {
    Score *s1 = (Score*)a;
    Score *s2 = (Score*)b;
    if (s2->avg > s1->avg) return 1;
    if (s2->avg < s1->avg) return -1;
    return 0;
}

void get_top3_in_course(Prom *p, char *matiere) {
    if (!p) return;
    
    // Tableau temporaire
    Score scores[p->numStudent];
    int count = 0;

    for (int i=0; i < p->numStudent; i++) {
        Student *s = &p->students[i];
        for (int j=0; j < s->num_course; j++) {
            if (strcmp(s->courses[j].name, matiere) == 0) {
                scores[count].s = s;
                scores[count].avg = s->courses[j].passmark;
                count++;
                break;
            }
        }
    }

    if (count == 0) {
        printf("Personne n'a la matiere %s\n", matiere);
        return;
    }

    qsort(scores, count, sizeof(Score), cmp_score);

    printf("Top 3 en %s :\n", matiere);
    int limit = (count < 3) ? count : 3;
    for (int i=0; i < limit; i++) {
        printf("%d. %s %s : %.2f\n", i+1, scores[i].s->lname, scores[i].s->fname, scores[i].avg);
    }
}
