/*
 * zsymlink - minimal symlink utility
 * C99 + POSIX.1-2001, no dependencies beyond libc
 * Build: cc -std=c99 -O2 -o zsymlink zsymlink.c
 *        or: tcc -o zsymlink zsymlink.c   (smallest toolchain)
 */
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

static void usage(const char *prog)
{
    fprintf(stderr, "usage: %s <target> <linkname>\n", prog);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        usage(argv[0]);
        return 1;
    }

    const char *target   = argv[1];
    const char *linkname = argv[2];

    if (symlink(target, linkname) == -1) {
        fprintf(stderr, "%s: %s -> %s: %s\n",
                argv[0], linkname, target, strerror(errno));
        return 1;
    }

    return 0;
}
