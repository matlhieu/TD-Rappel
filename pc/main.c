#include "read_files.h"
#include "display.h"
#include "sort.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    Prom *promo = load_promotion_from_file("data.txt");
    if (!promo) {
        printf("Error loading.\n");
        return 1;
    }

    display_promotion(promo);
    Student *top10 = get_top_students(promo, 10);
    printf("\n=== Top 10 students ===\n");
    for (int i = 0; i < 10 && i < promo->numStudent; i++){
        printf("%s %s : %.2f\n", top10[i].fname, top10[i].lname, top10[i].average);
    }
    free(top10);
    prom_destroy(promo);
    return 0;
}