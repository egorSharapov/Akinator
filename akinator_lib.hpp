#ifndef AKINATOR_LIB_HPP_INCLUDED
#define AKINATOR_LIB_HPP_INCLUDED

#include "tree.hpp"

typedef struct 
{
    Tree_node* node      = NULL;
    const char *answer   = NULL;
} Node_definition;


enum GAME_MODE
{
    PREDICTION   = 1,
    DEFINITION   = 2,
    COMPARSION   = 3,
    TREEPRINT    = 4,
    EXIT         = 5,
    EXITWITHSAVE = 6,
};


const int max_cmd_size = 128;

void main_game (Root *tree_root);
void prediction_mode (Root *tree_root);
void comparsion_mode (Root *tree_root);
void definition_mode (Root *tree_root);

#endif