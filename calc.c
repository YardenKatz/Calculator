/****************************************************************************/
/*                    Exercise: Calculatr                                   */
/*                    Date: 13.11.18                                        */
/*                    Author: Yarden Katz                                   */
/*                    Review: Inbal cartier                                 */
/****************************************************************************/

#include <stdlib.h>         /*    malloc      */
#include <stdio.h>          /*    printf      */
#include <string.h>         /*    strlen      */
#include <assert.h>         /*    assert      */
#include "./stack.h"        /*     stack      */
#include "calc.h"           /*   calculator   */

typedef double (*func)(double, double);
enum state {GETNUM, GETOPER};

static char num_LUT[256] = { 0 };
static char oper_LUT[256] = { 0 };
static func func_LUT[256] = { 0 };

static int Init();
static double Add(double num_1, double num_2);
static double Subtract(double num_1, double num_2);
static double Multiply(double num_1, double num_2);
static double Divide(double num_1, double num_2);
static int GetNextNum(char **str, int *next_state, my_stack_t *num_stack, 
                        my_stack_t *oper_stack);
static int GetNextOp(char **str, int *next_state, my_stack_t *num_stack, 
                        my_stack_t *oper_stack, int *status);
static void ComputeStatement(my_stack_t *num_stack, my_stack_t *oper_stack, 
                                int *status);

double Calculate(char *str, int *status)
{
    my_stack_t *num_stack = NULL;
    my_stack_t *oper_stack = NULL;
    size_t length = strlen(str);
    double ret = 0;
    char stub = '#';
    int next_state = GETNUM;

    assert(str);
    assert(status);

    num_stack = StackCreate(length, sizeof(double));
    if (NULL == num_stack)
    {
        *status = 1;
        return 0;
    }

    oper_stack = StackCreate(length, sizeof(char));
    if (NULL == num_stack)
    {
        StackDestroy(num_stack);
        *status = 1;
        return 0;
    }

    if (0 == num_LUT['0'])
    {
        Init();
    }

    if (*status != 0)
    {
        StackDestroy(num_stack);
        StackDestroy(oper_stack);
        return 0;
    }
    
    StackPush(oper_stack, &stub);
    while (*(char *)str != '\0')
    {
        if (GETNUM == next_state)
        {
            *status = GetNextNum(&str, &next_state, num_stack, oper_stack);
        }
        else
        {
            *status = GetNextOp(&str, &next_state, num_stack, oper_stack,   
                                status);
        }
        
        if (*status != 0)
        {
            StackDestroy(num_stack);
            StackDestroy(oper_stack);
            return 0;
        }
    }

    while('#' != *(char *)StackPeek(oper_stack))
    {
        ComputeStatement(num_stack, oper_stack, status);
    }
    ret = *(double *)StackPeek(num_stack);
    StackDestroy(num_stack);
    StackDestroy(oper_stack);
    
    return ret;    
}

static int Init()
{
    int i = 0;
    
    for(i = 0; i < 10; i++)
    {
        num_LUT[i + 48] = 1;
    }
    num_LUT['('] = 10;
    num_LUT['['] = 11;
    num_LUT['{'] = 12;

    oper_LUT['#'] = 1;
    oper_LUT['+'] = 2;
    oper_LUT['-'] = 2;
    oper_LUT['*'] = 3;
    oper_LUT['/'] = 3;
    oper_LUT['('] = 10;
    oper_LUT['['] = 11;
    oper_LUT['{'] = 12;
    oper_LUT[')'] = 20;
    oper_LUT[']'] = 22;
    oper_LUT['}'] = 24;
    
    func_LUT['+'] = Add;
    func_LUT['-'] = Subtract;
    func_LUT['*'] = Multiply;
    func_LUT['/'] = Divide;

    return 0;
}

static double Add(double num_1, double num_2)
{
    return num_1 + num_2;
}

static double Subtract(double num_1, double num_2)
{
    return num_1 - num_2;
}

static double Multiply(double num_1, double num_2)
{
    return num_1 * num_2;
}

static double Divide(double num_1, double num_2)
{
    return num_1 / num_2;
}

static int GetNextNum(char **str, int *next_state, my_stack_t *num_stack, 
                    my_stack_t *oper_stack)
{
    char *next_char = NULL;
    double next_num = 0;
    char current_char = num_LUT[(int)**str];

    if (0 == current_char)
    {
        return 1;
    }
    else if (current_char > 19)
    {
        *next_state = GETOPER;
    }
    else if (current_char > 9)
    {
        StackPush(oper_stack, *str);
        ++*str;
    }
    else
    {
        next_num = strtod(*str, &next_char);
        *str = next_char;
        StackPush(num_stack, &next_num);
        *next_state = GETOPER;
    }

    return 0;
}

static int GetNextOp(char **str, int *next_state, my_stack_t *num_stack, 
                        my_stack_t *oper_stack, int *status)
{
    char stack_head = *(char *)StackPeek(oper_stack);
    char next_item = **str;
    char open_brace = 0;

    if (0 == oper_LUT[(int)next_item])
    {
        return 1;
    }
    else
    {
        if (oper_LUT[(int)next_item] > 12)
        {
            open_brace = (oper_LUT[(int)next_item] / 2);
            while (open_brace != oper_LUT[(int)stack_head])
            {
                if (('#' == stack_head) || ((oper_LUT[(int)stack_head] > 9) && 
                    (oper_LUT[(int)stack_head] != open_brace)))
                {
                    return 1;
                }
                ComputeStatement(num_stack, oper_stack, status);
                stack_head = *(char *)StackPeek(oper_stack);
            }
            *next_state = GETOPER;
            StackPop(oper_stack);   
            ++*str;
        }
        else if (((oper_LUT[(int)next_item]) > (oper_LUT[(int)stack_head])) ||
                    (oper_LUT[(int)stack_head] > 9))    
        { 
            StackPush(oper_stack, *str);
            ++*str;
            *next_state = GETNUM;
        }
        else
        {
            while('#' != *(char *)StackPeek(oper_stack) && 
                    (oper_LUT[(int)*(char *)StackPeek(oper_stack)] < 9))
            {
                ComputeStatement(num_stack, oper_stack, status);
            } 
            StackPush(oper_stack, *str);
            ++*str;
            *next_state = GETNUM;
        }
    }
    
    return 0; 
}

static void ComputeStatement(my_stack_t *num_stack, my_stack_t *oper_stack, 
                                int *status)
{
    double num_1 = 0;
    double num_2 = 0;
    double new_value = 0;
    func operator = NULL;
    char next_op = *(char *)StackPeek(oper_stack);

    num_2 = *(double*)StackPeek(num_stack);
    StackPop(num_stack);
    
    if (('/' == next_op) && (0 == num_2)) 
    {   
        *status = 1;
    }
    
    num_1 = *(double*)StackPeek(num_stack);
    StackPop(num_stack);
    operator = func_LUT[(int)(next_op)];
    new_value = operator(num_1, num_2);
    StackPop(oper_stack);
    StackPush(num_stack, &new_value);
}
