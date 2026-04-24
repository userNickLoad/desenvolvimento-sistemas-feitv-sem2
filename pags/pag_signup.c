#include  "../pages.h"

Page * page_signup(void) {
    return build_page("\t- signup", "Area de cadastro: \n\t\t+ Coloque o seu usuario, senha e confirme a senha;", NULL, render, NULL);
}