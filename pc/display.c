#include <stdio.h>
#include <stdlib.h>

void display_promotion(Promo *p){
    for (int i=0; i < p->numStudent; i++){
        Student *s = &p->student[i];
        printf("ID : %d | %s %s | %d ans\n", s->id, s-> fname, s->lname, s->age);
        for (size_t j = 0; j < s->num_course; j++){
            Course *c = &s->courses[j];
            printf("%s | coef %.2f : ", c->name, c->coef);
            for (size_t k = 0; k < c->grades.size; k++){
                printf("%.1f", c->grades.values[k]);
            printf("\n");
            }
            printf("=> Average is : %.2f\n\n", s->average);
        }
    }
}