#ifndef STACK_H
#define STACK_H

#include "errors.h"

#define CANARY 0xDEDDED

typedef int data_t;

typedef struct Stack
{
	data_t* data;
	size_t size;
	size_t capacity;
} Stack;

Errors construct_stack(Stack* stack, size_t stk_capacity);
void destruct_stack(Stack* stack);
Errors push_stack(Stack* stack, data_t value);
data_t pop_stack(Stack* stack, Errors* err);
Errors verify_stack(Stack* stack);
void stack_dump(Stack* stack);

#endif
