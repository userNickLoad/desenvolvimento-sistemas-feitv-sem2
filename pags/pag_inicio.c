#include "../header.h"
#include "../pages.h"
#include "../lists.h"

void page_inicio(Page *this_p)
{
    char **ops = (char **)dina_prt_init(2);
    dinamic_inner_length(ops) = 10;

    ops = add_opcao("Entrar", ops, 20);
    ops = add_opcao("Cadastro", ops, 20);

    ChangePage *nxt = dina_chPage_init(2);

    add_nxt_pag(page_login, NULL,nxt)

    add_nxt_pag(page_signup, NULL,nxt)

    build_page(
        "inicio",
        "\tSeja bem vindo ao feitv!\n\tO seu catalogo de filmes pirat... GRATIS.",
        "\n\n\t- Como navegar:\n\t\t+ Use as setas do teclado;\n\t\t+ Para fechar o programa precione a tecla [e];",
        ops,
        nxt,
        NULL,
        NULL,
        NULL,
        NULL,
        this_p);
}