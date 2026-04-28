typedef struct User
{
    int unsigned id;
    char name[50];
    char password[20];
} User;

typedef struct Video
{
    int unsigned id;
    char name[50];
    char desc[250];
    int unsigned duration; //00:00:00 -> segundos de  0 a 4.294.967.295
} Video;

typedef struct Dislike
{
    int unsigned user_id;
    int unsigned video_id;
} Dislikes;

typedef struct Like
{
    int unsigned user_id;
    int unsigned video_id;
} Like;

typedef struct Playlist
{
    int unsigned id;
    int unsigned user_id;
    char name[20];
    char description[250];
} Playlist;

typedef struct Playlist_Video
{
    int unsigned user_id;
    int unsigned playlist_id;
    short int unsigned place;
} Playlist_Video;