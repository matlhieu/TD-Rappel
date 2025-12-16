#include "read_files.h"
#include "create.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void trim(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r' || s[len - 1] == ' ')) {
        s[--len] = '\0';
    }
}

static Student *find_student_by_id(Prom *p, int id) {
    for (int i = 0; i < p->numStudent; i++) {
        if (p->students[i].id == id)
            return &p->students[i];
    }
    return NULL;
}

static Course *find_course_in_student(Student *s, const char *name) {
    for (size_t i = 0; i < s->num_course; i++) {
        if (strcmp(s->courses[i].name, name) == 0)
            return &s->courses[i];
    }
    return NULL;
}

// Helper pour mapper le nom de la matière à l'enum CourseIndex
static int get_course_index_from_name(const char* name) {
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

    return -1; // Inconnu
}

/* --- Mise à jour des moyennes et des bits de validation --- */

void student_update_average(Student *s) {
    if (!s || s->num_course == 0) return;

    double total = 0.0;
    double coef_sum = 0.0;
    
    // Réinitialisation du champ de bits
    s->validated_courses = 0;

    for (size_t i = 0; i < s->num_course; i++) {
        Course *c = &s->courses[i];
        double sum = 0.0;
        
        for (size_t j = 0; j < c->grades.size; j++) {
            sum += c->grades.values[j];
        }

        if (c->grades.size > 0)
            c->passmark = sum / c->grades.size;
        else 
            c->passmark = 0.0;

        // Mise à jour du bit de validation si moyenne >= 10.0
        if (c->passmark >= 10.0) {
            int course_idx = get_course_index_from_name(c->name);
            if (course_idx >= 0) {
                // Opération binaire pour mettre le bit à 1
                s->validated_courses |= (1ULL << course_idx);
            }
        }

        total += c->passmark * c->coef;
        coef_sum += c->coef;
    }

    if (coef_sum > 0)
        s->average = total / coef_sum;
    else
        s->average = 0.0;
}

Prom *load_promotion_from_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Error opening file.\n");
        return NULL;
    }

    Prom *promo = prom_create();
    char line[256];
    int section = 0; 
    
    Course *courses_ref = NULL;
    int nb_courses = 0;
    
    int skip_header = 0;

    while (fgets(line, sizeof(line), f)) {
        trim(line);
        if (strlen(line) == 0) continue;

        if (strcmp(line, "ETUDIANTS") == 0) { section = 1; skip_header = 1; continue; }
        if (strcmp(line, "MATIERES") == 0)  { section = 2; skip_header = 1; continue; }
        if (strcmp(line, "NOTES") == 0)     { section = 3; skip_header = 1; continue; }

        if (skip_header) {
            skip_header = 0;
            continue;
        }

        if (section == 1) { // ETUDIANTS
            int id, age;
            char fname[50], lname[50];
            if (sscanf(line, "%d;%49[^;];%49[^;];%d", &id, fname, lname, &age) == 4) {
                Student *s = student_create(id, fname, lname, age);
                promo->students = realloc(promo->students, (promo->numStudent + 1) * sizeof(Student));
                promo->students[promo->numStudent] = *s;
                promo->numStudent++;
                free(s);
            }
        }
        else if (section == 2) { // MATIERES
            char name[50]; double coef;
            if (sscanf(line, "%49[^;];%lf", name, &coef) == 2) {
                Course *c = course_create(name, coef);
                courses_ref = realloc(courses_ref, (nb_courses + 1) * sizeof(Course));
                courses_ref[nb_courses] = *c;
                nb_courses++;
                free(c);
            }
        }
        else if (section == 3) { // NOTES
            int id; char cname[50]; double grade;
            if (sscanf(line, "%d;%49[^;];%lf", &id, cname, &grade) == 3) {
                Student *s = find_student_by_id(promo, id);
                if (s) {
                    Course *c = find_course_in_student(s, cname);
                    if (!c) {
                        for (int i = 0; i < nb_courses; i++) {
                            if (strcmp(courses_ref[i].name, cname) == 0) {
                                s->courses = realloc(s->courses, (s->num_course + 1) * sizeof(Course));
                                s->courses[s->num_course] = courses_ref[i]; 
                                c = &s->courses[s->num_course];
                                s->num_course++;
                                break;
                            }
                        }
                    }
                    if (c) {
                        c->grades.values = realloc(c->grades.values, (c->grades.size + 1) * sizeof(double));
                        c->grades.values[c->grades.size++] = grade;
                        student_update_average(s);
                    }
                }
            }
        }
    }

    free(courses_ref);
    fclose(f);
    return promo;
}