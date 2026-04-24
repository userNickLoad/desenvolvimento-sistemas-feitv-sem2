#include "../header.h"
#include "../pages.h"
#include "../lists.h"

char ** add_opcao(char* op, char **ops){
    char * crr = malloc(sizeof(char)*10);
    for(int i = 0; i < 10; i++){
        crr[i] = op[i];
        if(op[i] == '\0') break;
    }

    ops = dina_str_add(crr, ops);

    return ops;
}

Page * page_inicio(void){
    char **ops = dina_str_init(2);
    dinamic_inner_length(ops) = 10;

    ops = add_opcao("Entrar", ops);
    ops = add_opcao("Cadastro", ops);

    Page *page = build_page("inicio", "\tSeja bem vindo ao feitv!\n\tO seu catalogo de filmes pirat... GRATIS.", ops, render, NULL);

    return page;
}