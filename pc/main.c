#include "read_files.h"
#include "display.h"
#include "sort.h"
#include "save.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction helper pour afficher le résultat de l'API
void display_sorted_api(Prom *p, int mode, const char* mode_name) {
    if (API_set_sorting_mode(p, mode)) {
        printf("\n=== Sorting by: %s ===\n", mode_name);
        
        char **result = API_sort_students(p);
        if (result) {
            for (int i = 0; i < SIZE_TOP1; i++) {
                if (result[i] != NULL) {
                    printf("%s\n", result[i]);
                    free(result[i]); // Important : libérer chaque chaîne
                }
            }
            free(result); // Libérer le tableau de pointeurs
        }
    } else {
        printf("Error setting sort mode.\n");
    }
}

int main() {
    Prom *promo = load_promotion_from_file("data.txt");
    if (!promo) {
        printf("Error loading data.\n");
        return 1;
    }
    
    // Initialisation par défaut (Tri par moyenne)
    API_set_sorting_mode(promo, AVERAGE);

    int choice = 0;
    
    // Menu modifié pour l'exercice
    printf("What do you want to do ?\n");
    printf("1 : Display Full Promotion (Current Sort)\n");
    printf("2 : Sort by Last Name (API)\n");
    printf("3 : Sort by First Name (API)\n");
    printf("4 : Sort by Average (API)\n");
    printf("5 : Sort by Minimum (API)\n");
    printf("6 : Top 3 of a Course\n");
    printf("7 : Exit\n");
    
    printf("Choice: ");
    scanf("%d", &choice);
    while (getchar() != '\n');

    switch(choice) {
        case 1:
            // Utilise le tri actuel configuré dans promo
            display_promotion(promo);
            break;
        case 2:
            display_sorted_api(promo, ALPHA_LAST_NAME, "Last Name");
            break;
        case 3:
            display_sorted_api(promo, ALPHA_FIRST_NAME, "First Name");
            break;
        case 4:
            display_sorted_api(promo, AVERAGE, "Average");
            break;
        case 5:
            display_sorted_api(promo, MINIMUM, "Minimum Grade");
            break;
        case 6: {
            char course[50];
            printf("\nEnter course name: ");
            fgets(course, 50, stdin);
            course[strcspn(course, "\n")] = '\0';
            get_top3_in_course(promo, course);
            break;
        }
        case 7:
            prom_destroy(promo);
            return 0;
    }

    save_promotion_binary("binary.bin", promo);
    prom_destroy(promo);
    return 0;
}