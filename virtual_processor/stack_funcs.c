#include <stdio.h>
#include <math.h>

#include "stack_funcs.h"

Errors stack_binary_op(Stack* stack, int code)
{
    Errors err = verify_stack(stack);

    if (err != NO_ERRORS)
    {
        return err;
    }

    if (stack->size < 2)
    {
        return STACK_UNDERFLOW;
    }

    data_t a = pop_stack(stack);
    data_t b = pop_stack(stack);
    data_t result = 0;

    switch (code)
    {
        case ADD: result = b + a;
                  break;
        case SUB: result = b - a;
                  break;
        case MUL: result = b * a;
                  break;
        default:
            printf("stack_binary_op: unknown operation code %d\n", code);
            push_stack(b);
            push_stack(a);
            return STACK_INVALID_ARGUMENT;

    }

    push_stack(stack, result);

    return verify_stack(stack);
}

Errors stack_div(Stack* stack)
{
    Errors err = verify_stack(stack);

    if (err != NO_ERRORS)
    {
        return err;
    }

    if (stack->size < 2)
    {
        return STACK_UNDERFLOW;
    }

    data_t a = pop_stack(stack);
    if (a == 0)
    {
        push_stack(stack, a);
        return STACK_DIV_BY_ZERO;
    }

    data_t b = pop_stack(stack);
    push_stack(stack, (data_t)(b / a));

    return verify_stack(stack);
}

Errors stack_sqrt(Stack* stack)
{
    Errors err = verify_stack(stack);
    if (err != NO_ERRORS)
    {
        return err;
    }

    if (stack->size < 1)
    {
        return STACK_UNDERFLOW;
    }

    data_t value = pop_stack(stack);
    if (value < 0)
    {
        push_stack(stack, value);
        return STACK_INVALID_ARGUMENT;
    }

    push_stack(stack, (data_t)sqrt((double)value));
    return verify_stack(stack);
}

Errors stack_out(Stack* stack)
{
    Errors err = verify_stack(stack);
    if (err != NO_ERRORS)
    {
        return err;
    }

    if (stack->size < 1)
    {
        return STACK_UNDERFLOW;
    }

    data_t value = pop_stack(stack);
    printf("%d\n", value);

    return verify_stack(stack);
}


