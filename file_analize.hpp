#ifndef FILE_ANALIZE_HPP_INCLUDED
#define FILE_ANALIZE_HPP_INCLUDED

#include <stdio.h>
#include "tree.hpp"


typedef struct
{
    char *string_point  = NULL; // pointer to a start of string
    size_t string_len      = 0;    // len of string
} String;


typedef struct
{
    char *source        = NULL;     // pointers massive
    size_t count_of_symbols = 0;    // count of symbols in source file
    size_t count_of_strings = 0;    // count of strings in source file
    String *meta_string  = NULL;
} Text;


enum ERRORS_CODE 
{
    FALL            = 0, // abnormal program termination
    SUCCESS         = 1, // succes
    ERRORS          = 2, // typical error
    INPUT_ERROR     = 3, // incorrect comand line input
    OPEN_FILE_ERROR = 4, // file cannot open
    NO_MEM_ERROR    = 5, // no mem for calloc 
};


void create_pointers  (Text * text);
void counting_strings (Text * text);
void save_tree (const char* file_name, Root* tree_root);
void save_node (FILE* input_file, Tree_node* node, int height);
char *create_dir_name (const char *file_path, const char *file_name);
void read_t_file (const char *t_file_name, Root *tree_root);
ERRORS_CODE count_and_read (const char *file_name, Text * text);
const char *read_node (const char *source, Tree_node* parent);

#endif