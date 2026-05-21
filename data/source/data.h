#ifndef Response

#ifdef _WIN32
#include <io.h>
#define resize_fl(fl, new_size) \
    _chsize(_fileno(fl), new_size);
#else
#include <unistd.h>
#define resize_fl(fl, new_size) \
    ftruncate(fileno(fl), novo_tamanho);
#endif

#include "../../schema.h"
#include "../../header.h"

#define USER_SCAN_MASK "%10u;%10u;%50[^;];%20[^\r\n]\r\n"
#define USER_PRINT_MASK "%010u;%010u;%-50s;%-20s\r\n"
#define USER_SCAN(line, var)\
    sscanf(line, USER_SCAN_MASK, &var.id, &var.playlists, var.name, var.password);\
    trim(var.name, 50);\
    trim(var.password, 20);


#define VIDEO_SCAN_MASK "%10u;%50[^;];%250[^;];%10u;%10u;%10u\r\n"
#define VIDEO_PRINT_MASK "%010u;%-50s;%-250s;%010u;%010u;%010u\r\n"
#define VIDEO_SCAN(line, var)\
    sscanf(line, VIDEO_SCAN_MASK, &var.id, var.name, var.desc, &var.duration,  &var.likes,  &var.dislikes);\
    trim(var.name, 50);\
    trim(var.desc, 250);

#define LIKE_PRINT_MASK "%010u;%010u\r\n"
#define LIKE_SCAN_MASK "%10u;%10u\r\n"
#define LIKE_SCAN(line, var)\
    sscanf(line, LIKE_SCAN_MASK, &var.user_id, &var.video_id);

#define PLAYLIST_SCAN_MASK "%10u;%10u;%10u;%50[^;];%250[^\r\n]\r\n"
#define PLAYLIST_PRINT_MASK "%010u;%010u;%010u;%-50s;%-250s\r\n"
#define PLAYLIST_SCAN(line, var)\
    sscanf(line, PLAYLIST_SCAN_MASK, &var.id, &var.user_id, &var.videos, var.name, var.description);\
    trim(var.name, 50);\
    trim(var.description, 250);
#define PLAYLIST_PRINT(line, var)\
    snprintf(line, 336, PLAYLIST_PRINT_MASK, var.id, var.user_id, var.videos, var.name, var.description);


#define PLAYLIST_VID_SCAN_MASK "%10u;%10u;%10u\r\n"
#define PLAYLIST_VID_PRINT_MASK "%010u;%010u;%010u\r\n"
#define PLAYLIST_VID_SCAN(line, var)\
    sscanf(line, PLAYLIST_VID_SCAN_MASK, &var.video_id, &var.playlist_id, &var.place);
#define PLAYLIST_VID_PRINT(line, var)\
    snprintf(line, 34, PLAYLIST_VID_PRINT_MASK, var.video_id, var.playlist_id, var.place);


#define HEADER_MASK "%010u;%010u;%010u\r\n"
#define HEARDER_SIZE 34

typedef struct Response
{
    int unsigned code;
    char msg[50];
    void *data;
} Response;

Response *signup(char *name, char *password); // MEXIDO

Response *login(char *name, char *password);// MEXIDO

Response *search_for_videos(char *title); // MEXIDO

Response *video_user_search(unsigned int user_id, unsigned int video_id); // MEXIDO

Response *handle_like(unsigned int user_id, unsigned int video_id);

Response *handle_dislike(unsigned int user_id, unsigned int video_id);

Response *create_playlist(unsigned int user_id, char * title, char * description); //APROVADO

Response *delete_playlist(unsigned int playlist_id); //APROVADO

Response *search_for_playlists(unsigned int user_id, char * title);

Response *add_video_playlist(unsigned int playlist_id, unsigned int video_id);

Response *remove_video_playlist(unsigned int playlist_id, unsigned int video_id);

Response *videos_from_playlist(unsigned int playlist_id);

void copy_struct(void *to, void *from, int size);

void erase_line(char *arq_title, int (*verify)(char *, void *), unsigned amount_erase, void *data_filter);

void append_line(char *arq_title, int auto_incremente_id, char *fmt, ...);

void **read_fl(char *arq_title, void (*save)(char *, void *, void **), unsigned int amount_save, void *data_filter);

void update_fl(char *arq_title, int (*alter)(char *, void *), unsigned int amount_alter, void *data_filter);

int compare_titles(char *title1, char *title2, int size);

void trim(char *str, int size);

void trim_str(char *str);

int compare_str(char *str1, char *str2);

#define free_response(to_free, type) \
    do                               \
    {                                \
        Response *res = to_free;     \
        if (res->data != NULL)       \
        {                            \
            type *user = res->data;  \
            free(user);              \
        }                            \
        free(res);                   \
        to_free = NULL;              \
    } while (0);

#define free_prt_list(to_free, size) \
    do                                     \
    {                                      \
        for (int i = 0; i < size; i++)     \
        {                                  \
            free(to_free[i]);      \
        }                                  \
    } while (0);

void copy_str(char *to, char *from);

void ajust_info(char *stream, char *src, int size, char end_char);
int is_alfab(char *str1, char *str2, int size);


#endif