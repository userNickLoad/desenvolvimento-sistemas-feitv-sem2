
#include  "../pages.h"

Page * page_login(void) {
    return build_page("\t- login", "Area de login: \n\t\t+ Coloque o seu usuario e senha;", NULL, render, NULL);
}