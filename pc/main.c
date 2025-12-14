#include "read_files.h"
#include "display.h"
#include "sort.h"
#include "save.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Déclarations externes pour les nouvelles fonctions API (si pas dans header)
void API_display_results_per_field(Prom* pClass);
int API_cipher(const char* pIn, const char* pOut);
int API_decipher(const char* pIn, const char* pOut);

// Fonction helper pour afficher le résultat de l'API de tri
void display_sorted_api(Prom *p, int mode, const char* mode_name) {
    if (API_set_sorting_mode(p, mode)) {
        printf("\n=== Sorting by: %s ===\n", mode_name);
        char **result = API_sort_students(p);
        if (result) {
            for (int i = 0; i < SIZE_TOP1; i++) {
                if (result[i] != NULL) {
                    printf("%s\n", result[i]);
                    free(result[i]);
                }
            }
            free(result);
        }
    } else {
        printf("Error setting sort mode.\n");
    }
}

int main() {
    // On charge data.txt au début
    Prom *promo = load_promotion_from_file("data.txt");
    if (!promo) {
        printf("Error loading data (checking for decrypted binary...)\n");
        // Optionnel : Essayer de charger un binaire si texte absent, mais simplifions
    } else {
        API_set_sorting_mode(promo, AVERAGE);
    }

    int choice = 0;
    do {
        printf("\n--- MENU ---\n");
        printf("1 : Display Full Promotion\n");
        printf("2 : Sort by Last Name\n");
        printf("3 : Sort by First Name\n");
        printf("4 : Sort by Average\n");
        printf("5 : Sort by Minimum\n");
        printf("6 : Top 3 of a Course\n");
        printf("7 : Display Validation (Sciences/Humanities)\n");
        printf("8 : Save Binary & Encrypt\n");
        printf("9 : Decrypt Binary\n");
        printf("10: Exit\n");
        
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // vide buffer erreur
            continue;
        }
        while (getchar() != '\n');

        switch(choice) {
            case 1: if(promo) display_promotion(promo); break;
            case 2: if(promo) display_sorted_api(promo, ALPHA_LAST_NAME, "Last Name"); break;
            case 3: if(promo) display_sorted_api(promo, ALPHA_FIRST_NAME, "First Name"); break;
            case 4: if(promo) display_sorted_api(promo, AVERAGE, "Average"); break;
            case 5: if(promo) display_sorted_api(promo, MINIMUM, "Minimum Grade"); break;
            case 6: 
                if(promo) {
                    char course[50];
                    printf("Course name: ");
                    fgets(course, 50, stdin);
                    course[strcspn(course, "\n")] = '\0';
                    get_top3_in_course(promo, course);
                }
                break;
            case 7:
                if (promo) API_display_results_per_field(promo);
                break;
            case 8:
                if (promo) {
                    // Sauvegarde temporaire en clair puis chiffrement
                    save_promotion_binary("temp.bin", promo);
                    API_cipher("temp.bin", "data.enc");
                    remove("temp.bin"); // Supprime le fichier clair
                } else printf("No data to save.\n");
                break;
            case 9:
                // Déchiffre data.enc vers data_decrypted.bin
                if (API_decipher("data.enc", "data_decrypted.bin")) {
                    printf("You can now verify 'data_decrypted.bin' content.\n");
                }
                break;
            case 10:
                prom_destroy(promo);
                return 0;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 10);

    return 0;
}