#ifndef Response

#include "../../schema.h"
#include "../../header.h"

#define USER_SCAN_MASK "%10u;%50[^;];%20[^\n]\n"
#define USER_PRINT_MASK "%010u;%-50s;%-20s\n"

#define VIDEO_SCAN_MASK "%10u;%50[^;];%250[^;];%10u;%10u;%10u\n"
#define VIDEO_PRINT_MASK "%010u;%-50s;%-250s;%010u;%010u;%010u\n"

#define VIDEO_IDX_NAME_SACN_MASK "%10u;%50[^;];%10u;%10u;%10u\n"
#define VIDEO_IDX_NAME_PRINT_MASK "%010u;%-50s;%010u;%010u;%010u\n"

//Amount;Ln_size
#define HEADER_MASK "%010u;%010u\n"
#define HEARDER_SIZE 23

//Amount;Ln_size;Head\n
#define IDX_HEADER_MASK "%010u;%010u;%010u\n"
#define IDX_HEARDER_SIZE 34



typedef struct Response
{
    int unsigned code;
    char msg[50];
    void *data;
} Response;

Response signup(char *name, char *password);

Response login(char *name, char *password);

void copy_struct(void *to, void *from, int size);

#define free_response(to_free, type)      \
    do                              \
    {                               \
        Response *res = to_free;    \
        if (res->data != NULL)      \
        {                           \
            type *user = res->data; \
            free(user);             \
        }                           \
        free(res);                  \
        to_free = NULL;             \
    } while (0);

void copy_str(char *to, char *from);

void ajust_info(char *stream, char *src, int size, char end_char);
int is_alfab(char *str1, char *str2, int size);

#define db_file(file, mode)                                              \
                                                                         \
    FILE *fl_##file;                                                     \
    char fl_path_##file[100];                                            \
    sprintf(fl_path_##file, "data/files/%s.txt", #file);                 \
    fl_##file = fopen(fl_path_##file, mode);                             \
                                                                         \
    if (fl_##file == NULL)                                               \
    {                                                                    \
        fprintf(stderr, "NAO FOI POSSIVEL ABRIR ");              \
        return NULL;                                                     \
    }                                                                    \
    int unsigned file##_amount, file##_line_size;                        \
    fseek(fl_##file, 0, SEEK_SET);                                                                     \
    fscanf(fl_##file, "%010u;%010u", &file##_amount, &file##_line_size); \
                                                                         \
    fseek(fl_##file, 22, SEEK_SET);

int compare_str(char *str1, char *str2);

#endif