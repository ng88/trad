#include "stack.h"
#include "assert.h"

stack_t * create_stack()
{
    stack_t * s = (stack_t*)malloc(sizeof(stack_t));

    c_assert2(s, "malloc failed");

    s->size = 0;
    s->top = NULL;

    return s;
}

void stack_free(stack_t * s, int free_elt)
{
    c_assert(s);

    while(!stack_empty(s))
    {
	if(free_elt)
	    free(stack_top(s));

	stack_pop(s);
    }

    free(s);
}

stack_elt_t stack_pop(stack_t * s)
{
    c_assert(s);
    
    stack_node_t * prev = s->top->prev;
    stack_elt_t elt = stack_top(s);

    free(s->top);

    s->top = prev;

    s->size--;

    return elt;
}

void stack_push(stack_t * s, stack_elt_t v)
{
    c_assert(s);

    stack_node_t * newn =
	(stack_node_t*)malloc(sizeof(stack_node_t));

    c_assert2(newn, "malloc failed");

    newn->val = v;

    newn->prev = s->top;
    s->top = newn;

    s->size++;
}


