#include "../pages.h"
#include "../header.h"
#include "../schema.h"
#include "../lists.h"
#include "../data/source/data.h"

typedef struct Page_info
{
    char search_title[50];
    Video *videos;
} Page_info;


void request_vids(Page *this_p)
{   
    Page_info *info = this_p->data.payload;
    
    Response *res = this_p->data.response;
    if (res == NULL)
    {
        res = malloc(sizeof(Response));
        this_p->data.response = res;
        res->data = NULL;
    }

    if (info == NULL)
    {   
        info = malloc(sizeof(Page_info));
        info->search_title[0] = '\0';
        info->videos = NULL;
        this_p->data.payload = info;
    }
    

    char *title = (info->search_title[0] != '\0')? info->search_title: NULL;

    Response res_search = search_for_videos(title);
    
    copy_struct(res, &res_search, sizeof(Response));

    page_look_vids(this_p);
}

ChangePage selectFn_lv(Page *this_p, int lst_selected)
{
    return this_p->nxt[lst_selected];
}

void page_look_vids(Page *this_p) {

    Response *res = (Response *)this_p->data.response;
    Video *vids = (Video *)res->data;
    Page_info *info = (Page_info *)this_p->data.payload;

    char **ops = (char **)dina_prt_init(1 + dinamic_size(vids));
    ChangePage *nxt = dina_chPage_init(2);
    ChangePage *lst = malloc(sizeof(ChangePage));

    lst->free_all = NULL;
    lst->build = page_home;

    char ops1[85];
    sprintf(ops1, "Busque por filmes: %s", info->search_title);
    ops = add_opcao(ops1, ops, sizeof(ops1));

    for (int i = 0; i < sizeof(vids); i++)
    {
        add_opcao(vids[i].name, ops, sizeof(vids[i].name));
    }

    char *question = NULL;

    if (res->code != 200)
    {
        question = res->msg;
    }

    build_page(
        "videos",
        "Busque pelos videos que deseja ver, ou scrole ate achar um interessante",
        question,
        ops,
        nxt,
        lst,
        selectFn_lv,
        NULL,
        NULL,
        this_p
    );
}