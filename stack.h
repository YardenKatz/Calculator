#include <stdio.h>

#ifndef STACK_H
#define STACK_H

typedef struct stack my_stack_t;

/********************************************************/
/* Allocates memory for stack and create pointers       */
/* Returns NULL if allocation failed                    */
/********************************************************/
my_stack_t *StackCreate(size_t stack_size, size_t element_size);


/********************************************************/
/* Frees memory for stack                               */
/********************************************************/
void StackDestroy(my_stack_t *stack);


/********************************************************/
/* Inserts an item to stack                             */
/********************************************************/
void StackPush(my_stack_t *stack, void *item);


/********************************************************/
/* Removes last item from stack                         */
/********************************************************/
void StackPop(my_stack_t *stack);


/********************************************************/
/* Returns last item                                    */
/********************************************************/
void *StackPeek(my_stack_t *stack);

/********************************************************/
/* Returns number of items in stack                     */
/********************************************************/
size_t StackSize(my_stack_t *stack);
#endif




