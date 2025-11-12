#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

typedef enum Instructions
{
    PUSH = 1,
    ADD = 2,
    SUB = 3,
    MUL = 4,
    DIV = 5,
    SQRT = 6,
    OUT = 7,
    PUSHR = 8,
    POPR = 9,
    PUSHM = 10,
    POPM = 11,
    JMP = 12,
    JB = 13,
    JBE = 14,
    JE = 15,
    JA = 16,
    JAE = 17,
    JNE = 18,
    CALL = 19,
    RET = 20,
    HLT = 0
} Instructions;

#endif
