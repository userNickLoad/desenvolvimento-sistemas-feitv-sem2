#include "header.h"
#include "pages.h"
#include "lists.h"

#include <conio.h>
#include <stdlib.h>

#define qtd_itens_janela 10

dinamic_list_impl(void *, dina_pags, sizeof(void))

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

    do {
        c = getch();
        
    }while (!(((c == Arrow_down || c == Arrow_up  || c == Arrow_right || c == Enter) && (page->opcoes != NULL)) || c == Arrow_left || c == E));

    if (c == Arrow_left) {
        // selected = -2 significa que vai voltar para a pagina anterior
        page->selected = (page->lst != NULL)? GO_BACK: page->selected;
        return;
    }

    if (c == E){
        page->selected = EXIT_APP;
        return;
    }

    if(page->opcoes == NULL){
        return;
    }

    const int limit = dinamic_size(page->opcoes);

    const int f_janela = (limit > qtd_itens_janela)? page->i_janela + qtd_itens_janela: limit;


    switch (c) {
        case Arrow_down:
            // se o final da janela for o limite
            if (f_janela == limit) {
                // se não selecionamos o ultimo item da lista select++ se não select fica o mesmo
                page->selected += (page->selected < limit - 1)? 1: 0 ;
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

        default:
            // selected = -1 significa que vai avançar para a próxima pagina com base na opção selecionada
            page->selected = GO_FORD;
    }
    
}

void render_options_default(Page* page, int i ){
    if (i == page->selected) {
        printf("\t\t---> [%d]: %s;\n", i, page->opcoes[i]);
        return;
    }

    printf("\t\t- [%d]: %s;\n", i, page->opcoes[i]);
}

void render(Page * page){
    /*
     * - Essa função é responsavel por colocar na tela tudo o q o usuário precisa, incluindo:
     *      + Um link no header, aprenas para que facilite a localização;
     *      + Uma descrição, para dar orientação para o usuário;
     *      + Caso tenham opções, elas são expostas de forma agradavel;
     *
     */
    node_char_ptr * crr_link = page->link->head;

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
            page->render_options(page, i);
        }
    }

    if(page->question != NULL){
        printf(page->question);
    }

}

void build_page(char *title, char *description, char * question, char **opcoes, void *nxt, void *lst, void * render_options, void *action, Page *this_p)
{      
    if(lst == NULL && this_p->link != NULL){
        free_Str(this_p->link);
        this_p->link = NULL;
    }

    if(this_p->link == NULL){
        this_p->link = Str_init_list();
        add_Str(title, this_p->link);
    }

    this_p->title = title;
    this_p->description = description;
    this_p->question = question;
    this_p->opcoes = opcoes;
    if(render_options == NULL){
        this_p->render_options = render_options_default;
    }else{ 
        this_p->render_options = render_options;
    }
    this_p->action = action;
    this_p->selected = 0;
    this_p->i_janela = 0;
    this_p->lst = lst;
    this_p->nxt = nxt;

    return;
}

void live_page(Page * page){

    #ifdef _WIN32
        system("cls"); // Windows
    #else
        system("clear"); // Linux/Unix
    #endif

    
    if(page->selected < 0){
        page->selected = 0;
    }

    render(page);

    int lst_selecet = page->selected;

    listening_arrows(page);

    switch (page->selected)
    {
        case -3:
            exit(0);
            break;
        case -2:
            page->lst(NULL, page);
            
            break;
        case -1:
            page->nxt[lst_selecet](NULL, page);
            
            break;
    
    default:
        break;
    }   
}