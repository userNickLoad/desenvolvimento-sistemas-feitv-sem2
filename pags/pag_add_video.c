#include "../pages.h"
#include "../header.h"
#include "../schema.h"
#include "../lists.h"
#include "../data/source/data.h"
#include "utils.h"

void clear_resposnse_add_video(Page *this_p)
{
    Response *res = (Response *)this_p->data.response;
    if (res != NULL)
    {     
        free(res);
        this_p->data.response = NULL;
    }
}

void clear_lv_add(Page *this_p)
{   
    Page_video_info *page_i = (Page_video_info *) this_p->data.payload;
    if (page_i != NULL)
    {
        free(page_i);
        this_p->data.payload = NULL;
    }

//    clear_resposnse_lv(this_p);
}

void clear_pl_add(Page *this_p)
{      
    pop_title(this_p->link);
    Page_playlist_info *page_i = (Page_playlist_info *) this_p->data.payload;
    if (page_i != NULL)
    {
        free(page_i);
        this_p->data.payload = NULL;
    }

   clear_resposnse_add_video(this_p);
}

void request_add_video(Page *this_p)
{   
    Page_playlist_info *playlist_info = this_p->data.payload;

    Response *res = this_p->data.response;

    clear_resposnse_add_video(this_p);

    res = add_video_playlist(playlist_info->pl_selected, playlist_info->vid_id);

    if(res->code != 200)
    {
        this_p->data.response = res;
        request_playlists_add_video(this_p);
        return;
    }

    free(res);
    pop_title(this_p->link);
    pop_title(this_p->link);

    clear_pl_add(this_p);
    request_vids(this_p);
}


void request_playlists_add_video(Page *this_p)
{   
    Page_video_info *video_info = this_p->data.payload;
    int video_id = video_info->vid_selected;
    // clear_lv_add(this_p);

    Page_playlist_info *playlist_info = this_p->data.payload;
    
    Response *res = this_p->data.response;
    clear_resposnse_add_video(this_p);

    if (playlist_info == NULL)
    {   
        playlist_info = malloc(sizeof(Page_playlist_info));
        playlist_info->search_title[0] = '\0';
        playlist_info->lst_page = page_home;
        playlist_info->playlist = NULL;
        playlist_info->data = NULL;
        playlist_info->vid_id = video_id;
        this_p->data.payload = playlist_info;
    }

    playlist_info->vid_id = video_id;
    
    char *title = playlist_info->search_title;
    res = search_for_playlists_to_add(this_p->data.user.id, video_id, title);

    this_p->data.response = res;

    page_add_video(this_p);
}

generate_gettxt_page(pl_add_search_bar, "Busque por suas playlists", Page_playlist_info, search_title, validateCh_palylist, request_playlists_add_video)

ChangePage selectFn_pl_(Page *this_p, int lst_selected)
{   
    Page_playlist_info *data = this_p->data.payload;
    Response *res = this_p->data.response;
    Playlist **playlists = (Playlist **)res->data;
    data->pl_selected = (lst_selected != 0) ? playlists[lst_selected-1]->id: 0;

    return (lst_selected == 0)? this_p->nxt[lst_selected]: this_p->nxt[1];
}

void render_playlists_(Page *this_p, int i)
{   
    if (i == 1)
    {   
        Response *res = (Response *)this_p->data.response;
        Playlist **playlists = (Playlist **)res->data;

        if (playlists == NULL)
        printf("\n\tNenhuma playlistencontrada. Crie alguma ou mude a busca.\n");
        else
        printf("\n\tResulto %u playlists:\n\n", dinamic_size(playlists));
    }
    if (i == this_p->selected)
    {
        printf("\t\t---> [%d]: %s;\n", i, this_p->opcoes[i]);
        return;
    }

    printf("\t\t- [%d]: %s;\n", i, this_p->opcoes[i]);
}

void page_add_video(Page *this_p) {

    Response *res = (Response *)this_p->data.response;
    Playlist **playlists = (Playlist **)res->data;
    Page_playlist_info *info = (Page_playlist_info *)this_p->data.payload;
    int playlists_amout = 0;
    
    if(playlists != NULL) playlists_amout = dinamic_size(playlists);

    char **ops = (char **)dina_prt_init(1 + playlists_amout);
    ChangePage *nxt = dina_chPage_init(2);
    ChangePage *lst = malloc(sizeof(ChangePage));

    lst->free_all = clear_pl_add;
    lst->build = request_vids;

    char ops1[85];
    sprintf(ops1, "Busque por playlists: %s", info->search_title);
    ops = add_opcao(ops1, ops, sizeof(ops1));
    add_nxt_pag(pl_add_search_bar, NULL, nxt);

    add_nxt_pag(request_add_video, NULL, nxt);

    for (int i = 0; i < playlists_amout; i++)
    {
        add_opcao(playlists[i]->name, ops, sizeof(playlists[i]->name));
    }

    char *question = NULL;

    if (res->code != 200)
    {
        question = res->msg;
    }

    build_page(
        "playlists",
        "Busque por suas playlists que voce criou",
        question,
        ops,
        nxt,
        lst,
        selectFn_pl_,
        render_playlists_,
        NULL,
        this_p
    );
}