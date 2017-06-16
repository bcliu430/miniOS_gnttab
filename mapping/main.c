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

        domid_t my_friend;
        my_friend = 2;
        uint32_t ret; // typedef uint32_t grant_handle_t in grant_table.h
        ret = map(my_friend, 1, shared_page, 0);
        printf(DEBUG "ret: %d\n", ret);
        return 0;
}
/*
**    struct gnttab_map_grant_ref {
**        // IN parameters. 
**        uint64_t host_addr;
**        uint32_t flags;               // GNTMAP_* 
**        grant_ref_t ref;
**        domid_t  dom;
**        // OUT parameters. 
**        int16_t  status;              // GNTST_* 
**        grant_handle_t handle;
**        uint64_t dev_bus_addr;
**    };
**
*/
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
    grant_handle_t ret;
    /* Perform the map */
    HYPERVISOR_grant_table_op(GNTTABOP_map_grant_ref, &map_op,1);
    /* Check if it worked */


    if(map_op.status ==  GNTST_okay){
        /* Return the handle */
        *handle = map_op.handle;
        ret =  0;
        return ret;
    }
    else {
        ret = 0;
        if(map_op.status ==  GNTST_general_error){
            ret --;
            printf(DEBUG "general error\n");
        }

         
        if(map_op.status ==  GNTST_bad_domain){
            ret --;
            printf(DEBUG "bad_domain\n");
        }
            

        // GNTST_bad_gntref: 
        if(map_op.status ==  GNTST_bad_gntref){
            ret --;
            printf(DEBUG "bad_gntref\n");
        }

        // GNTST_bad_handle: 
        if(map_op.status ==  GNTST_bad_handle){
            ret --;
            printf(DEBUG "bad_handle\n");
        }

        //case GNTST_bad_virt_addr: 
        if(map_op.status ==  GNTST_bad_virt_addr){
            ret --;
            printf(DEBUG "bad_virt_addr\n");
        }

        //case GNTST_bad_dev_addr: 
        if(map_op.status ==  GNTST_bad_dev_addr){
            ret --;
            printf(DEBUG "bad_dev_addr\n");
        }

        //case GNTST_no_device_space: 
        if(map_op.status ==  GNTST_no_device_space){
            ret --;
            printf(DEBUG "no_device_space\n");
        } 
        

        //case GNTST_permission_denied: 
        if(map_op.status ==  GNTST_permission_denied){
            ret --;
            printf(DEBUG "permission_denied\n");
        }
        

        //case GNTST_bad_page: 
        if(map_op.status ==  GNTST_bad_page){
            ret --;
            printf(DEBUG "bad_page\n");
        }
        

        //case GNTST_bad_copy_arg: 
        if(map_op.status ==  GNTST_bad_copy_arg){
            ret --;
            printf(DEBUG "bad_copy_arg\n");
        }
        
    }
    return ret;
}
