#include "data.h"

Video *apend_video_idx_name(Video *vid){
    db_file(Video, "r+")}

Video *apend_video(Video *vid)
{
    db_file(Video, "r+")

        vid->id = Video_amount + 1;
    vid->likes = 0;
    vid->dislikes = 0;

    fseek(fl_Video, 0, SEEK_END);

    fprintf(fl_Video, "%010u;", vid->id);
    char name_b[51];
    ajust_info(name_b, vid->name, sizeof(name_b), ';');
    fwrite(name_b, sizeof(char), 51, fl_Video);

    char dec_b[251];
    ajust_info(dec_b, vid->desc, sizeof(dec_b), ';');
    fwrite(dec_b, sizeof(char), 251, fl_Video);

    fprintf(fl_Video, "%010u;%010u;%010u\n", vid->duration, 0, 0);

    fseek(fl_Video, 0, SEEK_SET);
    fprintf(fl_Video, HEADER_MASK, (Video_amount + 30), Video_line_size);

    fclose(fl_Video);
    return vid;
}

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

Node *palece_node(Node *crr_node, Node *new_node, int (*compare_strat)(void *val1, void *val2))
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
            crr_node->right = palece_node(crr_node->right, new_node, compare_strat);
            break;
        }
        if (!is_greater && crr_node->left != NULL)
        {
            crr_node->left = palece_node(crr_node->left, new_node, compare_strat);
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

    tree->head = palece_node(tree->head, new_node, tree->compare_strat);
}

void show_tree(Node *crr_node)
{
    if (crr_node == NULL)
        return;
    
    show_tree(crr_node->left);

    printf("Titulo: %s\n", ((Video *)crr_node->value)->name);

    show_tree(crr_node->right);

    
}

void free_name(void *name)
{
    return;
}

int name_strat(void *vid1, void *vid2)
{
    Video *video1 = (Video *)vid1;
    Video *video2 = (Video *)vid2;
   
    int ord = is_alfab(video1->name, video2->name, sizeof(video1->name));

    return ord;
}

void *func()
{

    db_file(Video, "r")

        db_file(Video_idx_name, "r+")

            Video *vids = malloc(sizeof(Video) * Video_amount);

    fseek(fl_Video, HEARDER_SIZE, SEEK_SET);

    for (int i = 0; i < Video_amount; i++)
    {
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vids[i].id, vids[i].name, vids[i].desc, &vids[i].duration, &vids[i].likes, &vids[i].dislikes);
    }

    Avl *tree = init_btree(name_strat, free_name);
    printf("Sem organizacao: \n\n");

    for (int i = 0; i < Video_amount; i++)
    {
        printf("Titulo: %s\n", vids[i].name);
        add_node(tree, &vids[i]);
    }
    printf("\n\n\nCom organizacao %d: \n\n", tree->elements);

    show_tree(tree->head);

    // printf("Foi")

    // 0000000000;                                                  ;0000000000;0000000000;0000000000

    free_btree(tree);

    free(vids);

    fclose(fl_Video);
    // fclose(fl_Video_idx_name);
}

int main()
{
    func();
    return 0;
}
