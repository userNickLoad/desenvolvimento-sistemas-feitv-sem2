#include "data.h"
#include "../../btree.h"

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


unsigned int append_idx(FILE *fl, Node *crr_node, unsigned int *amount){
    if (crr_node == NULL){
        return 0;
    }

    unsigned int left = append_idx(fl, crr_node->left, amount);
    unsigned int right = append_idx(fl, crr_node->right, amount);

    Video_idx_name *crr_v = (Video_idx_name *)crr_node->value;

    crr_v->height = crr_node->height;
    crr_v->left = left;
    crr_v->right = right;

    fseek(fl, 0, SEEK_END);


    fprintf(fl, "%010u;", crr_v->prt_original);

    char name_b[51];
    ajust_info(name_b, crr_v->value, 51, ';');
    fwrite(name_b, sizeof(char), 51, fl);

    fprintf(fl, "%010u;%010u;%010u\n", crr_v->height, crr_v->left, crr_v->right);

    // fseek(fl, 0, SEEK_SET);
    // fprintf(fl, "%010u", amount + 1);

    // fclose(fl);

    *amount += 1;

    return *amount;
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
    Video_idx_name *video1 = (Video_idx_name *)vid1;
    Video_idx_name *video2 = (Video_idx_name *)vid2;
   
    int ord = is_alfab(video1->value, video2->value, sizeof(video1->value));
    return ord;
}

void *func()
{

    db_file(Video, "r")

    // db_file(Video_idx_name, "r+")

    // Video *vids = malloc(sizeof(Video) * Video_amount);
    Video_idx_name *vids_idx = malloc(sizeof(Video_idx_name) * Video_amount);

    fseek(fl_Video, HEARDER_SIZE, SEEK_SET);

    for (int i = 0; i < Video_amount; i++)
    {   
        vids_idx[i].height = 0;
        vids_idx[i].left = 0;
        vids_idx[i].prt_original = i;
        vids_idx[i].right = 0;
        Video vids;
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vids.id, vids_idx[i].value, vids.desc, &vids.duration, &vids.likes, &vids.dislikes);
    }

    Avl *tree = init_btree(name_strat, free_name);
    printf("Sem organizacao: \n\n");

    for (int i = 0; i < Video_amount; i++)
    {
        printf("Titulo: %s\n", vids_idx[i].value);
        add_node(tree, &vids_idx[i]);
    }
    printf("\n\n\nCom organizacao %d: \n\n", tree->elements);

    show_tree(tree->head);

    FILE *fl = fopen("data/files/Video_idx_name.txt", "r+");
    int amount, ln_size, h;
    fscanf(fl, IDX_HEADER_MASK, &amount, &ln_size, &h);

    unsigned int head = append_idx(fl, tree->head, &amount);
    fseek(fl, 0, SEEK_SET);
    fprintf(fl, "%010u;%010u;%010u\n", amount, ln_size, head);
    
    fclose(fl);

    

    // printf("Foi")

    // 0000000000;                                                  ;0000000000;0000000000;0000000000

    free_btree(tree);

    free(vids_idx);
    
    fclose(fl_Video);
    // fclose(fl_Video_idx_name);
}

int main()
{
    func();
    return 0;
}
