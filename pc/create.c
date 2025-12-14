#include "create.h"


Grades *grades_create() {
    Grades *g = malloc(sizeof(Grades));
    if (g == NULL) {
        return NULL;
    }
    g->values = NULL;
    g->size = 0;
    return g;
}

Course *course_create(char *name, double coef) {
    Course *c = malloc(sizeof(Course));
    if (c == NULL) {
        return NULL;
    }
    
    c->name = malloc(strlen(name)+1);
    if (c->name == NULL){
        free(c);
        return NULL;
    }
    strcpy(c->name, name);
    
    c->coef = coef;
    c->grades.values = NULL;
    c->grades.size = 0;
    c->grades.capacity = 0;
    c->passmark = 0.0;

    return c;
}

Student *student_create(int id,char *fname,char *lname, int age) {
    Student *s = malloc(sizeof(Student));
    if (s == NULL){
        return NULL;
    } 
    s->id = id;
    s->age = age;
    
    s->fname = malloc(strlen(fname)+1);
    s->lname = malloc(strlen(lname)+1);
    if (s->fname == NULL || s->lname == NULL){
        free(s->fname);
        free(s->lname);
        free(s);
        return NULL;
    }

    strcpy(s->fname, fname);
    strcpy(s->lname, lname);

    s->courses = NULL;
    s->num_course = 0;
    s->average = 0.0;

    return s;
}

Prom *prom_create(void) {
    Prom *p = malloc(sizeof(Prom));
    if (p == NULL){
        return NULL;
    } 
    p->students = NULL;
    p->numStudent = 0;
    return p;
}