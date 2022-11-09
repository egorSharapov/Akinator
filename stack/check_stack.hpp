#ifndef CHECK_STACK_HPP_INCLUDED
#define CHECK_STACK_HPP_INCLUDED

#include <stdlib.h>
#include "stack.hpp"
#include "stack_functions.hpp"

#define stack_dump(output_file, Stack, mode) \
                          stack_dump_(output_file, Stack, mode, __FUNCTION__, __FILE__, __LINE__)

#define assert_ok(stack)  unsigned long errors = stack_ok (stack);    \
                          if (errors != 0)                            \
                          {                                           \
                              print_stack_error (output_file, errors);             \
                              stack_dump (output_file, stack, FULL);  \
                              exit(errors);                           \
                          }

enum CANARIES
{
    LEFT_DATA_CANARY  = 0xBAAD,
    RIGHT_DATA_CANARY = 0xDEAD,
    LEFT_CANARY       = 0xFAAD,
    RIGHT_CANARY      = 0x0BAD,
};


enum ERRORS
{
    CAPACITY_SIZE_ERROR     = 0,
    DATA_POINTER_ERROR      = 1,
    ZERO_CAPACITY_ERROR     = 2,
    LEFT_CANARY_ERROR       = 3,
    RIGHT_CANARY_ERROR      = 4,
    LEFT_DATA_CANARY_ERROR  = 5,
    RIGHT_DATA_CANARY_ERROR = 6,
    HASH_ERROR              = 7,
    NULL_POINTER_ERROR      = 8,
};


enum DUMP_MODE
{
    FULL  = 0,
    SHORT = 1,
    NAME  = 2,
};


void   print_stack_data       (FILE *file, int index, int stack_data);
void   print_stack_data       (FILE *file, int index, double stack_data);
void   stack_dump_            (FILE *file, Stack_t *stack, DUMP_MODE mode, const char *func_name, const char *file_name, const int line);
void   print_stack_error      (FILE *dump_file, unsigned long errors_code);
int    calculate_hash         (void * obj, size_t size);
void   calculate_stack_hash   (Stack_t * stack);
bool   check_stack_hash       (Stack_t *stack);
int    update_stack_hash      (Stack_t *stack);
int    update_data_hash       (Stack_t *stack); 
void   open_log               (const char * file_name);
void   close_log              (void);
Elem_s *create_data_canary    (Stack_t * stack);
Elem_s *recreate_data_canary  (Stack_t * stack, size_t new_capacity);

unsigned short data_canary_ok (Stack_t *stack);
unsigned short canary_ok      (Stack_t *stack);
unsigned long  stack_ok       (Stack_t *stack);

extern FILE *output_file;

#endif
