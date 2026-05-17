#include "data.h"
#include "../../btree.h"

User *user_name_password(char *name, char *password)
{
    db_file(User, "r")

    User user,
    *res = NULL;

    for (int i = 0; i < User_amount; i++)
    {
        fscanf(fl_User, USER_SCAN_MASK, &user.id, user.name, user.password);
        
        if (compare_str(user.name, name))
        {
            if (compare_str(user.password, password))
            {
                res = malloc(sizeof(User));
                res->id = user.id;
                copy_str(res->name, user.name);
                copy_str(res->password, user.password);
            }
            break;
        }
    }
    fclose(fl_User);

    return res;
}

Response login(char *name, char *password)
{
    Response res;
    res.data = user_name_password(name, password);

    if (res.data == NULL)
    {
        res.code = 401;
        copy_str(res.msg, "\n\tNome ou senha devem estar incorretos.");
        return res;
    }

    res.code = 200;
    copy_str(res.msg, "\n\tUsuario logado.");
    return res;
}

User *user_by_name(char *name)
{
    db_file(User, "r")

        User user,
        *res = NULL;

    for (int i = 0; i < User_amount; i++)
    {

        fscanf(fl_User, USER_SCAN_MASK, &user.id, user.name, user.password);
        if (compare_str(user.name, name))
        {
            res = malloc(sizeof(User));
            res->id = user.id;
            copy_str(res->name, user.name);
            copy_str(res->password, user.password);

            break;
        }
    }
    fclose(fl_User);

    return res;
}

User *append_user(char *name, char *password)
{
    db_file(User, "r+")

    fseek(fl_User, 0, SEEK_SET);
    fprintf(fl_User, HEADER_MASK, (User_amount + 1), User_line_size);

    fseek(fl_User, 0, SEEK_END);
    fprintf(fl_User, "%010u;", User_amount);

    char name_s[51];
    ajust_info(name_s, name, sizeof(name_s), ';');
    fwrite(name_s, sizeof(char), 51, fl_User);

    char password_s[21];
    ajust_info(password_s, password, sizeof(password_s), '\n');
    fwrite(password_s, sizeof(char), 21, fl_User);

    fclose(fl_User);

    User *res = malloc(sizeof(User));

    res->id = User_amount;
    copy_str(res->name, name);
    copy_str(res->password, password);

    return res;
}

Response signup(char *name, char *password)
{
    Response res;

    User *user = user_by_name(name);

    if (user != NULL)
    {
        res.code = 400;
        res.data = NULL;
        sprintf(res.msg, "\n\tUsuario %s ja existe", name);
        free(user);
        return res;
    }

    user = append_user(name, password);

    if (user == NULL)
    {
        res.code = 500;
        res.data = NULL;
        copy_str(res.msg, "\n\tErro ao tentar criar o usuario");
        return res;
    }

    res.code = 200;
    res.data = user;
    copy_str(res.msg, "\n\tUsuario criado");

    return res;
}

// unsigned int append_idx(FILE *fl, Node *crr_node, unsigned int *amount){
//     if (crr_node == NULL){
//         return 0;
//     }

//     unsigned int left = append_idx(fl, crr_node->left, amount);
//     unsigned int right = append_idx(fl, crr_node->right, amount);

//     User_idx_name *crr_v = (User_idx_name *)crr_node->value;

//     crr_v->height = crr_node->height;
//     crr_v->left = left;
//     crr_v->right = right;

//     fseek(fl, 0, SEEK_END);


//     fprintf(fl, "%010u;", crr_v->prt_original);

//     char name_b[51];
//     ajust_info(name_b, crr_v->value, 51, ';');
//     fwrite(name_b, sizeof(char), 51, fl);

//     fprintf(fl, "%010u;%010u;%010u\n", crr_v->height, crr_v->left, crr_v->right);

//     // fseek(fl, 0, SEEK_SET);
//     // fprintf(fl, "%010u", amount + 1);

