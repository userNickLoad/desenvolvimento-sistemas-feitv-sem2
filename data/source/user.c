#include "data.h"

User *user_name_password(char *name, char *password)
{
    db_file(User, "r")

        User user,
        *res = NULL;

    for (int i = 0; i < User_amount; i++)
    {

        fscanf(fl_User, USER_SCAN_MASK, &user.id, &user.name, &user.password);
        if (compare_str(user.name, name))
        {

            if (compare_str(user.password, password))
            {
                res = malloc(sizeof(User));
                res->id = user.id;
                copy_str(res->name, user.name);
                copy_str(res->password, user.password);
            }
            break;
        }
    }
    fclose(fl_User);

    return res;
}

Response login(char *name, char *password)
{
    Response res;

    res.data = user_name_password(name, password);

    if (res.data == NULL)
    {
        res.code = 401;
        copy_str(res.msg, "Nome ou senha devem estar incorretos");
        return res;
    }

    res.code = 200;
    copy_str(res.msg, "Usuario logado");
    return res;
}

User *user_by_name(char *name)
{
    db_file(User, "r")

        User user,
        *res = NULL;

    for (int i = 0; i < User_amount; i++)
    {

        fscanf(fl_User, USER_SCAN_MASK, &user.id, &user.name, &user.password);
        if (compare_str(user.name, name))
        {
            res = malloc(sizeof(User));
            res->id = user.id;
            copy_str(res->name, user.name);
            copy_str(res->password, user.password);

            break;
        }
    }
    fclose(fl_User);

    return res;
}

User *append_user(char *name, char *password)
{
    db_file(User, "r+")

        fseek(fl_User, 0, SEEK_SET);
    fprintf(fl_User, HEADER_MASK, (User_amount + 1), User_line_size);

    fseek(fl_User, 0, SEEK_END);
    fprintf(fl_User, "%010u;", User_amount);

    char name_s[51];
    ajust_info(name_s, name, sizeof(name_s), ';');
    fwrite(name_s, sizeof(char), 51, fl_User);

    char password_s[21];
    ajust_info(password_s, password, sizeof(password_s), '\n');
    fwrite(password_s, sizeof(char), 21, fl_User);

    fclose(fl_User);

    User *res = malloc(sizeof(User));

    res->id = User_amount;
    copy_str(res->name, name);
    copy_str(res->password, password);

    return res;
}

Response signup(char *name, char *password)
{
    Response res;

    User *user = user_by_name(name);

    if (user != NULL)
    {
        res.code = 400;
        res.data = NULL;
        sprintf(res.msg, "Usuario %s ja existe", name);
        free(user);
        return res;
    }

    user = append_user(name, password);

    if (user == NULL)
    {
        res.code = 500;
        res.data = NULL;
        copy_str(res.msg, "Erro ao tentar criar o usuario");
        return res;
    }

    res.code = 200;
    res.data = user;
    copy_str(res.msg, "Usuario criado");
    ;

    return res;
}

// int main()
// {
//     Response res = login("Gigigames", "senha134");

//     printf("Status code: %u, Message: %s;\n", res.code, res.msg);

//     if(res.code == 200){
//         User *user = (User *) res.data;
//         printf("Id: %u, name: %s, password: %s;", user->id, user->name, user->password);
//     }
//     return 0;
// }