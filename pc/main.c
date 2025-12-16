#include "read_files.h"
#include "display.h"
#include "sort.h"
#include "save.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void API_display_results_per_field(Prom* pClass);

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
    Prom *promo = load_promotion_binary("binary.bin");
    if (promo != NULL) {
        printf("Loading succeed from 'binary.bin'\n");
    } 
    else {
        printf("No save found => loading 'data.txt'\n");
        promo = load_promotion_from_file("data.txt");

        if (promo == NULL) {
            printf("Impossible to load 'data.txt'. Check your files.\n");
            return 1; 
        }
    }

    API_set_sorting_mode(promo, AVERAGE);


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
        printf("8 : Save in binary.bin\n");
        printf("9 : Exit\n");
        
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
                save_promotion_binary("binary.bin", promo);
                printf("Data saved in binary.bin\n");
                }
                break;
            case 9:
                prom_destroy(promo);
                return 0;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 9);

    return 0;
}
