#include "struct.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Affiche les matières et notes d'un étudiant
 */
void afficher_student(const Student *s) {
    printf("\nÉtudiant : %s %s (ID %d, %d ans)\n", s->fname, s->lname, s->id, s->age);
    printf("  Moyenne générale : %.2f\n", s->average);

    if (s->num_courses == 0) {
        printf("  Aucune matière enregistrée.\n");
        return;
    }

    for (size_t i = 0; i < s->num_courses; i++) {
        const Course *c = &s->courses[i];
        printf("  - %s (coef %.2f) : ", c->name, c->coef);
        if (c->grades.size == 0) {
            printf("aucune note\n");
        } else {
            printf("notes : ");
            for (size_t j = 0; j < c->grades.size; j++) {
                printf("%.1f ", c->grades.values[j]);
            }
            printf("→ moyenne : %.2f\n", c->average);
        }
    }
}

/**
 * @brief Affiche tous les étudiants d'une promo
 */
void afficher_promo(const Prom *p) {
    printf("\n===== Promotion =====\n");
    if (p->num_students == 0) {
        printf("Aucun étudiant trouvé.\n");
        return;
    }
    for (size_t i = 0; i < p->num_students; i++) {
        afficher_student(&p->students[i]);
    }
}

/**
 * @brief Programme principal
 */
int main(void) {
    // Lecture depuis le fichier texte
    Prom *promo = prom_from_file("data.txt");
    if (promo == NULL) {
        printf("Erreur de lecture du fichier data.txt\n");
        return EXIT_FAILURE;
    }

    // Affichage
    afficher_promo(promo);

    // Sauvegarde binaire
    if (prom_save_binary("promo.bin", promo) == 0) {
        printf("\nFichier binaire 'promo.bin' enregistré avec succès.\n");
    } else {
        printf("\nErreur d'écriture du fichier binaire.\n");
    }

    // Libération mémoire
    prom_destroy(promo);

    // Relecture binaire (simplifiée)
    Prom *loaded = prom_load_binary("promo.bin");
    if (loaded != NULL) {
        printf("\nLecture du fichier binaire réussie !\n");
        printf("Attention : cette version simplifiée ne relit pas toutes les données internes.\n");
        free(loaded); // car ici on a juste lu la structure brute
    } else {
        printf("\nErreur de lecture du fichier binaire.\n");
    }

    return EXIT_SUCCESS;
}
