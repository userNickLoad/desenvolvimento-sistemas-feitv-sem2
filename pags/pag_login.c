
#include  "../pages.h"

void page_login(void * _, Page *this_p) {
    build_page("\tlogin", "Area de login: \n\t\t+ Coloque o seu usuario e senha;", NULL, NULL, NULL, page_inicio, NULL, NULL, this_p);
}