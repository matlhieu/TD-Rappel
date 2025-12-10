#ifndef SAVE_H
#define SAVE_H
#include <stdlib.h>

int save_promotion_binary(const char *filename, Prom *p);
void load_promotion_binary(char* filename, Prom* p);

#endif