#include <stdio.h>
#include <unistd.h>
#include <mini-os/hypervisor.h>
#include <public/xen.h>
#include <xen/grant_table.h>
#include <xen/xen.h>

#define DEBUG "[DEBUG]: "

uint32_t shared_page[32];
grant_entry_t grant_table[1];

domid_t DOMID_FRIEND = DOMID_SELF+1;
void offer_page(void);

int main(void) {
        sleep(1);
        printf("Hello, world!\n");
        offer_page();
//        printf(DEBUG "end offer page\n");
        return 0;
}


void offer_page()
{
    uint16_t flags;
    /* Create the grant table */
    gnttab_setup_table_t setup_op;
    
    setup_op.dom = DOMID_SELF;
    setup_op.nr_frames = 1;
//    setup_op.frame_list = grant_table;
    set_xen_guest_handle(setup_op.frame_list, grant_table);

    HYPERVISOR_grant_table_op(GNTTABOP_setup_table, &setup_op, 1);

    /* Offer the grant */
    grant_table[0].domid = DOMID_FRIEND;
    grant_table[0].frame = *shared_page >> 12;
    flags = GTF_permit_access | GTF_reading | GTF_writing;
    grant_table[0].flags = flags;
    
//    printf(DEBUG "offered page\n");
}
