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
    //char *filter = getenv("SUB_FILTER");
    char *vent_address = getenv("VENT_ADDRESS");
    char *sink_address = getenv("SINK_ADDRESS");

    printf("Starting with proxy_address=%s\n", vent_address);

    zsock_t *pull = zsock_new_pull(vent_address);
    zsock_t *push = zsock_new_push(sink_address);

    char *string;

    while (1) {
        string = zstr_recv(pull);
        //printf("%s\n", string);
        zstr_send(push, string);
    }

    zstr_free(&string);
    zsock_destroy(&pull);
    return 0;
}
