#include <string.h> /* memmove */
#include <stdlib.h> /* malloc */
#include "./stack.h"

/********************************************************************/
/*                          DS Stack                                */
/*                       Author: Yarden Katz                        */
/*                       Date: 4.10.18                              */
/*                        Reviewed By:                              */
/********************************************************************/

struct stack
{
    void *head;
    void *current;
    void *end;
    size_t element_size;
};

my_stack_t *StackCreate(size_t stack_size, size_t element_size)
{
    my_stack_t *stack_list = (my_stack_t *)malloc(sizeof(my_stack_t));
    stack_list->head = malloc(stack_size * element_size);
    if (NULL == stack_list->head)
    {
        printf("failed to create stack\n");
        return NULL;
    }
     if (NULL == stack_list)
    {
        printf("failed to create stack\n");
        free(stack_list->head);
        return NULL;
    }
    
    stack_list -> current = stack_list->head;
    stack_list -> end =(void *)((size_t)stack_list->head + (stack_size * element_size)); 
    stack_list -> element_size = element_size;

    return stack_list;
}   

void StackDestroy(my_stack_t *my_stack)
{
    free(my_stack -> head);
    free(my_stack);
}

void StackPush(my_stack_t *my_stack, void *item)
{
    memmove(my_stack -> current, (const void *) item, my_stack -> element_size);
    (my_stack -> current) = (char *)(my_stack -> current) + my_stack -> element_size;  
}
void StackPop(my_stack_t *my_stack)
{
    (my_stack -> current) = (char *)(my_stack -> current) - my_stack -> element_size;
}

void *StackPeek(my_stack_t *my_stack)
{ 
    return (char *)my_stack -> current - my_stack -> element_size;
}

size_t StackSize(my_stack_t *my_stack)
{
    return ((char *)(my_stack -> current) - (char *)(my_stack -> head)) / my_stack -> element_size;
}


