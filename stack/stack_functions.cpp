#include "config.h"
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>


#ifdef DEBUG
    #include "check_stack.hpp"
#else
    #include "stack_functions.hpp"
#endif


void stack_ctor_ (Stack_t *stack, size_t capacity, const char * stack_name, const char * func_name, const char * file_name, const int line)
{
    assert (stack);

    stack->capacity = capacity;
    stack->size = 0;

    #ifdef CANARYCHECK
        stack->data = create_data_canary (stack);

        stack->left_canary = LEFT_CANARY;
        stack->right_canary = RIGHT_CANARY;
    #else
        stack->data = (Elem_s *) calloc (1,  stack->capacity*sizeof(Elem_s));
    #endif

    fill_info (stack, func_name, file_name, stack_name, line);
    
    for (unsigned index = 0; index < capacity; index++)
    {
        stack_data_fill (&stack->data[index]);
    }


    #ifdef HASHCHECK
        calculate_stack_hash (stack);
    #endif

    #ifdef DEBUG
        assert_ok (stack);
    #endif
}

void fill_info (Stack_t *stack, const char *func_name, const char *file_name, const char *stack_name, int line)
{
    stack->stack_info.func_name  = func_name;
    stack->stack_info.file_name  = file_name;
    stack->stack_info.stack_name = stack_name;
    stack->stack_info.line = line;
}

// void stack_data_fill (int *stack_data)
// {
//     assert (stack_data);

//     *stack_data = int_poison;
// }


// void stack_data_fill (double *stack_data)
// {
//     assert (stack_data);

//     *stack_data = NAN;
// }

void stack_data_fill (Node_definition **stack_data)
{
    assert (stack_data);

    *stack_data = {};
}

void stack_dtor (Stack_t *stack)
{
    #ifdef DEBUG
        assert_ok (stack);
    #endif
    
    stack->left_canary = -300;
    stack->right_canary = -300;

    stack->capacity = -1;
    stack->size = -2;
    
    stack->hash = int_poison;
    stack->data_hash = int_poison;
    
    free (stack->data);
    stack->data = NULL;

    stack = NULL;
}

void *recalloc(void *ptr, size_t num, size_t size)
{
    size_t temp_size = _msize(ptr);
    
    ptr = realloc (ptr, size*num);

    if (!ptr and (num*size > temp_size))
        memset ((char *) ptr + temp_size, 0, num*size - temp_size);

    return ptr;
}


void stack_increase (Stack_t *stack)
{

    
    if (stack->capacity == 0) 
        stack->capacity++;

    size_t new_capacity  = stack->capacity * multiple;
    #ifdef CANARYCHECK
    stack->data = recreate_data_canary (stack, new_capacity);
    #else
    stack->data = (Elem_s*) recalloc (stack->data,new_capacity*sizeof (Elem_s), 1);
    #endif
    stack->capacity = new_capacity;

    for (size_t index = stack->size; index < stack->capacity; index++)
        stack_data_fill(&stack->data[index]);
}


void stack_push (Stack_t *stack, Elem_s value)
{
    #ifdef DEBUG
        assert_ok (stack);
    #endif

    if (stack->size >= stack->capacity)
        stack_increase (stack);

    stack->data[stack->size++] = value;

    #ifdef HASHCHECK
        calculate_stack_hash (stack);
    #endif
}


Elem_s stack_pop (Stack_t *stack, int *err)
{
    #ifdef DEBUG
        assert_ok (stack);
        
    #endif
    
    Elem_s value = {};

    if (err) *err = 1;

    if (stack->size > 0)
    {
        value = stack->data[--stack->size];
        stack_data_fill (&stack->data[stack->size]);
        stack_fit (stack);
        if (err) *err = 0;

        #ifdef HASHCHECK
            calculate_stack_hash (stack);
        #endif
    }


    return value;
}

void stack_fit (Stack_t *stack)
{
    #ifdef DEBUG
        assert (stack);
    #endif

    if (stack->size*multiple <= stack->capacity and stack->size != 0)
    {
        size_t new_capacity = stack->capacity / multiple;
        #ifdef CANARYCHECK
            stack->data = recreate_data_canary (stack, new_capacity);
        #else
            stack->data = (Elem_s*) recalloc (stack->data,new_capacity*sizeof (Elem_s), 1);
        #endif
        
        stack->capacity /= multiple;
    }
}
