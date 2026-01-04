#include <stdio.h>
#include <math.h>

#include "../include/stack_funcs.h"
#include "../include/my_assert.h"

Errors stack_binary_op(Stack* stack, int code)
{
    MY_ASSERT(stack, "Error: stack is nullptr!\n");
    Errors err = verify_stack(stack);

    if (err != NO_ERRORS)
    {
        return err;
    }

    if (stack->size < 2)
    {
        return STACK_UNDERFLOW;
    }

    data_t a = pop_stack(stack, &err);
    data_t b = pop_stack(stack, &err);
    if (err != NO_ERRORS)
        return err;
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
            err = push_stack(stack, b);
            err = push_stack(stack, a);
            return err;

    }

    err = push_stack(stack, result);

    err = verify_stack(stack);
    return err;
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

    data_t a = pop_stack(stack, &err);
    if (err != NO_ERRORS)
        return err;

    if (a == 0)
    {
        err = push_stack(stack, a);
        err = (Errors)(err | STACK_DIV_BY_ZERO);
        return err;
    }

    data_t b = pop_stack(stack, &err);
    if (err != NO_ERRORS)
        return err;
    err = push_stack(stack, (b / a));

    err = verify_stack(stack);
    return err;
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

    data_t value = pop_stack(stack, &err);
    if (value < 0)
    {
        err = push_stack(stack, value);
        err = (Errors)(err | STACK_INVALID_ARGUMENT);
        return err;
    }

    err = push_stack(stack, (data_t)sqrt((double)value));
    err = verify_stack(stack);
    return err;
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
        err = (Errors)(err | STACK_UNDERFLOW);
        return err;
    }

    data_t value = pop_stack(stack, &err);
    if (err != NO_ERRORS)
        return err;
    printf("%d\n", value);

    err = verify_stack(stack);
    return err;
}


