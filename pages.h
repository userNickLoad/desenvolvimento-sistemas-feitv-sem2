#ifndef PAGED_H
#define PAGED_H
#include "header.h"
#include "lists.h"

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

typedef struct Page
{
    Str_list *link;
    char *title;
    char *description;
    char **opcoes;
    char *question;
    int selected;
    int i_janela;
    ChangePage *nxt;
    ChangePage *lst;
    Data data;
    ChangePage (*selectFn)(struct Page *, int);
    // void (*clearFn)(struct Page *);
    void (*render_options)(struct Page *, int i);
    void (*action)(struct Page *);
    void (*consulta)(struct Page *);

} Page;

dinamic_list_dec(ChangePage, dina_chPage, sizeof(ChangePage))

    void render(Page *page);

void listening_arrows(Page *page);

void insert_terminal(char *question, char *data, int limit);

void clearFn_defualt(Page *this_p);

char **add_opcao(char *op, char **ops, int size);

void build_page(char *title, char *description, char *question, char **opcoes, ChangePage *nxt, ChangePage *lst, void *selectFn, void *render_payload, void *action, Page *this_p);

void live_page(Page *page);

void page_inicio(Page *this_p);

void page_login(Page *this_p);

void page_signup(Page *this_p);

#define add_nxt_pag(pageFn_, free_before, list)  \
    do                                           \
    {                                            \
        ChangePage change_##pageFn;              \
        change_##pageFn.free_all = free_before; \
        change_##pageFn.build = pageFn_;        \
        dina_chPage_add(change_##pageFn, list);  \
    } while (0);

#endif
