#include "../header.h"
#include "../pages.h"
#include "../lists.h"



void page_inicio(void * _, Page * this_p){
    char **ops = dina_str_init(2);
    dinamic_inner_length(ops) = 10;

    ops = add_opcao("Entrar", ops);
    ops = add_opcao("Cadastro", ops);

    void ** nxt = dina_pags_init(2);

    dina_pags_add(page_login, nxt);
    dina_pags_add(page_signup, nxt);

    build_page("inicio", "\tSeja bem vindo ao feitv!\n\tO seu catalogo de filmes pirat... GRATIS.", "\n\n\t- Como navegar:\n\t\t+ Use as setas do teclado;\n\t\t+ Para fechar o programa precione a tecla [e];", ops, nxt, NULL,  NULL, NULL, this_p);

}