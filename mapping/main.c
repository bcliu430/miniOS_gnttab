#include <stdio.h>
#include <unistd.h>
#include <mini-os/hypervisor.h>
#include <public/xen.h>
#include <xen/grant_table.h>
#include <xen/xen.h>

#define DEBUG "[DEBUG]: "
/*
** TODO:
**  find out entry!!
*/
uint32_t shared_page[32];
grant_entry_t grant_table[1];


grant_handle_t map(domid_t friend, 
        unsigned int entry, 
        void * shared_page, 
        grant_handle_t * handle);

int main(void) {
        sleep(1);
        printf("Hello, world!\n");

        domid_t my_friend = DOMID_SELF+1;
        uint32_t ret; // typedef uint32_t grant_handle_t in grant_table.h
        ret = map(my_friend, 1, shared_page, 0);
        printf(DEBUG "ret: %d\n", ret);
        return 0;
}

grant_handle_t map(domid_t friend, 
        unsigned int entry, 
        void * shared_page, 
        grant_handle_t * handle)
{
    /* Set up the mapping operation */
    gnttab_map_grant_ref_t map_op;
    map_op.host_addr = shared_page;
    map_op.flags = GNTMAP_host_map;
    map_op.ref = entry;
    map_op.dom = friend;
    /* Perform the map */
    HYPERVISOR_grant_table_op(GNTTABOP_map_grant_ref, &map_op,1);
    /* Check if it worked */
    if(map_op.status != GNTST_okay)
    {
        return -1;
    }
    else 
    {
        /* Return the handle */
        *handle = map_op.handle;
        return 0;
    }
}
