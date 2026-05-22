#pragma once

#define generate_func_page_gettxt(name, question, type, info_var, txt_type, crr_page)      \
    void name(Page *this_p)                                                                \
    {                                                                                      \
        ChangePage *lst = malloc(sizeof(ChangePage));                                      \
                                                                                           \
        lst->free_all = NULL;                                                              \
        lst->build = crr_page;                                                             \
                                                                                           \
        build_page(#name, NULL, NULL, NULL, NULL, lst, NULL, NULL, name##_action, this_p); \
    }

#define generate_func_action_gettxt(name, question, type, info_var, txt_type, crr_page) \
    void name##_action(Page *this_p)                                                    \
    {                                                                                   \
        type *info = (type *)this_p->data.payload;                                      \
                                                                                        \
        insert_terminal(question, info->info_var, sizeof(info->info_var)-1, txt_type);    \
                                                                                        \
        this_p->selected = GO_BACK;                                                     \
    }

#define generate_gettxt_page(name, question, type, info_var, txt_type, crr_page)\
    generate_func_action_gettxt(name, question, type, info_var, txt_type, crr_page)\
    generate_func_page_gettxt(name, question, type, info_var, txt_type, crr_page)

