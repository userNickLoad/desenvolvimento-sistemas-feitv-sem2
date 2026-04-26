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

#define GO_FORD -1
#define GO_BACK -2
#define EXIT_APP -3
#define SEARCH -4

typedef void (*PageFn)(void *, Page * this_p);

typedef struct Page
{   
    Str_list *link;
    char *title;
    char *description;
    char **opcoes;
    char *question;
    int selected;
    int i_janela;
    PageFn *nxt;
    PageFn lst;
    void *payload;
    void (*render_options)(struct Page *, int i);
    void (*action)(struct Page *, int);
    void (*consulta)(struct Page *, int);

} Page;

dinamic_list_dec(void *, dina_pags, sizeof(void))

void render(Page * page);

void listening_arrows(Page * page);

char ** add_opcao(char* op, char **ops);

void build_page(char *title, char *description, char * question, char **opcoes, void *nxt, void *lst, void * render_payload, void *action, Page * this_p);

void live_page(Page * page);

void page_inicio(void *, Page * this_p);

void page_login(void *, Page * this_p);

void page_signup(void *, Page * this_p);

#endif
