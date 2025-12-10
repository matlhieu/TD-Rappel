#include "read_files.h"
#include "display.h"
#include "sort.h"
#include "save.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    Prom *promo = load_promotion_from_file("data.txt");
    if (!promo) {
        printf("Error loading.\n");
        return 1;
    }

   

    int choice = 0;
    printf("What do you want to display ?\n1 : Promotion\n2 : Top 10\n3 : Top 3 of a Course\n4 : All\n5 : Exit\n");
    scanf("%d", &choice);
    while (getchar() != '\n');
    choice_user(choice, promo); //display options

    
    save_promotion_binary("binary.bin", promo);

    prom_destroy(promo);
    /*Prom* p2 = calloc(0, sizeof(Prom));

    load_promotion_binary("binary.bin", p2);*/
    return 0;
}