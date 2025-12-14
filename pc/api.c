#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- EXERCICE BITWISE : Affichage par pole ---

int check_bits(Student *s, uint64_t mask) {
    // Si (valeur & mask) est egal a mask, alors tous les bits sont la
    return (s->validated_courses & mask) == mask;
}

void API_display_results_per_field(Prom* p) {
    if (p == NULL) return;

    printf("\n=== Validation des Poles ===\n");
    printf("%-15s %-15s | Sciences | Humanities | ANNEE\n", "Nom", "Prenom");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < p->numStudent; i++) {
        Student *s = &p->students[i];

        int ok_sci = check_bits(s, SCIENCES_MASK);
        int ok_hum = check_bits(s, HUMANITIES_MASK);
        int ok_year = check_bits(s, YEAR_MASK);

        printf("%-15s %-15s | %-8s | %-10s | %s\n",
               s->lname, s->fname,
               ok_sci ? "OK" : ".",
               ok_hum ? "OK" : ".",
               ok_year ? "VALIDE" : "NON");
    }
    printf("\n");
}

// --- EXERCICE CRYPTO : Master Key & XOR ---

// Génération de la Master Key selon l'algo du PDF
void generate_master_key(char* pass, unsigned char* key) {
    // Init avec valeurs hex du sujet
    unsigned char init[16] = {
        0x01, 0x45, 0x57, 0x89, 0xAB, 0xCD, 0xEF, 0x10,
        0x32, 0x54, 0x75, 0x98, 0xBA, 0xDC, 0xFE, 0x00 // dernier octet à 00 pour compléter
    };
    for(int i=0; i<16; i++) key[i] = init[i];

    int len = strlen(pass);
    int idx = 0;

    for (int i = 0; i < len; i++) {
        unsigned char c = (unsigned char)pass[i];
        
        // 1. XOR octet courant
        key[idx] = key[idx] ^ c;
        
        // 2. Somme octet suivant (modulo 16)
        int next = (idx + 1) % 16;
        key[next] = key[next] + c;
        
        // 3. Produit octet précédent
        int prev = (idx - 1);
        if (prev < 0) prev = 15;
        key[prev] = key[prev] * c;

        // Déplacement cyclique
        idx = (idx + 1) % 16;
    }
}

int API_cipher(char* pIn, char* pOut) {
    FILE *fin = fopen(pIn, "rb");
    if (!fin) { printf("Erreur ouverture entree\n"); return 0; }

    // Cle aleatoire fichier
    unsigned char file_key[16];
    srand(time(NULL));
    for(int i=0; i<16; i++) file_key[i] = rand() % 256;

    // Demande passphrase
    char pass[100];
    printf("Mot de passe chiffrement : ");
    // Lecture un peu "sale" avec scanf mais ça passe pour etudiant
    scanf("%s", pass);
    
    // Generer master key
    unsigned char master[16];
    generate_master_key(pass, master);

    // Chiffrer la cle du fichier
    unsigned char encrypted_key[16];
    for(int i=0; i<16; i++) encrypted_key[i] = file_key[i] ^ master[i];

    FILE *fout = fopen(pOut, "wb");
    if (!fout) { fclose(fin); return 0; }

    // Ecrire l'en-tête
    fwrite(encrypted_key, 1, 16, fout);

    // Chiffrer le contenu
    int c;
    int k = 0;
    while ((c = fgetc(fin)) != EOF) {
        unsigned char val = (unsigned char)c;
        val = val ^ file_key[k]; // XOR avec la clé fichier
        fputc(val, fout);
        
        k++;
        if (k >= 16) k = 0;
    }

    fclose(fin);
    fclose(fout);
    printf("Fichier chiffre cree : %s\n", pOut);
    return 1;
}

int API_decipher(char* pIn, char* pOut) {
    FILE *fin = fopen(pIn, "rb");
    if (!fin) { printf("Erreur fichier chiffre\n"); return 0; }

    unsigned char header[16];
    if (fread(header, 1, 16, fin) != 16) {
        fclose(fin); return 0;
    }

    char pass[100];
    printf("Mot de passe dechiffrement : ");
    scanf("%s", pass);

    unsigned char master[16];
    generate_master_key(pass, master);

    // Retrouver la clé fichier
    unsigned char file_key[16];
    for(int i=0; i<16; i++) file_key[i] = header[i] ^ master[i];

    FILE *fout = fopen(pOut, "wb");
    if (!fout) { fclose(fin); return 0; }

    int c;
    int k = 0;
    while ((c = fgetc(fin)) != EOF) {
        unsigned char val = (unsigned char)c;
        val = val ^ file_key[k];
        fputc(val, fout);

        k++;
        if (k >= 16) k = 0;
    }

    fclose(fin);
    fclose(fout);
    printf("Fichier dechiffre : %s\n", pOut);
    return 1;
}
