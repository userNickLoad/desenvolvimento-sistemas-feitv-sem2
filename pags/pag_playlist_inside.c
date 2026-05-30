#include "../pages.h"
#include "../header.h"
#include "../schema.h"
#include "../lists.h"
#include "../data/source/data.h"
#include "utils.h"

void clear_resposnse_pl_in(Page *this_p)
{
    Response *res = (Response *)this_p->data.response;
    if (res != NULL)
    {     
        Playlis_Videos_PV_DTO *dto = ( Playlis_Videos_PV_DTO *)res->data;
        if (dto != NULL)
        {
            Playlist * playlist =  dto->playlist;
            Videos_PV_DTO **videos =  dto->videos;

            if (videos != NULL)
            {
                for(int i = 0; i < dinamic_size(videos); i++)
                {
                    free(videos[i]->videos);
                    free(videos[i]);
                }
                dinamic_free(void*, videos);
            }

            if(playlist != NULL)
            {
                free(playlist);
            }
            
            free(res);
        }
       
        this_p->data.response = NULL;
    }
}

void request_videos_in_pl(Page *this_p)
{   
    Page_playlist_info *info = this_p->data.payload;
    
    Response *res = this_p->data.response;

    char *title = info->search_title;

    if (res == NULL)
    {
        title[0] = '\0';
    }

    clear_resposnse_pl_in(this_p);
    
    res = videos_from_playlist(info->pl_selected, title);

    this_p->data.response = res;

    page_playlist_inside(this_p);
}

void request_delete_playlist(Page *this_p)
{   
    Page_playlist_info *info = this_p->data.payload;
    
    Response *res = this_p->data.response;
    clear_resposnse_pl_in(this_p);

    res = delete_playlist(info->pl_selected, this_p->data.user.id);

    this_p->data.response = res;
    clear_resposnse_pl_in(this_p);
    pop_title(this_p->link);
    pop_title(this_p->link);
    request_playlists(this_p);
}

generate_gettxt_page(pl_in_search_bar, "Busque por seus videos", Page_playlist_info, search_title, validateCh_login, request_videos_in_pl)


ChangePage selectFn_pl_in(Page *this_p, int lst_selected)
{   
    Page_playlist_info *data = this_p->data.payload;
    Response *res = this_p->data.response;
    Playlis_Videos_PV_DTO *dto = ( Playlis_Videos_PV_DTO *)res->data;
    Videos_PV_DTO **videos =  dto->videos;

    data->vid_id = (lst_selected > 1) ? videos[lst_selected-2]->videos->id: 0;

    return (lst_selected < 2)? this_p->nxt[lst_selected]: this_p->nxt[2];
}

void render_in_playlists(Page *this_p, int i)
{   
    if (i == 2)
    {   
        Response *res = (Response *)this_p->data.response;
        Playlis_Videos_PV_DTO *dto = (Playlis_Videos_PV_DTO *)res->data;
        int videos_amount = dto->playlist->videos;

        if (dto->playlist->videos == 0)
        printf("\n\tNenhum video encontrado na playlist. Adicione videos para que eles aparecam aqui!\n");
        else
        printf("\n\tResulto %u videos:\n\n", videos_amount);
    }
    if (i == this_p->selected)
    {
        printf("\t\t---> [%d]: %s;\n", i, this_p->opcoes[i]);
        return;
    }

    printf("\t\t- [%d]: %s;\n", i, this_p->opcoes[i]);
}

void page_playlist_inside(Page *this_p) {

    Response *res = (Response *)this_p->data.response;
    Playlis_Videos_PV_DTO *dto = (Playlis_Videos_PV_DTO *)res->data;
    Page_playlist_info *info = (Page_playlist_info *)this_p->data.payload;

    int videos_amout = 0;
    Videos_PV_DTO **videos = NULL;
    Playlist *playlist = NULL;

    if (dto != NULL)
    {
        playlist = dto->playlist;
        videos = (Videos_PV_DTO **)dto->videos;

        if(videos != NULL) videos_amout = dinamic_size(videos);
    }
    
    char **ops = (char **)dina_prt_init(2 + videos_amout);
    ChangePage *nxt = dina_chPage_init(3);
    ChangePage *lst = malloc(sizeof(ChangePage));

    lst->free_all = clear_resposnse_pl_in;
    lst->build = request_playlists;

    char ops1[85];
    sprintf(ops1, "Busque por videos: %s", info->search_title);
    ops = add_opcao(ops1, ops, sizeof(ops1));
    add_nxt_pag(pl_in_search_bar, NULL, nxt);

    ops = add_opcao("Deletar playlist", ops, 25);
    add_nxt_pag(request_delete_playlist, clear_resposnse_pl_in, nxt)
  
    for (int i = 0; i < videos_amout; i++)
    {
        add_opcao(videos[i]->videos->name, ops, sizeof(videos[i]->videos->name));
    }

    add_nxt_pag(request_video_by_id_in_pl, clear_resposnse_pl_in, nxt)

    char *question = NULL;
    char *description = "NOT FUNDO";
    char *title = "NOT FUNDO";

    if (res->code != 200)
    {
        question = res->msg;
    }

    if(playlist != NULL)
    {
        title = playlist->name;
        description = playlist->description;
    }
    
    build_page(
        title,
        description,
        question,
        ops,
        nxt,
        lst,
        selectFn_pl_in,
        render_in_playlists,
        NULL,
        this_p
    );
}