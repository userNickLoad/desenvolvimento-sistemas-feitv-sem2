#include "pages.h"


int main(void)
{
    
    Page * crr_page = link_pages();

    while (1) {
        crr_page = live_page(crr_page);
        if (crr_page == NULL) return -1;

    }



    

    return 0;
}