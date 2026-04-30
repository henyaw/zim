#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>

/*
 * Resolve path to absolute by calling realpath() on the parent directory
 * (which must exist) and appending the basename. The file itself need not
 * exist yet — useful when creating symlinks to future or reverse targets.
 */
static int resolve_path(const char *path, char *out, size_t outsz)
{
    char dir_buf[PATH_MAX], base_buf[PATH_MAX], real_dir[PATH_MAX];

    strncpy(dir_buf,  path, PATH_MAX - 1); dir_buf[PATH_MAX - 1]  = '\0';
    strncpy(base_buf, path, PATH_MAX - 1); base_buf[PATH_MAX - 1] = '\0';

    if (!realpath(dirname(dir_buf), real_dir))
        return -1;

    int n = snprintf(out, outsz, "%s/%s", real_dir, basename(base_buf));
    return (n > 0 && (size_t)n < outsz) ? 0 : -1;
}

static void usage(const char *prog)
{
    fprintf(stderr, "usage: %s <target>\n"
                    "       %s -r <linkpath>\n", prog, prog);
}

int main(int argc, char *argv[])
{
    int reverse = 0;
    const char *arg;

    if (argc == 3 && strcmp(argv[1], "-r") == 0) {
        reverse = 1;
        arg = argv[2];
    } else if (argc == 2 && argv[1][0] != '-') {
        arg = argv[1];
    } else {
        usage(argv[0]);
        return 1;
    }

    char cwd[PATH_MAX];
    if (!getcwd(cwd, sizeof(cwd))) {
        fprintf(stderr, "%s: getcwd: %s\n", argv[0], strerror(errno));
        return 1;
    }

    char target[PATH_MAX], link[PATH_MAX], base_buf[PATH_MAX];

    if (reverse) {
        /* link at resolved(arg), target is CWD/basename(arg) */
        if (resolve_path(arg, link, sizeof(link)) == -1) {
            fprintf(stderr, "%s: %s: %s\n", argv[0], arg, strerror(errno));
            return 1;
        }
        strncpy(base_buf, link, PATH_MAX - 1); base_buf[PATH_MAX - 1] = '\0';
        int n = snprintf(target, sizeof(target), "%s/%s", cwd, basename(base_buf));
        if (n <= 0 || (size_t)n >= sizeof(target)) {
            fprintf(stderr, "%s: path too long\n", argv[0]);
            return 1;
        }
    } else {
        /* link in CWD named basename(arg), target is resolved(arg) */
        if (resolve_path(arg, target, sizeof(target)) == -1) {
            fprintf(stderr, "%s: %s: %s\n", argv[0], arg, strerror(errno));
            return 1;
        }
        strncpy(base_buf, target, PATH_MAX - 1); base_buf[PATH_MAX - 1] = '\0';
        int n = snprintf(link, sizeof(link), "%s/%s", cwd, basename(base_buf));
        if (n <= 0 || (size_t)n >= sizeof(link)) {
            fprintf(stderr, "%s: path too long\n", argv[0]);
            return 1;
        }
    }

    if (symlink(target, link) == -1) {
        fprintf(stderr, "%s: %s -> %s: %s\n", argv[0], link, target, strerror(errno));
        return 1;
    }

    printf("%s -> %s\n", link, target);
    return 0;
}
