/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
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
dump_ram(struct mainboard *mbp)
{
    const size_t MAX_WIDTH = 16;

    if (mbp == NULL) {
        return;
    }

    printf("[*] dumping 128 bytes of RAM...\n");
    for (size_t i = 0; i < 128; ++i) {
        if ((i % MAX_WIDTH) == 0) {
            if (i > 0) printf("\n");
            printf("[%08zX]: ", i);
        }
        printf("%02X ", mbp->ram[i] & 0xFF);
    }

    printf("\n");
}

static void
vm_run(void)
{
    struct mainboard board;
    char *fw_buf;
    size_t fw_size;
    int fw_fd, error;

    if (mainboard_init(&board) < 0) {
        perror("mainboard_init");
        return;
    }

    if ((fw_fd = open(firmware_rom, O_RDONLY)) < 0) {
        mainboard_power_off(&board);
        perror("open");
        return;
    }

    /* Map the buffer so that we can read it */
    fw_size = lseek(fw_fd, 0, SEEK_END);
    lseek(fw_fd, 0, SEEK_SET);
    fw_buf = mmap(
        NULL,
        fw_size,
        PROT_READ,
        MAP_SHARED,
        fw_fd,
        0
    );

    /* Write the firmware to memory */
    error = mainboard_ram_write(
        &board,
        0x000000,
        fw_buf,
        fw_size
    );

    if (error < 0) {
        printf("failed to write firmware to RAM\n");
        perror("mainboard_ram_write");
        goto done;
    }

    printf("[*] firmware loaded\n");
    dump_ram(&board);
done:
    munmap(fw_buf, fw_size);
    close(fw_fd);
    mainboard_power_off(&board);
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
