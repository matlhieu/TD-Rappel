#include "read_files.h"
#include "display.h"
#include "sort.h"
#include "save.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Chargement des données
    Prom *promo = load_promotion_from_file("data.txt");
    if (promo == NULL) {
        printf("Erreur lors du chargement des données.\n");
        return 1;
    }

    int choix = 0;
    
    // Menu principal
    printf("Que voulez-vous afficher ?\n");
    printf("1 : La promotion complete\n");
    printf("2 : Le Top 10 des etudiants\n");
    printf("3 : Le Top 3 par matiere\n");
    printf("4 : Tout afficher\n");
    printf("5 : Quitter\n");
    printf("Votre choix : ");
    
    scanf("%d", &choix);
    
    // Vider le buffer (classique en C)
    while (getchar() != '\n');

    // Gestion du choix
    choice_user(choix, promo);

    // Sauvegarde en binaire (bonus)
    save_promotion_binary("binary.bin", promo);

    // Nettoyage de la mémoire
    prom_destroy(promo);

    return 0;
}
