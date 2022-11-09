#include <windows.h>
#include "file_analize.hpp"
#include "akinator_lib.hpp"
#include "tree.hpp"

int main () 
{
    system ("chcp 1251");
    
    Root my_tree = {};

    read_t_file ("out\\tree_input.txt", &my_tree);

    main_game (&my_tree);

    free_node (my_tree.first_node);

    return 0;
}