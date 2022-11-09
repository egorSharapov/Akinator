#define TX_USE_SPEAK
#include <C:\TX\TXlib.h>
#include <assert.h>
#include <string.h>
#include "tree.hpp"
#include "akinator_lib.hpp"
#include "file_analize.hpp"
#include "stack\stack_functions.hpp"

//--------------------------------------------------------------------------------------------------------------------

static void safe_scanf (char *arg)
{
    if (!scanf (" %[^\n]", arg))
    {
        printf ("scanf error");
    }
    else
    {
    size_t end_ind = strlen (arg) - 1;

    while (arg[end_ind--] == ' ')
        arg[end_ind + 1] = '\0';
    }
}

//--------------------------------------------------------------------------------------------------------------------

void main_game (Root *tree_root)
{
    assert (tree_root);

    int cmd_number = 0;

    txSpeak ("\a\v\aПривет, это акинатор!\n");
    for (;;)
    {
        txSpeak ("\a\v\n\nЯ умею выполнять команды:\n [1]Угадывание\n [2]Определение\n [3]Сравнение\n"   \
                                               " [4]Вывод дерева \n [5]Выход\n [6]Выход с сохранением\n" \
                "\a\vЯ хочу (введи номер команды):\n");

        while (!scanf ("%d", &cmd_number))
        {
            fflush(stdin);
            txSpeak ("\a\vОшибка, попробуй еще раз\n");
        }
        switch (cmd_number)
        {
        case PREDICTION:
        {
            prediction_mode (tree_root);
            break;
        }
        case DEFINITION:
        {
            definition_mode (tree_root);
            break;
        }
        case COMPARSION:
        {
            comparsion_mode (tree_root);
            break;
        }
        case TREEPRINT:
        {
            show_tree (tree_root);
            break;
        }
        case EXIT:
        {
            txSpeak ("\a\v Прощай");
            save_tree ("out\\tree_output.txt", tree_root);
            return;
        }
        case EXITWITHSAVE:
        {
            txSpeak ("\a\v Прощай");
            save_tree ("out\\tree_input.txt", tree_root);
            save_tree ("out\\tree_output.txt", tree_root);
            return;
        }
        default:
        {
            txSpeak ("\a\vУпс, кажется, такой команды не существует...\n");
            break;
        }
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------

static void print_question (Tree_node *node)
{
    assert (node);

    txSpeak ("\a\vПерсонаж %s?\n", node->value);
}

//--------------------------------------------------------------------------------------------------------------------

static void add_answer (Tree_node* parent_node, const char *s_answer, char *s_attribute)
{
    assert (parent_node);
    assert (s_answer);
    assert (s_attribute);

    const char *buffer = parent_node->value;
    
    parent_node->value = s_attribute;
    parent_node->right = add_node (parent_node->right, s_answer);
    parent_node->right->parent = parent_node;
    parent_node->left = add_node (parent_node->left, buffer);
    parent_node->left->parent = parent_node;
}

//--------------------------------------------------------------------------------------------------------------------

static Tree_node* ask_question (Stack_t *inaccuracity, Tree_node* parent_node, char *s_answer)
{
    while (parent_node->left and parent_node->right)
    {
        print_question (parent_node);
        safe_scanf (s_answer);

        if (strcmp (s_answer, "да") == 0)
            parent_node = parent_node->right;
        else if (strcmp (s_answer, "нет") == 0)
            parent_node = parent_node->left;
        else if (strcmp (s_answer, "не знаю") == 0)
        {
            Node_definition *temp_node = (Node_definition *) calloc (1, sizeof (Node_definition));
            if (parent_node->right->right)
            {
                temp_node->node = parent_node->left;
                parent_node     = parent_node->right;
            }
            else
            {
                temp_node->node = parent_node->right;
                parent_node     = parent_node->left;
            }  
            stack_push (inaccuracity, temp_node);
        }
        else if (strcmp (s_answer, "скорее да") == 0)
        {

        }
        else 
        {
            txSpeak ("\a\vя тебя не понимаю... :(\n");
            printf ("доступные команды (да, нет, не знаю)\n");
        }
            
    }
    return parent_node;
}

static void add_definition (Root *tree_root, Tree_node *parent_node, char *s_answer, char *s_attribute)
{
    txSpeak ("\a\vХочешь добавить определение?\n");
    safe_scanf (s_answer);
        
    if (strcmp (s_answer, "нет") == 0)
        return;

    fflush (stdin);
    txSpeak ("\a\vЧто ты загадал?\n");
    safe_scanf (s_answer);

    if (find_node_by_value (tree_root->first_node, s_answer))
    {
        txSpeak ("\a\vТакой объект уже есть :(");
        return;
    }
        
    txSpeak ("\a\vЧем %s отличается от [%s]?\n", s_answer, parent_node->value);
    safe_scanf (s_attribute);
    txSpeak ("\a\vВ следующий раз я обязательно отгадаю :)\n");
        
    add_answer (parent_node, s_answer, s_attribute);

    fflush (stdin);
}
static void prediction_loop (Stack_t *inaccuracity, Root *tree_root, Tree_node *parent_node, char *s_answer, char *s_attribute)
{

    if (!parent_node->left and !parent_node->right)
    {
        txSpeak ("\vЯ думаю, это...");
        txSpeak ("\v%s\n", parent_node->value);

        if (inaccuracity->size == 0)
        {
            txSpeak ("\a\vЯ угадал?\n");
            safe_scanf (s_answer);
            if (strcmp (s_answer, "нет") == 0)
                add_definition (tree_root, parent_node, s_answer, s_attribute);
            else
                txSpeak ("\a\vКакой я молодец!");
        }
    }
    else if (strcmp (s_answer, "нет") == 0 and inaccuracity->size == 0)
    {
        add_definition (tree_root, parent_node, s_answer, s_attribute);
    }
    if (inaccuracity->size)
    {
        txSpeak ("\a\vЭто то, что ты загадал?\n");
        safe_scanf (s_answer);
        
        if (strcmp (s_answer, "да") == 0)
        {
            txSpeak ("\a\vЯ так и знал!");
            return;
        }
            
        
        txSpeak ("\a\vВычисляю вероятности... \n");
        parent_node = stack_pop (inaccuracity) -> node;

        parent_node = ask_question (inaccuracity, parent_node, s_answer);
        prediction_loop (inaccuracity, tree_root, parent_node, s_answer, s_attribute);
    }
}

void prediction_mode (Root *tree_root)
{
    assert (tree_root);
    
    char *s_answer    = (char *) calloc (sizeof (char), max_cmd_size);
    char *s_attribute = (char *) calloc (sizeof (char), max_cmd_size);
    
    if (!s_answer or !s_attribute)
        printf ("calloc error\n");

    Stack_t inaccuracity = {};
    stack_ctor (inaccuracity, 2);
    Tree_node *parent_node = tree_root->first_node;

    parent_node = ask_question (&inaccuracity, parent_node, s_answer);


    prediction_loop (&inaccuracity, tree_root, parent_node, s_answer, s_attribute);
}

//--------------------------------------------------------------------------------------------------------------------

static void push_tree_to_stack (Stack_t *stack, Tree_node *answer_node)
{
    assert (stack);
    assert (answer_node);

    while (answer_node->parent)
    {
        Node_definition* new_node = (Node_definition*) calloc (1, sizeof (Node_definition));
        new_node->node = answer_node->parent;
        new_node->answer   = "";

        if (answer_node->parent->left == answer_node)
            new_node->answer = "не";
            
        stack_push (stack, new_node);
        answer_node = answer_node->parent;
    }
}

//--------------------------------------------------------------------------------------------------------------------

static void print_tree_from_stack (Stack_t *stack)
{
    assert (stack);

    Node_definition *attribute = NULL;

    while (stack->size)
    {
        attribute = stack_pop (stack);
        txSpeak ("\a\v%s %s", attribute->answer, attribute->node->value);
        if (stack->size) txSpeak ("\a\v, ");
    }
}

//--------------------------------------------------------------------------------------------------------------------

void definition_mode (Root *tree_root)
{
    assert (tree_root);

    Stack_t stack = {};
    stack_ctor (stack, 2);
    
    char s_answer[max_cmd_size] = {0};

    txSpeak ("\a\vВведи имя объекта: ");
    scanf (" %[^\n]", s_answer);

    Tree_node *answer_node = find_node_by_value (tree_root->first_node, s_answer);

    if (answer_node)
    {
        txSpeak ("\a\v%s это... ", s_answer);
        push_tree_to_stack (&stack, answer_node);
        print_tree_from_stack (&stack);
    }
    else
        txSpeak ("\a\vОбъект не найден :(\n");

    stack_dtor (&stack);
}

//--------------------------------------------------------------------------------------------------------------------

void comparsion_mode (Root *tree_root)
{
    Stack_t stack1 = {};
    stack_ctor (stack1, 2);

    Stack_t stack2 = {};
    stack_ctor (stack2, 2);

    char s_answer1[max_cmd_size] = {0};
    char s_answer2[max_cmd_size] = {0};

    fflush (stdin);

    txSpeak ("\a\vВведи первый объект для сравнение: ");
    safe_scanf (s_answer1);
    Tree_node *answer_node1 = find_node_by_value (tree_root->first_node, s_answer1);
    
    txSpeak ("\a\vВведи второй объект для сравнения: ");
    safe_scanf (s_answer2);
    Tree_node *answer_node2 = find_node_by_value (tree_root->first_node, s_answer2);

    if (answer_node1 and answer_node2)
    {
        push_tree_to_stack (&stack1, answer_node1);
        push_tree_to_stack (&stack2, answer_node2);

        txSpeak ("\a\v%s и %s: ", answer_node1->value, answer_node2->value);
        
        Node_definition *answer1 = stack_pop (&stack1);
        Node_definition *answer2 = stack_pop (&stack2);

        if (answer1->answer != answer2->answer)
            txSpeak ("\a\vничего общего :(");

        while ((answer1->answer == answer2->answer) and (answer1->node->value == answer2->node->value))
        {
            txSpeak ("\a\v%s %s, ", answer1->answer, answer1->node->value);
            answer1 = stack_pop (&stack1);
            answer2 = stack_pop (&stack2);
        }

        stack_push (&stack1, answer1);
        stack_push (&stack2, answer2);

        txSpeak ("\a\v\nно %s: ", s_answer1);
        print_tree_from_stack (&stack1);
        
        txSpeak ("\a\v\na %s: ", s_answer2);
        print_tree_from_stack (&stack2);
    }
    else
    {
        txSpeak ("\a\vодин из объектов не найден... :(");
    }

    stack_dtor (&stack1);
    stack_dtor (&stack2);
}
