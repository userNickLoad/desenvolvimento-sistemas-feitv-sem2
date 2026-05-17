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
    Page_info *info;
    if (this_p->data.payload == NULL)
    {
        info = malloc(sizeof(Page_info));
        info->search_title[0] = '\0';
        info->videos = NULL;
        this_p->data.payload = info;
    }
    info = this_p->data.payload;

    char *title = (info->search_title[0] != '\0')? info->search_title[0]: NULL;

    Response res = search_for_videos(title);

    copy_struct(this_p->data.response, &res, sizeof(res));

    page_look_vids(this_p);
}



void page_look_vids(Page *this_p) {

    Response *res = (Response *)this_p->data.response;
    Video *vids = (Video *)res->data;
    Page_info *info = (Page_info *)this_p->data.payload;

    char **ops = (char **)dina_prt_init(1+dinamic_size(vids));
    ChangePage *nxt = dina_chPage_init(2);
    ChangePage *lst = malloc(sizeof(ChangePage));

    lst->free_all = NULL;
    lst->build = page_home;

    char ops1[85];
    sprintf(ops1, "Busque por filmes: %s", info->search_title);
    ops = add_opcao(ops1, ops, sizeof(ops1));

    build_page(
        "videos",
        "Busque pelos videos que deseja ver, ou scrole ate achar um interessante",
        NULL,
        NULL,
        NULL,
        lst,
        NULL,
        NULL,
        NULL,
        this_p
    );
}