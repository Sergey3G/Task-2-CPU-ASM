#ifndef LABELS_H
#define LABELS_H

#include <stdio.h>

typedef struct Label
{
    char name[32];
    size_t address;
} Label;

typedef struct LabelTable
{
    Label* data;
    size_t size;
    size_t capacity;
} LabelTable;

void labels_init(LabelTable* table);
void labels_add(LabelTable* table, const char* name, size_t address);
const Label* label_find(LabelTable* table, const char* name);
void labels_free(LabelTable* table);

#endif
