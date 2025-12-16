#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

static int check_validation(Student *s, uint64_t mask) {
    return (s->validated_courses & mask) == mask;
}

void API_display_results_per_field(Prom* pClass) {
    if (!pClass) {
        return;
    }
    printf("\n============================== Student Validation Status ==============================\n");
    printf("%-20s %-20s | %-10s | %-10s | %-8s | %-10s\n", "Last Name", "First Name", "Sciences", "Humanities", "Maths", "Full Year");
    printf("-------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < pClass->numStudent; i++) {
        Student *s = &pClass->students[i];

        int val_sciences = check_validation(s, SCIENCES_MASK);
        int val_maths = check_validation(s, MATHS_MASK);
        int val_humanities = check_validation(s, HUMANITIES_MASK);

        /* Année validée si Sciences ET Humanités ET Maths sont validés */
        int val_year = check_validation(s, YEAR_MASK);

        printf("%-20s %-20s | %-10s | %-10s | %-8s | %-17s |\n",
               s->lname,
               s->fname,
               val_sciences ? "VALID" : "---",
               val_humanities ? "VALID" : "---",
               val_maths ? "VALID" : "---",
               val_year ? "\x1b[32mPASSED\x1b[0m" : "\x1b[31mFAIL\x1b[0m");
    }
    printf("\n");
}


int save_promotion_binary(const char *filename, Prom *p);
void load_promotion_binary(char* filename, Prom* p);