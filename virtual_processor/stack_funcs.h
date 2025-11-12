#ifndef STACK_FUNCS_H
#define STACK_FUNCS_H

#include "stack.h"

Errors stack_binary_op(Stack* stack, int code);
Errors stack_div(Stack* stack);
Errors stack_sqrt(Stack* stack);
Errors stack_out(Stack* stack);

#endif

