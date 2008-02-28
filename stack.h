#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

typedef void* stack_elt_t;

typedef struct stack_node_s
{
    struct stack_node_s * prev;
    stack_elt_t val;
} stack_node_t;

typedef struct
{
    stack_node_t * top;
    size_t size;
} stack_t;


stack_t * create_stack();
void stack_free(stack_t * s, int free_elt);

stack_elt_t stack_pop(stack_t * s);

void stack_push(stack_t * s, stack_elt_t v);

#define stack_top(s) ((s)->top->val)

#define stack_size(s) ((s)->size)

#define stack_empty(s) ((s)->top == NULL)

#endif
