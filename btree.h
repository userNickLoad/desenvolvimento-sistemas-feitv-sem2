#pragma once

typedef struct Avl
{
    struct Node *head;
    unsigned elements;
    void (*free_values)(void *value_prt);
    int (*compare_strat)(void *val1,void *val2);
} Avl;

typedef struct Node
{
    unsigned int height;
    void * value;
    struct Node *left;
    struct Node *right;

} Node;

Avl *init_btree(int (*compare_strat)(void *val1, void *val2), void (*free_values)(void *values_prt));

void free_node(Node *n, void (*free_values)(void *value_ptr));

void free_btree(Avl *tree);

Node *init_node(void *prt_value);

void *find_in_btree(Avl *tree, void *search);

// void node_h(Node *n);

Avl *add_node(Avl *tree, void *prt_value);
