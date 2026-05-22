#include "../pages.h"
#include "../header.h"
#include "../schema.h"
#include "../lists.h"
#include "../data/source/data.h"
#include "utils.h"

generate_gettxt_page(lv_search_bar, "Busque por filmes", Page_video_info, search_title, validateCh_login, request_vids)

void clear_resposnse_lv(Page *this_p)
{
    Response *res = (Response *)this_p->data.response;
    if (res != NULL)
    {      
        Video **vids = res->data;
        for(int i = 0; i < dinamic_size(vids); i++)
        {
            free(vids[i]);
        }
        dinamic_free(void*, vids);
        free(res);
        this_p->data.response = NULL;
    }
}
void clear_lv(Page *this_p)
{   
    Page_video_info *page_i = (Page_video_info *) this_p->data.payload;
    if (page_i != NULL)
    {
        free(page_i);
        this_p->data.payload = NULL;
    }

   clear_resposnse_lv(this_p);
}

void request_vids(Page *this_p)
{   
    Page_video_info *info = this_p->data.payload;
    
    Response *res = this_p->data.response;
    if (res != NULL){
        Video **vids = res->data;
        if(vids != NULL)
        {
            for(int i = 0; i < dinamic_size(vids); i++)
            {
                free(vids[i]);
            }
            dinamic_free(void*, vids);
            res->data = NULL;
        }
        
        free(res);
        this_p->data.response = NULL;
    }

    if (info == NULL)
    {   
        info = malloc(sizeof(Page_video_info));
        info->search_title[0] = '\0';
        info->lst_page = request_vids;
        this_p->data.payload = info;
    }
    
    char *title = info->search_title;
    res = search_for_videos(title);

    
    this_p->data.response = res;

    page_look_vids(this_p);
}

ChangePage selectFn_lv(Page *this_p, int lst_selected)
{   
    Page_video_info *data = this_p->data.payload;
    Response *res = this_p->data.response;
    Video **vids = res->data;
    data->vid_selected = (vids != NULL)? vids[lst_selected - 1][0].id: 0;
    return (lst_selected == 0)? this_p->nxt[0]: this_p->nxt[1];
}

void render_videos(Page *this_p, int i)
{   
    if (i == 1)
    {   
        Response *res = (Response *)this_p->data.response;
        Video **videos = (Video **)res->data;

        if (videos == NULL)
        printf("\n\tNenhum video encontrado.\n");
        else
        printf("\n\tResulto %u videos:\n\n", dinamic_size(videos));
    }
    if (i == this_p->selected)
    {
        printf("\t\t---> [%d]: %s;\n", i, this_p->opcoes[i]);
        return;
    }

    printf("\t\t- [%d]: %s;\n", i, this_p->opcoes[i]);
}

void page_look_vids(Page *this_p) {

    Response *res = (Response *)this_p->data.response;
    Video **vids = (Video **)res->data;
    Page_video_info *info = (Page_video_info *)this_p->data.payload;
    int vids_amout = 0;
    if(vids != NULL) vids_amout = dinamic_size(vids);

    char **ops = (char **)dina_prt_init(1 + vids_amout);
    ChangePage *nxt = dina_chPage_init(2);
    ChangePage *lst = malloc(sizeof(ChangePage));

    lst->free_all = clear_lv;
    lst->build = page_home;

    char ops1[85];
    sprintf(ops1, "Busque por filmes: %s", info->search_title);
    ops = add_opcao(ops1, ops, sizeof(ops1));

    add_nxt_pag(lv_search_bar, NULL, nxt)

    if(vids_amout > 0) add_nxt_pag(request_video_by_id, clear_resposnse_lv, nxt)

    for (int i = 0; i < vids_amout; i++)
    {
        add_opcao(vids[i][0].name, ops, sizeof(vids[i][0].name));
    }

    char *question = NULL;

    // if (res->code != 200)
    // {
    //     question = res->msg;
    // }

    build_page(
        "videos",
        "Busque pelos videos que deseja ver, ou scrole ate achar um interessante",
        question,
        ops,
        nxt,
        lst,
        selectFn_lv,
        render_videos,
        NULL,
        this_p
    );
}