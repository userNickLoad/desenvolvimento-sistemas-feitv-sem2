#include "../pages.h"
#include "../header.h"
#include "../schema.h"
#include "../lists.h"
#include "../data/source/data.h"
#include "utils.h"

void clear_resposnse_pl(Page *this_p)
{
    Response *res = (Response *)this_p->data.response;
    if (res != NULL)
    {     
        Playlist **playlist = ( Playlist **)res->data;

        if (playlist != NULL)
        {
            for(int i = 0; i < dinamic_size(playlist); i++)
            {
                free(playlist[i]);
            }
            dinamic_free(void*, playlist);
        }
        
        free(res);
        this_p->data.response = NULL;
    }
}

void request_playlists(Page *this_p)
{   
    Page_playlist_info *info = this_p->data.payload;
    
    Response *res = this_p->data.response;
    clear_resposnse_pl(this_p);

    if (info == NULL)
    {   
        info = malloc(sizeof(Page_playlist_info));
        info->search_title[0] = '\0';
        info->lst_page = page_home;
        info->playlist = NULL;
        info->data = NULL;
        this_p->data.payload = info;
    }
    
    char *title = info->search_title;
    res = search_for_playlists(this_p->data.user.id, title);

    this_p->data.response = res;

    page_playlists(this_p);
}

generate_gettxt_page(pl_search_bar, "Busque por suas playlists", Page_playlist_info, search_title, validateCh_login, request_playlists)

void clear_pl(Page *this_p)
{   
    Page_playlist_info *page_i = (Page_playlist_info *) this_p->data.payload;
    if (page_i != NULL)
    {
        free(page_i);
        this_p->data.payload = NULL;
    }

   clear_resposnse_pl(this_p);
}

ChangePage selectFn_pl(Page *this_p, int lst_selected)
{   
    Page_playlist_info *data = this_p->data.payload;
    Response *res = this_p->data.response;
    Playlist **playlists = (Playlist **)res->data;
    data->pl_selected = (lst_selected > 1) ? playlists[lst_selected-2]->id: 0;
    // for(int i = 0; i < dinamic_size(this_p->nxt); i++)
    // {
    //     printf("Pointeiro do free)
    // }
    return (lst_selected < 2)? this_p->nxt[lst_selected]: this_p->nxt[2];
}

void render_playlists(Page *this_p, int i)
{   
    if (i == 2)
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

void page_playlists(Page *this_p) {

    Response *res = (Response *)this_p->data.response;
    Playlist **playlists = (Playlist **)res->data;
    Page_playlist_info *info = (Page_playlist_info *)this_p->data.payload;
    int playlists_amout = 0;
    
    if(playlists != NULL) playlists_amout = dinamic_size(playlists);

    char **ops = (char **)dina_prt_init(2 + playlists_amout);
    ChangePage *nxt = dina_chPage_init(3);
    ChangePage *lst = malloc(sizeof(ChangePage));

    lst->free_all = clear_pl;
    lst->build = page_home;

    char ops1[85];
    sprintf(ops1, "Busque por playlists: %s", info->search_title);
    ops = add_opcao(ops1, ops, sizeof(ops1));
    add_nxt_pag(pl_search_bar, NULL, nxt);

    ops = add_opcao("Criar nova playlist", ops, 25);
    add_nxt_pag(page_create_playlist, clear_resposnse_pl, nxt)
  

    for (int i = 0; i < playlists_amout; i++)
    {
        add_opcao(playlists[i]->name, ops, sizeof(playlists[i]->name));
    }

    add_nxt_pag(request_videos_in_pl, clear_resposnse_pl, nxt)

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
        selectFn_pl,
        render_playlists,
        NULL,
        this_p
    );
}