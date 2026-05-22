#include "../../../../header.h"
#include "../../data.h"
#include "../../../../schema.h"

int main(void)
{

    Response * res = delete_playlist(3, 0);
    printf("res->code: %u\n", res->code);
    printf("res->msg %s\n", res->msg);
    printf("res->data: %p\n", res->data);

}