//     // fclose(fl);

//     *amount += 1;

//     return *amount;
// }

void show_tree(FILE *fl_user, FILE * fl_idx, unsigned int idx)
{   
    if (idx == 0)
    {
        return;
    }
    // printf("Idx: %u\n", (IDX_HEARDER_SIZE + 96 * (idx - 1)));
    fseek(fl_idx, (IDX_HEARDER_SIZE + 96 * (idx - 1)), SEEK_SET);
    User_idx_name user_idx_name;
    fscanf(fl_idx, VIDEO_IDX_NAME_SACN_MASK, &user_idx_name.prt_original, user_idx_name.value, &user_idx_name.height, &user_idx_name.left, &user_idx_name.right);
    // printf("left: %u; right: %u\n", user_idx_name.left, user_idx_name.right);
    show_tree(fl_user, fl_idx, user_idx_name.left);

    fseek(fl_user, (HEARDER_SIZE + 83 * user_idx_name.prt_original), SEEK_SET);
    User user;
    fscanf(fl_user, USER_SCAN_MASK, &user.id, user.name, user.password);
    printf(USER_PRINT_MASK, user.id, user.name, user.password);

    show_tree(fl_user, fl_idx, user_idx_name.right);

}

void free_name(void *name)
{
    return;
}

int name_strat(void *vid1, void *vid2)
{
    User_idx_name *User1 = (User_idx_name *)vid1;
    User_idx_name *User2 = (User_idx_name *)vid2;
   
    int ord = is_alfab(User1->value, User2->value, sizeof(User1->value));
    return ord;
}

void *func()
{

    db_file(User, "r")

    FILE *fl_User_idx_name = fopen("data/files/User_idx_name.txt", "r");
    unsigned int idx_name_amount, idx_name_ln_size, idx_name_head;
    fscanf(fl_User_idx_name, IDX_HEADER_MASK, &idx_name_amount, &idx_name_ln_size, &idx_name_head);

    // printf("Idx: %u\n", idx_name_head);

    show_tree(fl_User, fl_User_idx_name, idx_name_head);

    // // User *vids = malloc(sizeof(User) * User_amount);
    // User_idx_name *user_idx = malloc(sizeof(User_idx_name) * User_amount);

    // fseek(fl_User, HEARDER_SIZE, SEEK_SET);

    // for (int i = 0; i < User_amount; i++)
    // {   
    //     user_idx[i].height = 0;
    //     user_idx[i].left = 0;
    //     user_idx[i].prt_original = i;
    //     user_idx[i].right = 0;
    //     User user;
    //     fscanf(fl_User, USER_SCAN_MASK, &user.id, user_idx[i].value, user.password);
    // }

    // Avl *tree = init_btree(name_strat, free_name);
    // printf("Sem organizacao: \n\n");

    // for (int i = 0; i < User_amount; i++)
    // {
    //     printf("Titulo: %s\n", user_idx[i].value);
    //     add_node(tree, &user_idx[i]);
    // }
    // printf("\n\n\nCom organizacao %d: \n\n", tree->elements);

    // show_tree(tree->head);

    // FILE *fl = fopen("data/files/User_idx_name.txt", "r+");
    // int amount, ln_size, h;
    // fscanf(fl, IDX_HEADER_MASK, &amount, &ln_size, &h);

    // unsigned int head = append_idx(fl, tree->head, &amount);
    // fseek(fl, 0, SEEK_SET);
    // fprintf(fl, "%010u;%010u;%010u\n", amount, ln_size, head);
    
    // fclose(fl);

    

    // // printf("Foi")

    // // 0000000000;                                                  ;0000000000;0000000000;0000000000

    // free_btree(tree);

    // free(user_idx);
    
    fclose(fl_User);
    fclose(fl_User_idx_name);
    // fclose(fl_User_idx_name);
}

int main()
{
    func();
    return 0;
}
