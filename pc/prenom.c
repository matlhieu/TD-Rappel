#include "prenom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **listePrenoms(const Prom *p, int *taille) {
    if (!p || !taille) return NULL;

    *taille = p->numStudent;
    if (*taille == 0) return NULL;

    size_t total_len = 0;
    for (int i = 0; i < p->numStudent; i++) {
        if (p->students[i].fname) {
            total_len += strlen(p->students[i].fname) + 1;
        }
    }

    if (total_len == 0) return NULL;

    char **tab = malloc(sizeof(char*) * p->numStudent);
    if (!tab) return NULL;

    char *block = malloc(sizeof(char) * total_len);
    if (!block) {
        free(tab);
        return NULL;
    }

    size_t offset = 0;
    for (int i = 0; i < p->numStudent; i++) {
        tab[i] = block + offset;
        const char *source = p->students[i].fname;

        int j = 0;
        if (source) {
            while (source[j] != '\0') {
                char c = source[j];
                if (c >= 'a' && c <= 'z') {
                    c = c - ('a' - 'A');
                }
                tab[i][j] = c;
                j++;
            }
        }
        tab[i][j] = '\0'; 
        offset += (j + 1);
    }

    return tab;
}