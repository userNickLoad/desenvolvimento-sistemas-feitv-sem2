#pragma once

typedef struct User
{
    unsigned int id;
    unsigned int playlists;
    char name[50];
    char password[20];
} User;

typedef struct Video
{
    unsigned int id;
    char name[50];
    char desc[250];
    unsigned int duration; //00:00:00 -> segundos de  0 a 4.294.967.295
    unsigned int likes;
    unsigned int dislikes;
} Video;

typedef struct Video_user
{
    Video video;
    int like;
    int dislike;
} Video_user;


typedef struct Dislike
{
    unsigned int user_id;
    unsigned int video_id;
} Dislike;

typedef struct Like
{
    unsigned int user_id;
    unsigned int video_id;
} Like;

typedef struct Playlist
{
    unsigned int id;
    unsigned int user_id;
    unsigned int videos;
    char name[20];
    char description[250];
} Playlist;

typedef struct Playlist_Video
{
    unsigned int video_id;
    unsigned int playlist_id;
    unsigned int place;
} Playlist_Video;

typedef struct Videos_PV_DTO
{
    unsigned int place;
    Video *videos;
} Videos_PV_DTO;

typedef struct Playlis_Videos_PV_DTO
{
    Playlist *playlist;
    Videos_PV_DTO **videos;
} Playlis_Videos_PV_DTO;