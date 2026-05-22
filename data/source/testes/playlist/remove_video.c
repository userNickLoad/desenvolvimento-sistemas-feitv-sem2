#include "../../../../header.h"
#include "../../data.h"
#include "../../../../schema.h"
#include "../../../../lists.h"

int main(void)
{

    Response *res = remove_video_playlist(6, 8);
    printf("res->code: %u\n", res->code);
    printf("res->msg %s\n", res->msg);
    printf("res->data: %p\n", res->data);

    getchar();

}