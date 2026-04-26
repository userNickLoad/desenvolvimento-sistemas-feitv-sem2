#include  "../pages.h"

void page_signup(void *_, Page *this_p) {
    return build_page("\t- signup", "Area de cadastro: \n\t\t+ Coloque o seu usuario, senha e confirme a senha;", NULL, NULL, NULL, page_inicio, NULL, NULL, this_p);
}