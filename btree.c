#include "header.h"
#include "btree.h" 

Avl *init_btree(int (*compare_strat)(void *val1, void *val2), void (*free_values)(void *values_prt))
{
    Avl *tree = malloc(sizeof(Avl));
    if (tree == NULL)
    {
        fprintf(stderr, "Erro ao tentar criar arvore");
        return NULL;
    }

    tree->elements = 0;
    tree->head = NULL;
    tree->compare_strat = compare_strat;
    tree->free_values = free_values;

    return tree;
}

void free_node(Node *n, void (*free_values)(void *value_ptr))
{
    if (n == NULL)
        return;

    free_node(n->left, free_values);
    free_node(n->right, free_values);

    free_values(n->value);

    free(n);

    return;
}

void free_btree(Avl *tree)
{
    free_node(tree->head, tree->free_values);
    free(tree);
}

Node *init_node(void *prt_value)
{
    Node *n = malloc(sizeof(Node));
    if (n == NULL)
    {
        fprintf(stderr, "Erro ao criar o node");
        return NULL;
    }

    n->value = prt_value;
    n->height = 0;
    n->left = NULL;
    n->right = NULL;

    return n;
}

void node_h(Node *n)
{
    int left_height = ((n->right != NULL) ? n->right->height : 0);
    int right_height = ((n->left != NULL) ? n->left->height : 0);
    n->height = ((left_height > right_height) ? left_height : right_height) + 1;
    return;
}

Node *rotation_ll(Node *crr_node)
{
    Node *above = crr_node;
    Node *middle = above->left;

    // preserva subárvore intermediária
    above->left = middle->right;

    // rotação
    middle->right = above;

    // atualiza alturas
    node_h(above);
    node_h(middle);

    return middle;
}

Node *rotation_rr(Node *crr_node)
{
    Node *above = crr_node;
    Node *middle = above->right;

    // preserva subárvore intermediária
    above->right = middle->left;

    // rotação
    middle->left = above;

    // atualiza alturas
    node_h(above);
    node_h(middle);

    return middle;
}

Node *rotation_lr(Node *crr_node)
{
    // primeiro roda RR no filho esquerdo
    crr_node->left = rotation_rr(crr_node->left);

    // depois LL no nó atual
    return rotation_ll(crr_node);
}

Node *rotation_rl(Node *crr_node)
{
    // primeiro roda LL no filho direito
    crr_node->right = rotation_ll(crr_node->right);

    // depois RR no nó atual
    return rotation_rr(crr_node);
}

Node *make_balance(Node *crr_node, int (*compare_strat)(void *val1, void *val2))
{
    // R-L
    if (crr_node->right != NULL && crr_node->right->left != NULL)
    {
        return rotation_rl(crr_node);
    }

    // L-R
    if (crr_node->left != NULL && crr_node->left->right != NULL)
    {
        return rotation_lr(crr_node);
    }

    // R-R
    if (crr_node->right != NULL && crr_node->right->right != NULL)
    {
        return rotation_rr(crr_node);
    }

    // L-L
    if (crr_node->left != NULL && crr_node->left->left)
    {
        return rotation_ll(crr_node);
    }

}

Node *place_node(Node *crr_node, Node *new_node, int (*compare_strat)(void *val1, void *val2))
{
    int is_greater = compare_strat(crr_node->value, new_node->value);

    do
    {
        if (is_greater && crr_node->right == NULL)
        {
            crr_node->right = new_node;
            break;
        }
        if (!is_greater && crr_node->left == NULL)
        {
            crr_node->left = new_node;
            break;
        }
        if (is_greater && crr_node->right != NULL)
        {
            crr_node->right = place_node(crr_node->right, new_node, compare_strat);
            break;
        }
        if (!is_greater && crr_node->left != NULL)
        {
            crr_node->left = place_node(crr_node->left, new_node, compare_strat);
            break;
        }
    } while (0);

    int left_height = ((crr_node->right != NULL) ? crr_node->right->height : 0);
    int right_height = ((crr_node->left != NULL) ? crr_node->left->height : 0);
    int balance = left_height - right_height;

    if (!(-1 <= balance && balance <= 1))
    {
        crr_node = make_balance(crr_node, compare_strat);
    }

    node_h(crr_node);

    return crr_node;
}

Avl *add_node(Avl *tree, void *prt_value)
{
    tree->elements++;
    Node *new_node = init_node(prt_value);

    if (new_node == NULL)
    {
        return NULL;
    }

    if (tree->head == NULL)
    {
        tree->head = new_node;
        return tree;
    }

    tree->head = place_node(tree->head, new_node, tree->compare_strat);
}

void *recursive_search(Avl *tree, Node *crr_node, void *search)
{
    int comp = tree->compare_strat(crr_node->value, search);
    if (comp == 2)
    {
        return crr_node->value;
    }

    if(comp == 1)
    {
        return recursive_search(tree, crr_node->right, search);
    }

    if(comp == 0)
    {
        return recursive_search(tree, crr_node->left, search);
    }
}

void *find_in_btree(Avl *tree, void *search)
{
    return recursive_search(tree, tree->head, search);
}
