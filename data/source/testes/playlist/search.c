#include "../../../../header.h"
#include "../../data.h"
#include "../../../../schema.h"
#include "../../../../lists.h"

int main(void)
{

    Response *res = search_for_playlists(0, "vi");
    printf("res->code: %u\n", res->code);
    printf("res->msg %s\n", res->msg);
    printf("res->data: %p\n", res->data);

    Playlist **playlists = res->data;
    for(int i = 0; i < dinamic_size(playlists); i++)
    {
        printf("\nTitulo: %0.50s\n", playlists[i]->name);
    }
    getchar();

}