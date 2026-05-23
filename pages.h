#ifndef PAGED_H
#define PAGED_H
#include "header.h"
#include "lists.h"

#include "schema.h"

#include <conio.h>
#include <stdlib.h>

#define Arrow_up 72
#define Arrow_down 80
#define Arrow_left 75
#define Arrow_right 77
#define Enter (int)'\r'
#define E (int)'e'
#define P (int)'p'

#define GO_FORD -1
#define GO_BACK -2
#define EXIT_APP -3
#define SEARCH -4

typedef struct User_loged
{
    int unsigned id;
    char name[50];
} User_loged;


typedef struct Data
{
    User_loged user;
    void *payload;
    void *response;
} Data;

typedef void (*PageFn)(Page *this_p);

typedef struct ChangePage
{
    PageFn free_all;
    PageFn build;
} ChangePage;

typedef struct Page_video_info
{
    char search_title[50];
    unsigned int vid_selected;
    PageFn lst_page;
} Page_video_info;

typedef struct Page_playlist_info
{
    char search_title[50];
    unsigned int pl_selected;
    unsigned int vid_id;
    void *data;
    Playlist *playlist;
    PageFn lst_page;
} Page_playlist_info;

typedef struct Page
{
    prt_l_list *link;
    char title[20];
    char description[350];
    char **opcoes;
    char question[350];
    int selected;
    int i_janela;
    ChangePage *nxt;
    ChangePage *lst;
    Data data;
    ChangePage (*selectFn)(struct Page *, int);
    // void (*clearFn)(struct Page *);
    void (*render_options)(struct Page *, int i);
    void (*action)(struct Page *);
} Page;

dinamic_list_dec(ChangePage, dina_chPage, sizeof(ChangePage))

void render(Page *page);

void listening_arrows(Page *page);

int validateCh_login(char c);

int validateCh_palylist(char c);

void insert_terminal(char *question, char *data, int limit, int (*verifyFn)(char c));

void clearFn_defualt(Page *this_p);

char **add_opcao(char *op, char **ops, int size);

void build_page(char *title, char *description, char *question, char **opcoes, ChangePage *nxt, ChangePage *lst, void *selectFn, void *render_payload, void *action, Page *this_p);

void live_page(Page *page);

void page_inicio(Page *this_p);

void page_login(Page *this_p);

void page_signup(Page *this_p);

void page_home(Page *this_p);

void page_look_vids(Page *this_p);

void page_playlists(Page *this_p);

void page_open_vid(Page *this_p);

void page_create_playlist(Page *this_p);

void page_video_in_pl(Page *this_p);

void page_add_video(Page *this_p);

void page_playlist_inside(Page *this_p);

void request_vids(Page *this_p);

void request_video_by_id(Page *this_p);

void request_playlists(Page *this_p);

void request_playlists_add_video(Page *this_p);

void request_videos_in_pl(Page *this_p);

void request_video_by_id_in_pl(Page *this_p);

ChangePage selectFn_pl(Page *this_p, int lst_selected);

#define add_nxt_pag(pageFn_, free_before, list)  \
    do                                           \
    {                                            \
        ChangePage change_##pageFn;              \
        change_##pageFn.free_all = free_before; \
        change_##pageFn.build = pageFn_;        \
        dina_chPage_add(change_##pageFn, list);  \
    } while (0);

#endif
