/**
 * @file save.h
 * @brief Sauvegarde/Chargement
 */
#ifndef SAVE_H
#define SAVE_H
#include <stdlib.h>
#include "struct.h"

int save_promotion_binary(const char *filename, Prom *p);
Prom* load_promotion_binary(const char* filename);

#endif