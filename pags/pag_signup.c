#include "../data/source/data.h"
#include "../schema.h"
#include "../pages.h"
#include "../header.h"
#include "utils.h"

generate_gettxt_page(sg_name, "Crie um nome para o seu usuario", User, name, validateCh_login, page_signup)

generate_gettxt_page(sg_password, "Crie uma senha a senha", User, password, validateCh_login, page_signup)

void clear_signup(Page *this_p)
{   
    if (this_p->data.payload != NULL)
    {
        User *user = this_p->data.payload;
        free(user);
        this_p->data.payload = NULL;
    }
    if (this_p->data.response != NULL){
        free_response(this_p->data.response, User)
        this_p->data.response = NULL;
    }
}

void signup_request(Page *this_p)
{   
    pop_Str(this_p->link);

    Response *res = this_p->data.response; 
    if (res == NULL){
        res = malloc(sizeof(Response));
        this_p->data.response = res;
    }

    res->data = NULL;
    res->code = 0;
    copy_str(res->msg, "\0");

    User *user = (User *) this_p->data.payload;

    if (user == NULL)
    {   
        res->code = 400;
        copy_str(res->msg, "Payload mal formulado");
        page_signup(this_p);
        return;
    }

    Response res_signup = signup(user->name, user->password);

    if(res_signup.code != 200){
        copy_struct(res, &res_signup, sizeof(Response));
        page_signup(this_p);
        return;
    }

    clear_signup(this_p);
    page_login(this_p);
}

void page_signup(Page *this_p)
{

    User *user;
    if (this_p->data.payload == NULL)
    {
        user = (User *)malloc(sizeof(User));
        user->id = 0;
        user->name[0] = '\0';
        user->password[0] = '\0';
        this_p->data.payload = user;
    }

    user = this_p->data.payload;

    char **ops = (char **)dina_prt_init(3);
    ChangePage *nxt = dina_chPage_init(3);
    ChangePage *lst = malloc(sizeof(ChangePage));

    char ops1[85];
    sprintf(ops1, "Crie um nome para o seu usuario: %s", user->name);
    ops = add_opcao(ops1, ops, sizeof(ops1));

    char ops2[50];
    sprintf(ops2, "Crie uma senha a senha: %s", user->password);
    ops = add_opcao(ops2, ops, sizeof(ops2));

    add_nxt_pag(sg_name, NULL, nxt)
    add_nxt_pag(sg_password, NULL, nxt)

    if (user->name[0] != '\0' && user->password[0] != '\0')
    {
        ops = add_opcao("Cadastrar", ops, 11);
        add_nxt_pag(signup_request, NULL, nxt)
    }

    lst->build = page_inicio;
    lst->free_all = clear_signup;

    char *question = NULL;

    if ((user->name[0] == '\0' || user->password[0] == '\0') && this_p->data.response == NULL)
        question = "\n\tNome ou senha nao preenchidos! Preencha os campos para dar continuidade.";

    if (this_p->data.response != NULL)
    {
        Response *res = (Response *)this_p->data.response;
        question = res->msg;
    }

    build_page("signup", "Area de cadastro: \n\t\t+ Coloque o seu usuario, senha e confirme a senha;\n\t\t + Valem apenas letras de [A-z], numeros de [0-9] e anderline [_]", question, ops,
               nxt, lst, NULL, NULL, NULL, this_p);
}
