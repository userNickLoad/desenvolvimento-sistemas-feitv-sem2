#pragma once

typedef struct User
{
    unsigned int id;
    char name[50];
    char password[20];
} User;

typedef struct Video
{
    unsigned int id;
    char name[50];
    char desc[250];
    unsigned int duration; //00:00:00 -> segundos de  0 a 4.294.967.295
} Video;

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
    char name[20];
    char description[250];
} Playlist;

typedef struct Playlist_Video
{
    unsigned int user_id;
    unsigned int playlist_id;
    short unsigned int place;
} Playlist_Video;
