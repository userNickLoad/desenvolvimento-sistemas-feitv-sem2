#ifndef PAGED_H
#define PAGED_H
#include "header.h"
#include "lists.h"

#define Arrow_up 72
#define Arrow_down 80
#define Arrow_left 75
#define Arrow_right 77
#define Enter 10

typedef struct Page
{
    Str_list link;
    char *title;
    char *description;
    char **opcoes;
    int selected;
    int i_janela;
    Page **nxt;
    Page *lst;
    void *payload;
    void (*render_payload)(struct Page *);
    void (*action)(struct Page *, int);
    void (*consulta)(struct Page *, int);

} Page;

dinamic_list_dec(Page *, dina_pags, sizeof(Page))

void render(Page * page);

void listening_arrows(Page * page);

Page *build_page(char *title, char *description, char **opcoes, void * render_payload, void *action);

Page * link_pages();

Page * live_page(Page * page);

Page * page_inicio(void);

Page * page_login(void);

Page * page_signup(void);

#endif
