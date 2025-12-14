#include "read_files.h"
#include "create.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
    int l = strlen(str);
    while(l > 0 && (str[l-1] == '\n' || str[l-1] == '\r')) str[--l] = '\0';
}

Student *find_student(Prom *p, int id) {
    for (int i = 0; i < p->numStudent; i++)
        if (p->students[i].id == id) return &p->students[i];
    return NULL;
}

Course *find_course(Student *s, char *name) {
    for (int i = 0; i < s->num_course; i++)
        if (strcmp(s->courses[i].name, name) == 0) return &s->courses[i];
    return NULL;
}

// Mapping nom -> enum
int get_index_mat(char* name) {
    if (strcmp(name, "Mathematiques") == 0) return MATHS;
    if (strcmp(name, "Physique") == 0) return PHYSIQUE;
    if (strcmp(name, "Biologie") == 0) return BIOLOGIE;
    if (strcmp(name, "Informatique") == 0) return INFORMATIQUE;
    if (strcmp(name, "Economie") == 0) return ECONOMIE;
    if (strcmp(name, "Francais") == 0 || strcmp(name, "Français") == 0) return FRANCAIS;
    if (strcmp(name, "Histoire") == 0) return HISTOIRE;
    if (strcmp(name, "Geographie") == 0) return GEOGRAPHIE;
    if (strcmp(name, "Philosophie") == 0) return PHILOSOPHIE;
    if (strcmp(name, "Anglais") == 0) return ANGLAIS;
    if (strcmp(name, "Espagnol") == 0) return ESPAGNOL;
    if (strcmp(name, "Latin") == 0) return LATIN;
    if (strcmp(name, "Musique") == 0) return MUSIQUE;
    if (strcmp(name, "Arts Plastiques") == 0) return ARTS_PLASTIQUES;
    if (strcmp(name, "Sociologie") == 0) return SOCIOLOGIE;
    if (strcmp(name, "Sciences Sociales") == 0) return SCIENCES_SOCIALES;
    if (strcmp(name, "EPS") == 0) return EPS;
    return -1;
}

void student_update_average(Student *s) {
    if (!s) return;
    double total = 0, coefs = 0;
    
    // Reset bits
    s->validated_courses = 0;

    for (int i = 0; i < s->num_course; i++) {
        Course *c = &s->courses[i];
        double sum = 0;
        for (int j = 0; j < c->grades.size; j++) sum += c->grades.values[j];
        
        c->passmark = (c->grades.size > 0) ? sum / c->grades.size : 0;

        // Logique Bitwise : si moyenne >= 10, on allume le bit
        if (c->passmark >= 10.0) {
            int idx = get_index_mat(c->name);
            if (idx != -1) {
                s->validated_courses |= (1ULL << idx);
            }
        }

        total += c->passmark * c->coef;
        coefs += c->coef;
    }
    s->average = (coefs > 0) ? total / coefs : 0;
}

Prom *load_promotion_from_file(char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("Erreur fichier\n"); return NULL; }

    Prom *p = prom_create();
    char line[200];
    int mode = 0;
    Course *ref_courses = NULL;
    int nb_ref = 0;

    while (fgets(line, 200, f)) {
        trim(line);
        if (strlen(line) == 0) continue;

        if (strcmp(line, "ETUDIANTS") == 0) { mode=1; continue; }
        if (strcmp(line, "MATIERES") == 0) { mode=2; continue; }
        if (strcmp(line, "NOTES") == 0) { mode=3; continue; }

        if (mode == 1) {
            int id, age; char n[50], pnom[50];
            if (sscanf(line, "%d;%49[^;];%49[^;];%d", &id, n, pnom, &age) == 4) {
                Student *s = student_create(id, n, pnom, age);
                p->students = realloc(p->students, (p->numStudent + 1) * sizeof(Student));
                p->students[p->numStudent] = *s;
                p->numStudent++;
                free(s);
            }
        }
        else if (mode == 2) {
            char n[50]; double k;
            if (sscanf(line, "%49[^;];%lf", n, &k) == 2) {
                Course *c = course_create(n, k);
                ref_courses = realloc(ref_courses, (nb_ref + 1) * sizeof(Course));
                ref_courses[nb_ref] = *c;
                nb_ref++;
                free(c);
            }
        }
        else if (mode == 3) {
            int id; char mat[50]; double note;
            if (sscanf(line, "%d;%49[^;];%lf", &id, mat, &note) == 3) {
                Student *s = find_student(p, id);
                if (s) {
                    Course *c = find_course(s, mat);
                    if (!c) {
                        for(int i=0; i<nb_ref; i++) {
                            if (strcmp(ref_courses[i].name, mat) == 0) {
                                s->courses = realloc(s->courses, (s->num_course+1)*sizeof(Course));
                                s->courses[s->num_course] = ref_courses[i];
                                c = &s->courses[s->num_course];
                                s->num_course++;
                                break;
                            }
                        }
                    }
                    if (c) {
                        c->grades.values = realloc(c->grades.values, (c->grades.size+1)*sizeof(double));
                        c->grades.values[c->grades.size++] = note;
                        student_update_average(s);
                    }
                }
            }
        }
    }
    free(ref_courses);
    fclose(f);
    return p;
}
