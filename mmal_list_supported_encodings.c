/*
 * Copyright (c) 2017 Sugizaki Yukimasa (ysugi@idein.jp)
 * All rights reserved.
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */

#include <interface/mmal/mmal.h>
#include <interface/mmal/mmal_logging.h>
#include <interface/mmal/util/mmal_util.h>
#include <bcm_host.h>
#include <stdio.h>
#include <stdlib.h>


#define _check(x) do { \
    MMAL_STATUS_T status = (x); \
    if (status != MMAL_SUCCESS) { \
        mmal_log_error("%s:%d: %s: %s(%d)\n", __FILE__, __LINE__, \
                #x, mmal_status_to_string(status), status); \
        exit(EXIT_FAILURE); \
    } \
} while (0)


static void list_supported_encodings(MMAL_PORT_T *port)
{
    char buf[5];
    MMAL_PARAMETER_HEADER_T *hdrp = NULL;
    MMAL_STATUS_T status = 0;
    uint32_t *encodings = NULL;
    unsigned i;

    printf("\t* Minimum number of buffers: %u\n", port->buffer_num_min);
    printf("\t* Minimum size of buffers: %u\n", port->buffer_size_min);
    printf("\t* Minimum alignment of buffers: %u\n",
            port->buffer_alignment_min);
    printf("\t* Number of buffers recommended: %u\n",
            port->buffer_num_recommended);
    printf("\t* Size of buffers recommended: %u\n",
            port->buffer_size_recommended);

    printf("\t* Capability:");
    if (port->capabilities & MMAL_PORT_CAPABILITY_PASSTHROUGH)
        printf(" passthrough");
    if (port->capabilities & MMAL_PORT_CAPABILITY_ALLOCATION)
        printf(" allocation");
    if (port->capabilities & MMAL_PORT_CAPABILITY_SUPPORTS_EVENT_FORMAT_CHANGE)
        printf(" event_format_change");
    printf("\n");

    printf("\t* Default encoding: %s\n",
            mmal_4cc_to_string(buf, sizeof(buf), port->format->encoding));

    hdrp = mmal_port_parameter_alloc_get(port,
            MMAL_PARAMETER_SUPPORTED_ENCODINGS, 0, &status);
    _check(status);
    if (hdrp == NULL)
        return;

    printf("\t* Supported encodings:\n");
    encodings = (uint32_t*) ((uint8_t*) hdrp + sizeof(*hdrp));
    for (i = 0; i < (hdrp->size - sizeof(*hdrp)) / 4; i ++)
        printf("\t\t%2u. %s\n", i+1,
                mmal_4cc_to_string(buf, sizeof(buf), encodings[i]));

    mmal_port_parameter_free(hdrp);
}

int main(int argc, char *argv[])
{
    unsigned i;
    MMAL_COMPONENT_T *comp = NULL;

    if (argc != 1 + 1) {
        printf("Usage: %s COMPONENT_NAME\n", argv[0]);
        printf("Example: %s vc.ril.camera\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    bcm_host_init();
    _check(mmal_component_create(argv[1], &comp));

    printf("* Control port\n");
    list_supported_encodings(comp->control);
    for (i = 0; i < comp->input_num; i ++) {
        printf("* Input port %u\n", i);
        list_supported_encodings(comp->input[i]);
    }
    for (i = 0; i < comp->output_num; i ++) {
        printf("* Output port %u\n", i);
        list_supported_encodings(comp->output[i]);
    }

    _check(mmal_component_destroy(comp));
    bcm_host_deinit();
    return 0;
}
