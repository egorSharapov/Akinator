#ifndef TREE_HPP_INCLUDED
#define TREE_HPP_INCLUDED

#include <stdio.h>

typedef const char* Elem_t;

struct Tree_node
{
    Tree_node *parent = NULL;
    Elem_t value = "(free)";
    Tree_node *left  = NULL;
    Tree_node *right = NULL;

};


typedef struct
{
    size_t number_of_elem = 0;
    Tree_node *first_node = NULL;

} Root;

Tree_node *add_node (Tree_node *parent, Elem_t value);
Tree_node *find_node_by_value (Tree_node *parent, Elem_t value);

void show_tree (Root *tree_root);
void print_node (Tree_node *node);
void free_node (Tree_node* node);

#endif