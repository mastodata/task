#include <czmq.h>
#include <zsock.h>
#include <zstr.h>
#include <zmsg.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include "hll.h"

int main() {
    char *filter = getenv("SUB_FILTER");
    char *vent_address = getenv("VENT_ADDRESS");
    char *sink_address = getenv("SINK_ADDRESS");

    printf("Starting with filter=%s and proxy_address=%s\n", filter, vent_address);

    if (NULL == filter)
    {
        filter = "";
    }
    zsock_t *sub = zsock_new_sub(vent_address, filter);
    zsock_t *push = zsock_new_push(sink_address);

    char *string;

    while (1) {
        string = zstr_recv(sub);
        //printf("%s\n", string);
        zstr_send(push, string);
    }

    zstr_free(&string);
    zsock_destroy(&sub);
    return 0;
}
