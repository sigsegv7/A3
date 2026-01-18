/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "as3/state.h"
#include "as3/parser.h"

#define AS3_VERSION "0.0.1"

static void
help(void)
{
    printf(
        "as3 - the A3 assembler\n"
        "-----------------------------\n"
        "[-h]   Display this help menu\n"
        "[-v]   Display the version\n"
    );
}

static void
version(void)
{
    printf(
        "Copyright (c) 2026, Ian Moffett\n"
        "Provided under the BSD-3 clause\n"
        "-------------------------------\n"
        "AS3 v%s\n",
        AS3_VERSION
    );
}

static int
assemble(const char *path)
{
    struct as3_state state;

    if (path == NULL) {
        return -EINVAL;
    }

    if (as3_state_init(&state, path) < 0) {
        perror("as3_state_init");
        return -1;
    }

    if (parser_parse(&state) < 0) {
        return -1;
    }

    as3_state_destroy(&state);
    return 0;
}

int
main(int argc, char **argv)
{
    int opt;

    while ((opt = getopt(argc, argv, "hv")) != -1) {
        switch (opt) {
        case 'h':
            help();
            return -1;
        case 'v':
            version();
            return -1;
        }
    }

    while (optind < argc) {
        if (assemble(argv[optind++]) < 0) {
            return -1;
        }
    }

    return 0;
}
