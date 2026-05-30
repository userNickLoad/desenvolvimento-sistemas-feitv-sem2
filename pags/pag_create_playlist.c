#include "../pages.h"
#include "../header.h"
#include "../schema.h"
#include "../lists.h"
#include "../data/source/data.h"
#include "utils.h"


generate_gettxt_page(pl_create_name, "Crie um nome", Page_playlist_info, playlist->name, validateCh_palylist, page_create_playlist)

generate_gettxt_page(pl_create_desc, "Crie uma descricao", Page_playlist_info, playlist->description, validateCh_palylist, page_create_playlist)

void clr_palylist(Playlist *playlist)
{
    if(playlist) free(playlist);
}

void clear_resposnse_resquest_create(Response *res)
{
    if (res != NULL)
    {     
        Playlist **playlist = ( Playlist **)res->data;

        if (playlist != NULL)
        {
            for(int i = 0; i < dinamic_size(playlist); i++)
            {
                clr_palylist(playlist[i]);
            }
            dinamic_free(void*, playlist);
        }
        
        free(res);
    }
}

void clr_create_playlist(Page *this_p)
{   
    Page_playlist_info *page_info = this_p->data.payload;
    Playlist *playlist = (Playlist *) page_info->playlist;
    Response *res = this_p->data.response;

    clr_palylist(playlist);
    page_info->playlist = NULL;
    
    clear_resposnse_resquest_create(res);
    this_p->data.response = NULL;
}

void resquest_create_playlist(Page *this_p)
{
    Page_playlist_info *page_info = this_p->data.payload;
    Playlist *playlist = page_info->playlist;

    Response *res = create_playlist(this_p->data.user.id, playlist->name, playlist->description);

    if (res->code != 200)
    {
        this_p->data.response = res;
        pop_title(this_p->link);
        clr_palylist(page_info->playlist);
        page_info->playlist = NULL;
        page_create_playlist(this_p);
        return;
    }

    pop_title(this_p->link);
    pop_title(this_p->link);
    clr_create_playlist(this_p);
    request_playlists(this_p);
}

void page_create_playlist(Page *this_p)
{   
    Page_playlist_info *page_info = this_p->data.payload;
    Playlist *playlist = page_info->playlist;
    if (playlist == NULL)
    {
        playlist = malloc(sizeof(Playlist));
        playlist->name[0] = '\0';
        playlist->description[0] = '\0';
        playlist->user_id = this_p->data.user.id;
        page_info->playlist = playlist;
    }

    Response *res = (Response *)this_p->data.response;

    char **ops = (char **)dina_prt_init(4);
    ChangePage *nxt = dina_chPage_init(3);
    ChangePage *lst = malloc(sizeof(ChangePage));

    lst->build = request_playlists;
    lst->free_all = clr_create_playlist;

    char ops1[85];
    sprintf(ops1, "Crie um nome: %.50s", playlist->name);
    ops = add_opcao(ops1, ops, sizeof(ops1));
    add_nxt_pag(pl_create_name, NULL, nxt)

    char ops2[300];
    sprintf(ops2, "Crie uma descricao: %.250s", playlist->description);
    ops = add_opcao(ops2, ops, sizeof(ops2));
    add_nxt_pag(pl_create_desc, NULL, nxt)

    char *question = NULL;

    if (playlist->name[0] == '\0')
    {
        question = "\n\tCrie pelo menos o nome da playlist.";
    } else
    {
        ops = add_opcao("CRIAR", ops, 10);
        add_nxt_pag(resquest_create_playlist, NULL, nxt)
    }

    if (res != NULL)
    {
        question = res->msg;
    }

    build_page(
        "create",
        "\n\n\tCrie uma nova playlist com um nome e descricao criativos.",
        question,
        ops,
        nxt,
        lst,
        NULL,
        NULL,
        NULL,
        this_p
    );
}