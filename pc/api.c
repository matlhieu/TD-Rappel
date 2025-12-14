#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Pour srand/rand

// ------------------------------------------------------------------
// PARTIE 1 : BITWISE (Validation par pôles)
// ------------------------------------------------------------------

// Fonction interne pour vérifier un masque
static int check_validation(Student *s, uint64_t mask) {
    // Si tous les bits du masque sont présents dans validated_courses
    return (s->validated_courses & mask) == mask;
}

void API_display_results_per_field(Prom* pClass) {
    if (!pClass) return;

    printf("\n=== Student Validation Status ===\n");
    printf("%-20s %-20s | %-10s | %-10s | %-10s\n", "Last Name", "First Name", "Sciences", "Humanities", "Full Year");
    printf("----------------------------------------------------------------------------------\n");

    for (int i = 0; i < pClass->numStudent; i++) {
        Student *s = &pClass->students[i];

        int val_sciences = check_validation(s, SCIENCES_MASK);
        int val_humanities = check_validation(s, HUMANITIES_MASK);
        // Année validée si Sciences ET Humanités (et potentiellement autres) sont validés
        int val_year = check_validation(s, YEAR_MASK);

        printf("%-20s %-20s | %-10s | %-10s | %-10s\n",
               s->lname,
               s->fname,
               val_sciences ? "VALID" : "---",
               val_humanities ? "VALID" : "---",
               val_year ? "\x1b[32mPASSED\x1b[0m" : "\x1b[31mFAIL\x1b[0m");
    }
    printf("\n");
}


// ------------------------------------------------------------------
// PARTIE 2 : CRYPTO (XOR & Master Key)
// ------------------------------------------------------------------

// Algorithme de génération de la Master Key (16 octets)
static void generate_master_key(const char* passphrase, unsigned char* key) {
    // 1. Initialisation avec les valeurs hexadécimales données
    unsigned char init_vals[16] = {
        0x01, 0x45, 0x57, 0x89, 0xAB, 0xCD, 0xEF, 0x10,
        0x32, 0x54, 0x75, 0x98, 0xBA, 0xDC, 0xFE, 0x00 // J'ajoute 0x00 pour faire 16 octets
    };
    memcpy(key, init_vals, 16);

    int pass_len = strlen(passphrase);
    int key_idx = 0; // Index courant de la clé

    // 2. Boucle sur la passphrase
    for (int i = 0; i < pass_len; i++) {
        unsigned char c = (unsigned char)passphrase[i];

        // "modifier l'octet courant de la clé en faisant le XOR avec l'octet de l'utilisateur"
        key[key_idx] ^= c;

        // "modifier l'octet suivant ... somme"
        int next_idx = (key_idx + 1) % 16;
        key[next_idx] += c;

        // "modifier l'octet précédent ... produit"
        int prev_idx = (key_idx - 1 + 16) % 16;
        key[prev_idx] *= c;

        // "on déplace l'index de la clé de manière cyclique"
        key_idx = (key_idx + 1) % 16;
    }
}

int API_cipher(const char* pIn, const char* pOut) {
    // 1. Ouvrir le fichier d'entrée
    FILE *fin = fopen(pIn, "rb");
    if (!fin) { perror("Error opening input file"); return 0; }

    // 2. Générer une clé aléatoire de 16 octets (File Key)
    unsigned char file_key[16];
    srand(time(NULL));
    for(int i=0; i<16; i++) file_key[i] = rand() % 256;

    // 3. Demander la passphrase et générer la Master Key
    char passphrase[256];
    printf("Enter encryption passphrase (min 16 chars recommended): ");
    if (fgets(passphrase, sizeof(passphrase), stdin)) {
        passphrase[strcspn(passphrase, "\n")] = 0;
    }
    
    unsigned char master_key[16];
    generate_master_key(passphrase, master_key);

    // 4. Chiffrer la File Key avec la Master Key
    unsigned char encrypted_file_key[16];
    for(int i=0; i<16; i++) {
        encrypted_file_key[i] = file_key[i] ^ master_key[i];
    }

    // 5. Ouvrir le fichier de sortie
    FILE *fout = fopen(pOut, "wb");
    if (!fout) { fclose(fin); perror("Error opening output file"); return 0; }

    // 6. Écrire la clé chiffrée en en-tête
    fwrite(encrypted_file_key, 1, 16, fout);

    // 7. Chiffrer le contenu du fichier avec la File Key
    int c;
    int k_idx = 0;
    while ((c = fgetc(fin)) != EOF) {
        unsigned char ch = (unsigned char)c;
        ch ^= file_key[k_idx];
        fputc(ch, fout);
        k_idx = (k_idx + 1) % 16;
    }

    fclose(fin);
    fclose(fout);
    printf("File encrypted successfully to %s\n", pOut);
    return 1;
}

int API_decipher(const char* pIn, const char* pOut) {
    // 1. Ouvrir fichier chiffré
    FILE *fin = fopen(pIn, "rb");
    if (!fin) { perror("Error opening encrypted file"); return 0; }

    // 2. Lire l'en-tête (clé chiffrée)
    unsigned char encrypted_file_key[16];
    if (fread(encrypted_file_key, 1, 16, fin) != 16) {
        printf("Error: File too short.\n");
        fclose(fin);
        return 0;
    }

    // 3. Demander passphrase et générer Master Key
    char passphrase[256];
    printf("Enter decryption passphrase: ");
    if (fgets(passphrase, sizeof(passphrase), stdin)) {
        passphrase[strcspn(passphrase, "\n")] = 0;
    }

    unsigned char master_key[16];
    generate_master_key(passphrase, master_key);

    // 4. Déchiffrer la File Key
    unsigned char file_key[16];
    for(int i=0; i<16; i++) {
        file_key[i] = encrypted_file_key[i] ^ master_key[i];
    }

    // 5. Ouvrir fichier sortie
    FILE *fout = fopen(pOut, "wb");
    if (!fout) { fclose(fin); perror("Error opening output file"); return 0; }

    // 6. Déchiffrer le contenu
    int c;
    int k_idx = 0;
    while ((c = fgetc(fin)) != EOF) {
        unsigned char ch = (unsigned char)c;
        ch ^= file_key[k_idx];
        fputc(ch, fout);
        k_idx = (k_idx + 1) % 16;
    }

    fclose(fin);
    fclose(fout);
    printf("File decrypted successfully to %s\n", pOut);
    return 1;
}

// Pour compatibilité avec les autres fichiers si prototypes manquants
int save_promotion_binary(const char *filename, Prom *p);
void load_promotion_binary(char* filename, Prom* p);