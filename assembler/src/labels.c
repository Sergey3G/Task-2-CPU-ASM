#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/labels.h"
#include "../include/my_assert.h"

void labels_init(LabelTable* table)
{
    table->data = NULL;
    table->size = 0;
    table->capacity = 0;
}

void labels_add(LabelTable* table, const char* name, size_t address)
{
    for (size_t i = 0; i < table->size; i++)
    {
        if (!strcmp(table->data[i].name, name))
        {
            printf("Duplicate label: %s\n", name);
            return;
        }
    }

    if (table->size == table->capacity)
    {
        size_t new_capacity = table->capacity ? 2 * table->capacity : 8;
        Label* new_data = (Label*)realloc(table->data, new_capacity * sizeof(Label));
        MY_ASSERT(new_data, "Error: memory reallocation failed!\n");
        table->data = new_data;
        table->capacity = new_capacity;
    }

    strncpy(table->data[table->size].name, name, sizeof(table->data[table->size].name) - 1);
    table->data[table->size].name[31] = '\0';
    table->data[table->size].address = address;
    table->size++;
}

const Label* label_find(LabelTable* table, const char* name)
{
    for (size_t i = 0; i < table->size; i++)
    {
        if (!strcmp(table->data[i].name, name))
            return &table->data[i];
    }
    return NULL;
}

void labels_free(LabelTable* table)
{
    free(table->data);
    table->data = NULL;
    table->size = 0;
    table->capacity = 0;
}
