#ifndef LISTS_H
#define LISTS_H

typedef struct Page Page;

#define nodex(type)              \
    typedef struct node_##type   \
    {                            \
        struct node_##type *nxt; \
        type value;              \
        struct node_##type *lst; \
    } node_##type;

#define listx(type, name)         \
    typedef struct                \
    {                             \
        struct node_##type *head; \
        struct node_##type *tail; \
        int size;                 \
    } name##_list;

#define init_listx(name)                                               \
    name##_list *name##_init_list()                                    \
    {                                                                  \
        name##_list *prt = (name##_list *)malloc(sizeof(name##_list)); \
        if (prt == NULL)                                               \
        {                                                              \
            printf("Erro ao alocar lista");                            \
            return NULL;                                               \
        }                                                              \
        prt->head = NULL;                                              \
        prt->tail = NULL;                                              \
        prt->size = 0;                                                 \
        return prt;                                                    \
    }

#define add_listx(type, name)                                               \
    void add_##name(type value, name##_list *list)                          \
    {                                                                       \
                                                                            \
        node_##type *new_node = (node_##type *)malloc(sizeof(node_##type)); \
        new_node->value = value;                                            \
        new_node->nxt = NULL;                                               \
        new_node->lst = NULL;                                               \
                                                                            \
        if (list->size == 0)                                                \
        {                                                                   \
            list->size = 1;                                                 \
            list->head = new_node;                                          \
            list->tail = new_node;                                          \
            return;                                                         \
        }                                                                   \
                                                                            \
        list->tail->nxt = new_node;                                         \
        new_node->lst = list->tail;                                         \
        list->tail = new_node;                                              \
        list->size++;                                                       \
        return;                                                             \
    }
#define pop_listx(type, name)              \
    void pop_##name(name##_list *list)     \
    {                                      \
                                           \
        if (list->size > 0)                \
        {                                  \
            node_##type *crr = list->tail; \
            list->tail = list->tail->lst;  \
            list->size -= 1;               \
            free(crr);                     \
        }                                  \
                                           \
        if (list->size == 0)               \
        {                                  \
            list->head = NULL;             \
        }                                  \
    }

#define free_listx(type, name)          \
    void free_##name(name##_list *list) \
    {                                   \
        while (list->size > 0)          \
        {                               \
            pop_##name(list);           \
        }                               \
        free(list);                     \
    }

#define declara_list_dec(type, name)                \
    nodex(type)                                     \
        listx(type, name) void                      \
        pop_##name(name##_list *list);              \
    void free_##name(name##_list *list);            \
    void pop_##name(name##_list *list);             \
    void add_##name(type value, name##_list *list); \
    name##_list *name##_init_list();

#define declara_list_impl(type, name) \
    init_listx(name)                  \
        add_listx(type, name)         \
            pop_listx(type, name)     \
                free_listx(type, name)

#define dinamic_size(list) *(((int *)list) - 2)

#define dinamic_pre_size(list) *(((int *)list) - 1)

#define dinamic_inner_length(list) *(((int *)list) - 3)

#define init_dinamic_list(type, name, size)                              \
    type *name##_init(int pre_size)                                      \
    {                                                                    \
        int *list = malloc((sizeof(type)) * pre_size + 3 * sizeof(int)); \
        if (list == NULL)                                                \
            return NULL;                                                 \
        list[1] = 0;                                                     \
        list[2] = pre_size;                                              \
        list[0] = size;                                                  \
        return (type *)(list + 3);                                       \
    }

#define add_dinamic_list(type, name)                                  \
    type *name##_add(type value, type *list)                          \
    {                                                                 \
                                                                      \
        if (dinamic_size(list) + 1 > dinamic_pre_size(list))          \
        {                                                             \
            type *new_list = name##_init(dinamic_pre_size(list) * 2); \
            for (int i = 0; i < dinamic_size(list); i++)              \
            {                                                         \
                new_list[i] = list[i];                                \
            }                                                         \
            free(list);                                               \
            list = new_list;                                          \
        }                                                             \
                                                                      \
        list[dinamic_size(list)] = value;                             \
                                                                      \
        dinamic_size(list) += 1;                                      \
                                                                      \
        return list;                                                  \
    }

#define dinamic_list_dec(type, name, inner_size) \
    type *name##_add(type value, type *list);    \
    type *name##_init(int pre_size);

#define dinamic_list_impl(type, name, inner_size) \
    init_dinamic_list(type, name, inner_size)     \
        add_dinamic_list(type, name)

typedef char *char_ptr;

typedef char **str_ptr;

declara_list_dec(char_ptr, Str);

dinamic_list_dec(char_ptr, dina_str, 20);


#endif