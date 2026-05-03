#include "header.h"
#include "pages.h"
#include "lists.h"

#include <conio.h>
#include <stdlib.h>

#define qtd_itens_janela 10

dinamic_list_impl(ChangePage, dina_chPage, sizeof(ChangePage))

void listening_arrows(Page *page) {
    /*
     *  - Essa função monitora e adiministra a seleção da pagina e possui os seguintes cases:
     *      + Seta para cima faz o cursor subir;
     *      + Seta para baixo faz o cursor descer;
     *      + Seta para esquerda seleciona a opção de voltar para a pagina anterior;
     *      + Enter ou seta para a direita confirmam e executam a opção já selecionada;
     *      + Janela: Como não é legal encher a tela de opções, o maximo está definido para 10 opções.
     *               Quando a pagina apresentar mais do que 10 opções, a regra será mantida graças a
     *               janela que seleciona quais itens serão expostos. Na Struct Page, o atributo i_janela
     *               indica a primeira opção que será mostrada e a ultima será a última opção de todas
     *               caso o número de opções < 10, caso contrario, a última opção será i_janela + 10.
     */

    int c;

    do {
        c = getch();
    } while (!(((c == Arrow_down || c == Arrow_up || c == Arrow_right || c == Enter) && (page->opcoes != NULL)) || c ==
               Arrow_left || c == E));

    if (c == Arrow_left) {
        // selected = -2 significa que vai voltar para a pagina anterior
        page->selected = (page->lst != NULL) ? GO_BACK : page->selected;
        return;
    }

    if (c == E) {
        page->selected = EXIT_APP;
        return;
    }

    if (page->opcoes == NULL) {
        return;
    }

    const int limit = dinamic_size(page->opcoes);

    const int f_janela = (limit > qtd_itens_janela) ? page->i_janela + qtd_itens_janela : limit;

    switch (c) {
        case Arrow_down:
            // se o final da janela for o limite
            if (f_janela == limit) {
                // se não selecionamos o ultimo item da lista select++ se não select fica o mesmo
                page->selected += (page->selected < limit - 1) ? 1 : 0;
                break; // early return
            }

            // se o final da janela não for o limite E está selecionado o ultimo item DA JANELA TEMOS QUE MOVER
            if (page->selected == qtd_itens_janela - 1) {
                // janela desce junto com tudo
                page->i_janela++;
                page->selected++;
                break; // early return
            }

            // se o final da janela não é o limite E o item selecionado não é o ultimo DESCE SEM MEDO DE SER FELIZ
            page->selected++;

            break;

        case Arrow_up:
            // se o inicio da janela for o primeiro item de todos
            if (page->i_janela == 0) {
                // se não está selecionado o primeiro item da lista select-- se está select fica o mesmo
                page->selected -= (page->selected > 0) ? 1 : 0;
                break; // early return
            }

            // se o inicio da janela não for o primeiro item de todos E está selecionado o primeiro item DA JANELA TEMOS QUE MOVER
            if (page->selected == 0) {
                // janela desce junto com tudo
                page->i_janela--;
                page->selected--;
                break; // early return
            }

            // se o final da janela não é o limite E o item selecionado não é o ultimo DESCE SEM MEDO DE SER FELIZ
            page->selected--;

            break;

        case P:
            page->selected = SEARCH;

        default:
            // selected = -1 significa que vai avançar para a próxima pagina com base na opção selecionada
            page->selected = GO_FORD;
    }
}

void render_options_default(Page *page, int i) {
    if (i == page->selected) {
        printf("\t\t---> [%d]: %s;\n", i, page->opcoes[i]);
        return;
    }

    printf("\t\t- [%d]: %s;\n", i, page->opcoes[i]);
}

void insert_terminal(char *question, char *data, int limit) {

    int lst = 0;
    char c;
    data[lst] = '\0';
    
    printf("\t - %s: ", question);


    while ((c = getch()) != Enter) {
        if (c == '\b' && lst > 0) {
            data[lst] = '\0';
            lst--;
            printf("\b \b");
        } else if (lst < limit && c != ' ' && c != ';' && c != ',' && c != '.' && c != '\0' && c != '\b') {
            data[lst] = c;
            lst++;
            data[lst] = '\0';
            printf("%c", c);
        }
    }
}

