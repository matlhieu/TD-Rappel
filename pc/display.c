#include "struct.h"
#include "sort.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void display_promotion(Prom *p) {
    for (int i = 0; i < p->numStudent; i++) {
        Student *s = &p->students[i];
        printf("\x1b[33m%s %s\x1b[0m | ID : %d | %d years old\n", s->fname, s->lname, s->id, s->age);
        
        for (size_t j = 0; j < s->num_course; j++) {
            Course *c = &s->courses[j];
            printf("%s\x1b[0m | coef %.2f :", c->name, c->coef);

            float sum = 0.0;
            for (size_t k = 0; k < c->grades.size; k++) {
                printf(" %.1f /", c->grades.values[k]);
                sum += c->grades.values[k];
            }
            float passmark = (c->grades.size > 0) ? sum / c->grades.size : 0.0;
            printf("[Average] : \x1b[35m%.2f\x1b[0m\n", passmark);
        }
        printf("\n=> Average is : \x1b[35m%.2f\x1b[0m\n\n", s->average);
    }
}   

void choice_user(int c, Prom *promo) {
    if (c == 1) {
        display_promotion(promo);
    }
    else if (c == 2) {
        Student *top10 = get_top_students(promo, 10);
        printf("\n=== Top 10 students ===\n");
        for (int i = 0; i < 10 && i < promo->numStudent; i++) {
            printf("%s %s : %.2f\n", top10[i].fname, top10[i].lname, top10[i].average);
        }   
        free(top10);
    }
    else if (c == 3) {
        char course[50];
        printf("\nEnter the name of a course to show the top 3 : ");

        fgets(course, 50, stdin);
        course[strcspn(course, "\n")] = '\0'; // Enlever le saut de ligne
        
        get_top3_in_course(promo, course); 
    }
    else if (c == 4) {
        display_promotion(promo);
        
        Student *top10 = get_top_students(promo, 10);
        printf("\n=== Top 10 students ===\n");
        for (int i = 0; i < 10 && i < promo->numStudent; i++) {
            printf("%s %s : %.2f\n", top10[i].fname, top10[i].lname, top10[i].average);
        }   
        free(top10);

        char course[50];
        printf("\nEnter the name of a course to show the top 3 : ");
        
        fgets(course, 50, stdin);
        course[strcspn(course, "\n")] = '\0';
    
        get_top3_in_course(promo, course); 
    }
    else if (c == 5) {
        exit(0);
    }
}