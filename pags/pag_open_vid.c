#include "../pages.h"
#include "../header.h"
#include "../schema.h"
#include "../lists.h"
#include "../data/source/data.h"
#include "utils.h"

void clear_open_vid(Page * this_p)
{   
    Response * res = this_p->data.response;
    Video_user * data = res->data;

    if (data != NULL)
    {
        free(data);
    }

    if (res != NULL)
    {
        free(res);
    }

    this_p->data.response = NULL;
}

void request_video_by_id(Page *this_p)
{   
    Page_video_info *info = this_p->data.payload;

    Response *res = video_user_search(this_p->data.user.id, info->vid_selected);

    this_p->data.response = res;

    page_open_vid(this_p);
}

void request_like(Page *this_p)
{
    pop_title(this_p->link);
    Page_video_info *info = this_p->data.payload;

    handle_like(this_p->data.user.id, info->vid_selected);

    request_video_by_id(this_p);
}

void request_disike(Page *this_p)
{   
    pop_title(this_p->link);
    Page_video_info *info = this_p->data.payload;

    handle_dislike(this_p->data.user.id, info->vid_selected);

    request_video_by_id(this_p);
}

void page_open_vid(Page *this_p) 
{
    Response *res = (Response *)this_p->data.response;
    Video_user *vid = (Video_user *)res->data;
    Page_video_info *info = (Page_video_info *)this_p->data.payload;

    char **ops = (char **)dina_prt_init(3);
    ChangePage *nxt = dina_chPage_init(3);
    ChangePage *lst = malloc(sizeof(ChangePage));

    lst->free_all = clear_open_vid;
    lst->build = info->lst_page;

    char ops1[40];
    snprintf(ops1, 40, "%u Gostei%s", vid->video.likes, (vid->like)? " @": "");
    ops = add_opcao(ops1, ops, sizeof(ops1));

    char ops2[40];
    snprintf(ops2, 40, "%u Nao gostei%s", vid->video.dislikes, (vid->dislike)? " @": "");
    ops = add_opcao(ops2, ops, sizeof(ops2));

    ops = add_opcao("Salvar em playlists", ops, sizeof(ops2));

    add_nxt_pag(request_like, clear_open_vid, nxt)
    add_nxt_pag(request_disike, clear_open_vid, nxt)
    add_nxt_pag(request_playlists_add_video, clear_open_vid, nxt)

    // char *description = malloc(330);
    char description[330];
    description[330] = '\0';

    char *question = NULL;
    char *title = "video";

    if(res->code == 200)
    {
        title = vid->video.name;
        snprintf(description, 330, "\n\n\tTitulo: %s;\n\nDescricao: %s;", vid->video.name, vid->video.desc);
    } else
    {
        question = res->msg;
    }

    build_page(
        title,
        description,
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