char **add_opcao(char *op, char **ops, int size) {
    char *crr = malloc(sizeof(char) * size);
    for (int i = 0; i < size; i++) {
        crr[i] = op[i];
        if (op[i] == '\0')
            break;
    }

    ops = (char **) dina_prt_add(crr, (void **) ops);

    return ops;
}


void free_opcoes(char **ops) {
    for (int i = 0; i < dinamic_size(ops); i++) {
        free(ops[i]);
    }
    dinamic_free(void *, ops);
}

void render(Page *page) {
    /*
     * - Essa função é responsavel por colocar na tela tudo o q o usuário precisa, incluindo:
     *      + Um link no header, aprenas para que facilite a localização;
     *      + Uma descrição, para dar orientação para o usuário;
     *      + Caso tenham opções, elas são expostas de forma agradavel;
     *
     */
    node_char_ptr *crr_link = page->link->head;

    while (crr_link != NULL) {
        printf("%s/", crr_link->value);
        crr_link = crr_link->nxt;
    }

    printf("\n\n");

    if (page->description != NULL) {
        printf("%s", page->description);

        printf("\n\n");
    }

    if (page->opcoes != NULL) {
        const int limit = dinamic_size(page->opcoes);

        const int f_janela = (limit > qtd_itens_janela) ? page->i_janela + qtd_itens_janela : limit;

        for (int i = page->i_janela; i < f_janela; i++) {
            page->render_options(page, i);
        }
    }

    if (page->question != NULL) {
        printf("%s", page->question);
    }
}

void clearFn_defualt(Page *this_p) {
   
    if (this_p->opcoes != NULL)
        free_opcoes(this_p->opcoes);

    if (this_p->nxt != NULL){
        dinamic_free(ChangePage, this_p->nxt);
    }
    
    if (this_p->lst != NULL){
        free(this_p->lst);
    }
}

ChangePage selectFn_default(Page *this_p, int lst_selected){
    return this_p->nxt[lst_selected];
}

void build_page(char *title, char *description, char *question, char **opcoes, ChangePage *nxt, ChangePage *lst, void *selectFn,
                void *render_options, void *action, Page *this_p) {
    
    // if(clearFn == NULL){
    //     this_p->clearFn = clearFn_defualt;
    // }
    // else{
    //     this_p->clearFn = clearFn;
    // }

    if(selectFn == NULL)
        this_p->selectFn = selectFn_default;
    else
        this_p->selectFn = selectFn;

    if (lst == NULL && this_p->link != NULL) {
        free_Str(this_p->link);
        this_p->link = NULL;
    }

    if (this_p->link == NULL) {
        this_p->link = Str_init_list();
        this_p->data.payload = NULL;
    }

    add_Str(title, this_p->link);

    this_p->title = title;
    this_p->description = description;
    this_p->question = question;
    this_p->opcoes = opcoes;
    if (render_options == NULL) {
        this_p->render_options = render_options_default;
    } else {
        this_p->render_options = render_options;
    }
    this_p->action = action;
    this_p->selected = 0;
    this_p->i_janela = 0;
    this_p->lst = lst;
    this_p->nxt = nxt;
    

    return;
}

void live_page(Page *this_p) {
    CLEAR_TERMINAL

    if (this_p->selected < 0) {
        this_p->selected = 0;
    }

    render(this_p);

    int lst_selecet = this_p->selected;

    if (this_p->opcoes != NULL)
        listening_arrows(this_p);

    if (this_p->opcoes == NULL && this_p->action != NULL)
        this_p->action(this_p);


    switch (this_p->selected) {
        case EXIT_APP:
            exit(0);
            break;
        case GO_BACK: {
            pop_Str(this_p->link);
            pop_Str(this_p->link);

            ChangePage build_p = this_p->lst[0];
            
            clearFn_defualt(this_p);

            if(build_p.free_all)
                build_p.free_all(this_p);

            build_p.build(this_p);

            break;
        }
        case GO_FORD:{
            ChangePage build_p = this_p->selectFn(this_p, lst_selecet);

            clearFn_defualt(this_p);
            
            if(build_p.free_all)
                build_p.free_all(this_p);
            
            build_p.build(this_p);
            break;
        }
        default:
            break;
    }
}
