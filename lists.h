#define str_node typedef struct node_str{ \
    struct node_str *nxt;          \
    char* value[50];                    \
    struct node_str *lst;              \
} node_str;

#define int_node typedef struct node_int { \
    struct node_int *nxt;          \
    int value;                    \
    struct node_int *lst;              \
} node_int;               \

# define nodex(type, name) \
    type##_node

#define listx(type, name) typedef struct {\
        struct node_##type *head;\
        struct node_##type * tail;\
        int size; \
} name##_list;\

#define init_listx(name)\
    name##_list *name##_init_list(){\
        name##_list *prt = (name##_list *)malloc(sizeof(name##_list));\
        if(prt == NULL){\
            printf("Erro ao alocar lista");\
            return NULL;\
        }   \
        prt->head = NULL;\
        prt->tail = NULL;\
        prt->size = 0;\
        return prt;\
}\

#define add_listx(type, name)\
    void add_##name(type value, name##_list *list){\
    \
        node_##type * new_node;\
        new_node->value = value;\
        new_node->nxt = NULL;\
        new_node->lst = NULL;\
    \
        if(list->size == 0){\
            list->size = 1;\
            list->head = new_node;\
            list->tail = new_node;\
            return;\
        }\
    \
        list->tail->nxt = new_node;\
        new_node->lst = list->tail;\
        list->tail = new_node;\
        list->size++;\
        return;\
    }

#define declara_list(type, name) \
    nodex(type, name)\
    listx(type, name) \
    init_listx(name) \
    add_listx(type, name)\


