/**
 * @file read_files.h
 * @brief lecture fichiers
 */
#ifndef READ_FILES_H
#define READ_FILES_H
#include "struct.h"

Prom *load_promotion_from_file(const char *filename);
void student_update_average(Student *s);

#endif