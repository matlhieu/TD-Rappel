#ifndef STRUCT_H
#define STRUCT_H

typedef struct liste {
    float nombre;
    struct liste* suivant;
} liste;

typedef struct grades {
    char* nom;
    liste* tab;
    struct grades* suivant;
    float moyenne;
} grades;

typedef struct course {
    char* nom;
    float coef;
    struct course* suivant;
} course;

typedef struct student {
    char* prenom;
    char* nom;
    int age;
    int num;
    grades* matieres;
    float moyenne;
    struct student* suivant;
} student;

typedef struct prom {
    student* tab_etu;
    int nb_etu;
    struct prom* suivant;
} prom;

/* prototypes constructeurs / destructeurs */
liste* ConstructeurListe(liste* a, float nb);
float moyenneMatiere(liste* a);
void ConstructeurMatiere(grades** a, float nb, char* mat);
student* ajouterNotes(student* etu, char* mat, int id, float note);
void ConstructeurCourse(course** matiere, char* a, float coef);
student* ConstructeurStudent(student* etu, char* prenom, char* nom, int age, int num);
student* moyenneEtu(student* stu, course* mat);
prom* ConstructeurProm(prom* a, student* etu);

/* destructeurs */
void destructeurListe(liste* a);
void destructeurMat(grades* a);
void destructeurStu(student* a);
void destructeurProm(prom* a);
void destructeurCourse(course* a);

/* lecture / écriture fichier */
student* lecture(course** matiere, const char* filename);
int binaryWrite(prom* promotion, course* matiere);
student* lectureBinaire(prom* p, course** matiere);

/* affichages */
void affichageStudent(student* a);
void affichageCourse(course* a);
#endif
