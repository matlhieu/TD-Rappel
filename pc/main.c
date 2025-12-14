#include "read_files.h"
#include "display.h"
#include "sort.h"
#include "save.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Déclarations manuelles si pas dans les .h inclus
void API_display_results_per_field(Prom* p);
int API_cipher(char* in, char* out);
int API_decipher(char* in, char* out);

// Helper pour afficher le retour de l'API de tri
void show_sorted_api(Prom *p, int mode, char *txt) {
    if (API_set_sorting_mode(p, mode)) {
        printf("\n--- Tri par %s ---\n", txt);
        char **res = API_sort_students(p);
        if (res) {
            for(int i=0; i<SIZE_TOP1; i++) {
                if (res[i]) {
                    printf("%s\n", res[i]);
                    free(res[i]);
                }
            }
            free(res);
        }
    }
}

int main() {
    Prom *promo = load_promotion_from_file("data.txt");
    if (!promo) printf("Pas de data.txt ou erreur.\n");
    else API_set_sorting_mode(promo, AVERAGE); // Défaut

    int c = 0;
    while (c != 10) {
        printf("\n--- MENU ---\n");
        printf("1. Afficher tout\n");
        printf("2. Tri par NOM (API)\n");
        printf("3. Tri par PRENOM (API)\n");
        printf("4. Tri par MOYENNE (API)\n");
        printf("5. Tri par PIRE NOTE (API)\n");
        printf("6. Top 3 Matiere\n");
        printf("7. Validation (Bitwise)\n");
        printf("8. Sauvegarder & Chiffrer\n");
        printf("9. Dechiffrer binaire\n");
        printf("10. Quitter\n");
        printf("Choix : ");
        scanf("%d", &c);
        while(getchar() != '\n'); // vide buffer

        if (c==1 && promo) display_promotion(promo);
        if (c==2 && promo) show_sorted_api(promo, ALPHA_LAST_NAME, "Nom");
        if (c==3 && promo) show_sorted_api(promo, ALPHA_FIRST_NAME, "Prenom");
        if (c==4 && promo) show_sorted_api(promo, AVERAGE, "Moyenne");
        if (c==5 && promo) show_sorted_api(promo, MINIMUM, "Minimum");
        
        if (c==6 && promo) {
            char m[50];
            printf("Matiere : ");
            fgets(m, 50, stdin);
            m[strcspn(m, "\n")] = 0;
            get_top3_in_course(promo, m);
        }

        if (c==7 && promo) API_display_results_per_field(promo);

        if (c==8 && promo) {
            save_promotion_binary("temp.bin", promo);
            API_cipher("temp.bin", "data.enc");
            remove("temp.bin");
        }

        if (c==9) {
            API_decipher("data.enc", "data_decrypted.bin");
        }
    }

    if (promo) prom_destroy(promo);
    return 0;
}
