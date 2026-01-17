/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "a3vm/board.h"

#define A3VM_VERSION "0.0.1"

static const char *firmware_rom = NULL;

static void
help(void)
{
    printf(
        "a3vm - A3 virtual machine\n"
        "-----------------------------\n"
        "[-h]   Display this help menu\n"
        "[-v]   Display the a3vm version\n"
        "[-f]   Path to firmware ROM\n"
    );
}

static void
version(void)
{
    printf(
        "Copyright (c) 2026, Ian Moffett\n"
        "Provided under the BSD-3 clause\n"
        "-------------------------------\n"
        "A3VM version v%s\n",
        A3VM_VERSION
    );
}

static void
vm_run(void)
{
    struct mainboard board;

    if (mainboard_init(&board) < 0) {
        perror("mainboard_init");
        return;
    }
}

int
main(int argc, char **argv)
{
    int opt;

    while ((opt = getopt(argc, argv, "hvf:")) != -1) {
        switch (opt) {
        case 'h':
            help();
            return -1;
        case 'v':
            version();
            return -1;
        case 'f':
            firmware_rom = strdup(optarg);
            break;
        }
    }

    if (firmware_rom == NULL) {
        printf("fatal: expected firmware ROM path\n");
        help();
        return -1;
    }

    vm_run();
    return 0;
}
