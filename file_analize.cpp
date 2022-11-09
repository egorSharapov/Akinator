#include <ctype.h>
#include <sys\stat.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include "file_analize.hpp"
#include "akinator_lib.hpp"

//--------------------------------------------------------------------------------------------------------------------

static size_t file_size_count (const char * file_name)
{
    assert (file_name);

    struct stat buff = {};
    buff.st_size = 0;

    stat(file_name, &buff);

    return buff.st_size;
}

//--------------------------------------------------------------------------------------------------------------------

void counting_strings (Text * text)
{
    assert (text);

    text->source [text->count_of_symbols] = '\0';
    
    char * point = text->source;
    
    while ((point = strchr (point, '\n')) != NULL)
    {
        if (*(point - 1) == 13)
            *(point - 1) = ' ';
        
        *point = ' ';
        point++;
        text->count_of_strings++;
    }
    text->count_of_strings++;

}

//--------------------------------------------------------------------------------------------------------------------

ERRORS_CODE count_and_read (const char *file_name, Text * text)
{
    assert (text);
    assert (file_name);

    FILE * input_file = fopen (file_name, "rb");

    if (!input_file)
        return FALL;

    size_t file_size = file_size_count (file_name);
    
    text->source = (char *) calloc ((file_size + 2), sizeof (char));

    if (text->source == NULL)
        return NO_MEM_ERROR;
    
    text->count_of_symbols = fread (text->source, sizeof(char), file_size, input_file);

    if (file_size != text->count_of_symbols)
        return FALL;

    counting_strings (text);
    
    if (fclose (input_file) != 0)
    {
        printf ("close file %s error", file_name);
        return FALL;
    }

    return SUCCESS;
}

//--------------------------------------------------------------------------------------------------------------------

void create_pointers (Text * text)
{
    assert (text);

    text->meta_string = (String *) calloc (text->count_of_strings + 1, sizeof (String));
    size_t index_of_string = 0;
    char * point = text->source;

    text->meta_string [index_of_string].string_point = point;
    while (index_of_string++ < text->count_of_strings)
    {
        point += strlen (point);
        point += 1;
        text->meta_string [index_of_string].string_point = point;
        text->meta_string [index_of_string - 1].string_len = point - text->meta_string [index_of_string - 1].string_point - 2;
    }

}

//--------------------------------------------------------------------------------------------------------------------

void save_tree (const char* file_name, Root* tree_root)
{
    FILE *output_file = fopen (file_name, "wb");
    if (!output_file)
        printf ("open file error");

    int height = 0;
    save_node (output_file, tree_root->first_node, height);

    if (fclose (output_file) != 0)
        printf ("close file error");
}

//--------------------------------------------------------------------------------------------------------------------

void save_node (FILE* output_file, Tree_node* node, int height)
{
    if (node)
    {
        fprintf (output_file, "%-*s{ \"%s\" ", height, "", node->value);
        
        if (node->right)
            fprintf (output_file, "\n");
        
        height += 4;
        save_node (output_file, node->right, height);
        save_node (output_file, node->left, height);

        if (node->right)
            fprintf (output_file, "%-*s", height - 4, "");

        fprintf (output_file, "}\n");
    }
}

//--------------------------------------------------------------------------------------------------------------------

static int clear_scanf (const char* source, char *value)
{
    source = strchr (source, '\"');

    if (!source)
        printf ("syntax error\n");

    return sscanf (source + 1, "%[^\"]", value);
}

//--------------------------------------------------------------------------------------------------------------------

void read_t_file (const char *t_file_name, Root *tree_root)
{
    Text text = {};

    count_and_read (t_file_name, &text);
    const char *temp_source = text.source;
    if (strchr (text.source, '{'))
    {
        temp_source = strchr (text.source, '{');

        if (!temp_source)
            printf ("syntax error");
        
        char *value = (char *) calloc (sizeof (char), max_cmd_size);

        if (!value)
            printf ("no memory");

        clear_scanf (temp_source + 1, value);
        tree_root->first_node = add_node (NULL, value);
        read_node (temp_source + 1, tree_root->first_node);
    }
}

//--------------------------------------------------------------------------------------------------------------------

static const char* read_leather (const char* source, Tree_node *parent, Tree_node** son)
{
    assert (source);
    assert (parent);
    assert (son);
    
    source = strchr (source, '{') + 1;

    char *value = (char *) calloc (sizeof (char), max_cmd_size);
    
    if (!value)
        printf ("no memory");

    clear_scanf (source, value);
    *son = add_node (parent, value);
    if (strchr (source, '{') < strchr (source, '}'))
        source = read_node (source, *son);

    return source;
}

//--------------------------------------------------------------------------------------------------------------------

const char *read_node (const char *source, Tree_node* parent)
{
    assert (source);
    assert (parent);

    if (strchr (source, '{'))
        source = read_leather (source, parent, &parent->right);

    if (strchr (source, '{') > strchr (source, '}'))
        source = read_leather (source, parent, &parent->left);

    return source;

}

//--------------------------------------------------------------------------------------------------------------------

char * create_dir_name (const char *file_path, const char *file_name)
{
    assert (file_path);
    assert (file_name);

    char *file_dir = (char *) calloc (strlen (file_path) + strlen (file_name) + 1, sizeof (char));

    if (file_dir == NULL)
        printf ("calloc error");

    strcpy (file_dir, file_path);
    strcat (file_dir, file_name);

    return file_dir;
}
