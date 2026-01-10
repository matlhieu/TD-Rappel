/**
 @brief Exo 1 : Recalcul
*/

#ifndef RECALCUL_H
#define RECALCUL_H

#include "struct.h"

typedef void(*Recalcul)(Student *s, float min_promo, float max_promo);

int miseAJourNotes(Prom *p, Recalcul fct);

void normalisation(Student *s, float min_promo, float max_promo);

#endif
