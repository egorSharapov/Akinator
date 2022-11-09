#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "C:\Users\Egor\projects\libiconv-for-Windows-master\include\iconv.h"
#include "tree.hpp"

const int rus_n = 64;

const char *rus_to_eng[rus_n] = {"A", "B", "V", "G", "D", "E", "J", "Z", "I",      \
                                 "Y", "K", "L", "M", "N", "O", "P", "R", "S", "T",  \
                                 "U", "F", "h","C", "Ch", "Sh", "Sch", "\"", "Y",    \
                                 "\'", "E", "Yu", "Ya",                               \
                                 "a", "b", "v", "g", "d", "e", "j", "z", "i",      \
                                 "y", "k", "l", "m", "n", "o", "p", "r", "s", "t", \
                                 "u", "f", "h","c", "ch", "sh", "sch", "\"", "y",  \
                                 "\'", "e", "yu", "ya"};

//--------------------------------------------------------------------------------------------------------------------

Tree_node *add_node (Tree_node *parent, Elem_t value)
{
    Tree_node *son_node = (Tree_node *) calloc (1, sizeof (Tree_node));

    if (son_node == NULL)
        printf ("calloc node error");

    son_node->parent = parent;
    son_node->value = value;
    son_node->left = NULL;
    son_node->right = NULL;

    return (son_node);
}

//--------------------------------------------------------------------------------------------------------------------

static const char *convert_eng (const char* string)
{
    assert (string);

    size_t len = strlen (string);
    char *convert_string = (char *) calloc (len, 3*sizeof(char));

    if (!convert_string)
        printf ("calloc error\n");

    size_t i = 0;

    for (size_t index = 0; index < len; index++)
    {
        size_t rus_ind = rus_n + string[index];

        if (rus_ind <= rus_n)
        {
            size_t symbol_len = strlen (rus_to_eng[rus_ind]);

            while (symbol_len-- > 0)
                convert_string[i + symbol_len] = *(rus_to_eng[rus_ind] + symbol_len);   
       
            i += strlen (rus_to_eng[rus_ind]);
        } 
        else
        {
            convert_string[i] = string[index];
            i += 1;
        }
    }
    return convert_string;
}

//--------------------------------------------------------------------------------------------------------------------

static void print_val (FILE* graph_file, Tree_node *parent, Tree_node *son)
{
    assert (graph_file);

    if (son)
    {
        if (parent)
        {
            if (parent->right == son)
                fprintf (graph_file, "    \"%s\" -> \"%s\" [xlabel = yes, color = \"green\"]\n", convert_eng (parent->value), convert_eng (son->value));
            else 
                fprintf (graph_file, "    \"%s\" -> \"%s\" [xlabel = no, color = \"red\"]\n", convert_eng (parent->value), convert_eng (son->value));
        }
            
        print_val (graph_file, son, son->left);
        print_val (graph_file, son, son->right);
    }
}

//--------------------------------------------------------------------------------------------------------------------

void show_tree (Root *tree_root)
{
    assert (tree_root);

    FILE *graph_file = fopen ("out\\input_cp1251.gv", "wb");
    
    if (graph_file == NULL)
        printf ("open %s error", "out\\input.dot");

    fprintf (graph_file, "digraph {\n");
    fprintf (graph_file, "    labelloc = \"t\"\n");
    fprintf (graph_file, "    fontsize = \"70\"\n");
    fprintf (graph_file, "    nodesep = 0.4\n");
    fprintf (graph_file, "    node [shape = record]\n");
    fprintf (graph_file, "    splines=ortho\n    rankdir=TB\n");

    print_val (graph_file, NULL, tree_root->first_node);

    fprintf (graph_file, "}");
    
    if (fclose (graph_file) != 0)
        printf ("graph file close error");
    
    system ("dot out\\input_cp1251.gv -Tpng -o out\\output.png");

}

//--------------------------------------------------------------------------------------------------------------------

Tree_node *find_node_by_value (Tree_node *parent, Elem_t value)
{
    if (parent)
    {
        if (strcmp (parent->value, value) == 0)
            return parent;

        Tree_node* left = find_node_by_value (parent->left, value);
            
        if (left)
            return left;
        else 
            return find_node_by_value (parent->right, value);

    }
    
    return NULL;
}

//--------------------------------------------------------------------------------------------------------------------

void print_node (Tree_node *node)
{
    assert (node);

    printf ("\nparent = %p\n", node->parent);
    printf ("value = %s\n", node->value);
    printf ("left = %p\n", node->left);
    printf ("right = %p\n", node->right);
}

//--------------------------------------------------------------------------------------------------------------------

void free_node (Tree_node* node)
{
    if (node)
    {
        free_node (node->left);
        free_node (node->right);
        free (node);
    }
}

