/*
 * This file is part of aa-lsm-hook.
 *
 * Copyright © 2018 Solus Project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define _GNU_SOURCE

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>

#include "hook.h"
#include "util.h"

DEF_AUTOFREE(DIR, closedir)

/**
 * Build a single item into the target cache
 *
 * We take an incremental approach to ensure we don't compile anything we don't need to, and so that
 * everything has a chance to compile (rather than one broken definition stopping all compilation)
 */
static bool aa_hook_context_build_one(AaHookContext *self, const char *source_dir,
                                      const char *d_name)
{
        char path_buf[PATH_MAX] = { 0 };
        if (snprintf(path_buf, sizeof(path_buf), "%s/%s", source_dir, d_name) < 0) {
                return false;
        }

        char *command[] = {
                NULL,    /* parser_binary path */
                "-WQTL", /* Write cache, never load, never read cache, set cache location */
                (char *)self->cache_dir, /* Target cache */
                NULL,                    /* Path to file being parsed */
                NULL,                    /* Terminator */
        };

        command[0] = (char *)self->parser_binary;
        command[3] = path_buf;

        if (aa_lsm_hook_exec_command(command) != 0) {
                fprintf(stderr,
                        "aa_hook_context_build_one(): Compilation of %s failed\n",
                        path_buf);
                return false;
        }

        return true;
}

/**
 * Walk items in source_dir and build target cache items
 */
static bool aa_hook_context_build_dir(AaHookContext *self, const char *source_dir)
{
        autofree(DIR) *dir = NULL;
        struct dirent *ent = NULL;
        struct stat st = { 0 };
        int fd = 0;
        bool ret = true;

        /* We know ahead of time if the source directory exists. */
        dir = opendir(source_dir);
        if (!dir) {
                return false;
        }

        fd = dirfd(dir);

        /* Walk source dir for candidates */
        while ((ent = readdir(dir)) != NULL) {
                if (strlen(ent->d_name) == 1 && strcmp(ent->d_name, ".") == 0) {
                        continue;
                }
                if (strlen(ent->d_name) == 2 && strcmp(ent->d_name, "..") == 0) {
                        continue;
                }

                /* Can't rely on d_type being correct */
                if (fstatat(fd, ent->d_name, &st, AT_SYMLINK_NOFOLLOW) != 0) {
                        fprintf(stderr,
                                "Failed to read item %s/%s: %s\n",
                                source_dir,
                                ent->d_name,
                                strerror(errno));
                        continue;
                }

                /* Only want real files */
                if (!S_ISREG(st.st_mode)) {
                        continue;
                }

                /* Try compilation of all but mark bad exit */
                if (!aa_hook_context_build_one(self, source_dir, ent->d_name)) {
                        ret = false;
                }
        }

        return ret;
}

/**
 * Call out to our apparmor_parser helper and rebuild cache from
 * source tree items
 */
bool aa_hook_context_build_cache(AaHookContext *self)
{
        struct stat st = { 0 };
        bool ret = true;

        if (aa_unlikely(!self)) {
                return false;
        }

        /* First up, make sure our cache target exists */
        if (stat(self->cache_dir, &st) != 0) {
                if (mkdir(self->cache_dir, 00755) != 0) {
                        fprintf(stderr,
                                "Failed to create cache dir %s: %s\n",
                                self->cache_dir,
                                strerror(errno));
                        return false;
                }
        } else {
                /* Just in case someone derps it. */
                if (!S_ISDIR(st.st_mode)) {
                        fprintf(stderr,
                                "Cache directory is not actually a directory %s\n",
                                self->cache_dir);
                        return false;
                }
        }

        /* Attempt to build all source directories */
        for (size_t i = 0; i < self->n_source_dirs; i++) {
                if (!aa_hook_context_build_dir(self, self->source_dirs[i])) {
                        fprintf(stderr, "Compilation of %s failed\n", self->source_dirs[i]);
                        ret = false;
                }
        }

        return ret;
}

/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=8 tabstop=8 expandtab:
 * :indentSize=8:tabSize=8:noTabs=true:
 */