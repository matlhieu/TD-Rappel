#include "save.h"
#include "create.h"
#include "read_files.h" 
#include <stdio.h>
#include <stdlib.h>     
#include <string.h>     

int save_promotion_binary(const char *filename, Prom *p) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Error fopen");
        return -1;
    }

    fwrite(&p->numStudent, sizeof(int), 1, f);
    
    for (int i = 0; i < p->numStudent; i++) {
        Student *s = &p->students[i];

        // Écriture ID et Age
        fwrite(&s->id, sizeof(int), 1, f);
        fwrite(&s->age, sizeof(int), 1, f);

        // Écriture Prénom
        int len_fname = strlen(s->fname) + 1;
        fwrite(&len_fname, sizeof(int), 1, f);
        fwrite(s->fname, sizeof(char), len_fname, f);

        // Écriture Nom
        int len_lname = strlen(s->lname) + 1;
        fwrite(&len_lname, sizeof(int), 1, f);
        fwrite(s->lname, sizeof(char), len_lname, f);

        // Écriture des matières
        fwrite(&s->num_course, sizeof(size_t), 1, f);
        for (size_t j = 0; j < s->num_course; j++) {
            Course *c = &s->courses[j];
            
            int len_name = strlen(c->name) + 1;
            fwrite(&len_name, sizeof(int), 1, f);
            fwrite(c->name, sizeof(char), len_name, f);
            
            fwrite(&c->coef, sizeof(double), 1, f);

            fwrite(&c->grades.size, sizeof(size_t), 1, f);
            fwrite(c->grades.values, sizeof(double), c->grades.size, f);
        }

        fwrite(&s->average, sizeof(float), 1, f);
    }

    fclose(f);
    return 0;
}

Prom* load_promotion_binary(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        return NULL; 
    }

    Prom* p = prom_create();
    if (!p) { 
        fclose(f); 
        return NULL; 
    }

    if (fread(&p->numStudent, sizeof(int), 1, f) != 1) {
        prom_destroy(p); 
        fclose(f); 
        return NULL;
    }

    printf("Loading binary : %d students detected.\n", p->numStudent);

    if (p->numStudent > 0) {
        p->students = malloc(sizeof(Student) * p->numStudent);
    }

    for (int i = 0; i < p->numStudent; i++) {
        Student* s = &p->students[i];

        char *ptr_s = (char*)s;
        for (size_t k = 0; k < sizeof(Student); k++) {
            ptr_s[k] = 0;
        }
        
        fread(&s->id, sizeof(int), 1, f);
        fread(&s->age, sizeof(int), 1, f);

        int len_fname;
        fread(&len_fname, sizeof(int), 1, f);
        s->fname = malloc(len_fname);
        fread(s->fname, sizeof(char), len_fname, f);

        int len_lname;
        fread(&len_lname, sizeof(int), 1, f);
        s->lname = malloc(len_lname);
        fread(s->lname, sizeof(char), len_lname, f);

        fread(&s->num_course, sizeof(size_t), 1, f);
        if (s->num_course > 0) {
            s->courses = malloc(sizeof(Course) * s->num_course);
        }

        for (size_t j = 0; j < s->num_course; j++) {
            Course* c = &s->courses[j];
            memset(c, 0, sizeof(Course)); 

            int len_name;
            fread(&len_name, sizeof(int), 1, f);
            c->name = malloc(len_name);
            fread(c->name, sizeof(char), len_name, f);

            fread(&c->coef, sizeof(double), 1, f);

            fread(&c->grades.size, sizeof(size_t), 1, f);
            c->grades.capacity = c->grades.size;
            
            if (c->grades.size > 0) {
                c->grades.values = malloc(sizeof(double) * c->grades.size);
                fread(c->grades.values, sizeof(double), c->grades.size, f);
            } else {
                c->grades.values = NULL;
            }
        }

        fread(&s->average, sizeof(float), 1, f);
        

        student_update_average(s);
    }

    fclose(f);
    return p;
}