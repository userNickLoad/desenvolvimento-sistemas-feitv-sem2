#include "header.h"
#include "pages.h"
#include "lists.h"

#define qtd_itens_janela 10

dinamic_list_impl(Page *, dina_pags, sizeof(Page))

void listening_arrows(Page * page) {
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
    const int limit = dinamic_size(page->opcoes);
    const int f_janela = (limit > qtd_itens_janela)? page->i_janela + qtd_itens_janela: limit;

    do {
        c = getchar();
    }while (!(c == Arrow_down || c == Arrow_up || c == Arrow_left || c == Arrow_right || c == Enter));

    switch (c) {
        case Arrow_down:
            // se o final da janela for o limite
            if (f_janela == limit) {
                // se não selecionamos o ultimo item da lista select++ se não select fica o mesmo
                page->selected += (page->selected < limit)? 1: 0 ;
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
                page->selected -= (page->selected > 0)? 1: 0 ;
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

        case Arrow_left:
            // selected = -2 significa que vai voltar para a pagina anterior
            page->selected = (page->lst != NULL)? -2: page->selected;
            break;

        default:
            // selected = -1 significa que vai avançar para a próxima pagina com base na opção selecionada
            page->selected = -1;
    }
    
}

void render(Page * page){
    /*
     * - Essa função é responsavel por colocar na tela tudo o q o usuário precisa, incluindo:
     *      + Um link no header, aprenas para que facilite a localização;
     *      + Uma descrição, para dar orientação para o usuário;
     *      + Caso tenham opções, elas são expostas de forma agradavel;
     *
     */
    node_char_ptr *crr_link = page->link.head;

    while(crr_link != NULL){
        printf("%s/", crr_link->value);
        crr_link = crr_link->nxt;
    }

    printf("\n\n");

    if(page->description != NULL){
        printf("%s", page->description);

        printf("\n\n");
    }

    if (page->opcoes != NULL) {
        const int limit = dinamic_size(page->opcoes);

        const int f_janela = (limit > qtd_itens_janela)? page->i_janela + qtd_itens_janela: limit;

        for(int i = page->i_janela; i < f_janela; i++){
            if (i == page->selected) {
                printf("\t\t---> [%d]: %s;\n", i, page->opcoes[i]);
            }

            printf("\t\t- [%d]: %s;\n", i, page->opcoes[i]);
        }
    }

}

Page *build_page(char *title, char *description, char **opcoes, void * render_payload, void *action)
{   
    Page * this_p = (Page *) malloc(sizeof(Page));

    if (this_p == NULL) {
        fprintf(stderr, "ERRO AO ALOCAR ESPAÇO PARA A PAGINA %s NA HEAP", title);

        return NULL;
    }

    this_p->link = *Str_init_list();

    this_p->title = title;
    this_p->description = description;
    this_p->opcoes = opcoes;
    this_p->nxt = nxt;
    this_p->lst = lst;
    this_p->render_payload = render_payload;
    this_p->action = action;
    this_p->selected = 0;

    return this_p;
}

Page * live_page(Page * page){
    if(page->lst){
        page->link = page->lst->link;
    }

    add_Str(page->title, &page->link);

    render(page);


    return page;
   
}