#include "recalcul.h"
#include <stdio.h>

int miseAJourNotes(Prom *p, Recalcul fct){
    if (!p || !p->students || p->numStudent == 0 || !fct){
        return 0;
    }

    float min_promo = 20.0f;
    float max_promo = 0.0f;

    for (int i=0; i < p->numStudent; i++){
        float avg = p->students[i].average;
        if (avg < min_promo) min_promo = avg;
        if (avg > max_promo) max_promo = avg;   

    }
    printf("Min promo : %.2f | Max promo %.2f", min_promo, max_promo); /*Test*/

    for (int i = 0; i<p->numStudent; i++){
        fct(&p->students[i], min_promo, max_promo);
    }

    
    return 1;
}

void normalisation(Student *s, float min_promo, float max_promo) {
    if (!s) {
        fprintf(stderr, "Error Student pointeur NULL\n");
        exit(EXIT_FAILURE);
    }

    float range = max_promo - min_promo;

    if (range == 0.0f) {
        printf("Error, stopping programm\n");
        exit(EXIT_FAILURE); 
    }

    s->average = ((s->average - min_promo) / range) * 20.0f;
}