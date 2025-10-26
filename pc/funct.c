#include "struct.h"

void grades_destroy(Grades *g) {
    if (!g) {
        return;
    }
    free(g->values);
    free(g);
}

int grades_add(Grades *g, double value) {
    if (!g){
        return -1;
    } 
    if (g->size + 1 > g->capacity) {
        size_t newcap = (g->capacity == 0) ? 4 : g->capacity * 2;
        double *tmp = realloc(g->values, newcap * sizeof(double));
        if (!tmp) return -1;
        g->values = tmp;
        g->capacity = newcap;
    }
    g->values[g->size++] = value;
    return 0;
}
