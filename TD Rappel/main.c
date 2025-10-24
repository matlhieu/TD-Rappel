#include "struct.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    Prom *promo = prom_from_file("data.txt");
    if (!promo) {
        printf("Erreur lecture fichier texte.\n");
        return 1;
    }
    printf("Lecture OK : %zu étudiants\n", promo->num_students);
    for (size_t i = 0; i < promo->num_students; i++) {
        Student *s = &promo->students[i];
        printf("%s %s : moyenne %.2f\n", s->fname, s->lname, s->average);
    }

    prom_save_binary("promo.bin", promo);
    prom_destroy(promo);

    Prom *load = prom_load_binary("promo.bin");
    printf("\nLecture binaire : %zu étudiants\n", load->num_students);
    for (size_t i = 0; i < load->num_students; i++) {
        Student *s = &load->students[i];
        printf("%s %s (ID %d) -> %.2f\n", s->fname, s->lname, s->id, s->average);
    }

    prom_destroy(load);
    return 0;
}
