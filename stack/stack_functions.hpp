#ifndef STACK_FUNCTIONS_HPP_INCLUDED
#define STACK_FUNCTIONS_HPP_INCLUDED
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include "stack.hpp"



#define stack_ctor(Stack , size_t) stack_ctor_(&Stack, size_t, #Stack, __FUNCTION__, __FILE__, __LINE__)

void  *recalloc        (void *ptr, size_t num, size_t size);
void   stack_increase  (Stack_t *stk);
void   stack_push      (Stack_t *stk, Elem_s value);
Elem_s stack_pop       (Stack_t *stk, int *err = NULL);
void   stack_ctor_     (Stack_t *stack, size_t capacity, const char * stack_name,\
                                                         const char * func_name, const char * file_name, const int line);
void   fill_info       (Stack_t *stack, const char *func_name, const char *file_name, const char *stack_name, int line);
void   stack_dtor      (Stack_t *stack);
void   stack_fit       (Stack_t *stk);
void   stack_data_fill (int *stack_data);
void   stack_data_fill (double *stack_data);
void   stack_data_fill (Node_definition **stack_data);

extern FILE *output_file;

#endif
