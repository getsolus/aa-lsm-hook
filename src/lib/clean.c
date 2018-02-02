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

#include "hook.h"
#include "util.h"

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

DEF_AUTOFREE(DIR, closedir)

/**
 * Return true if we find the name within a source directory.
 */
static bool aa_hook_context_has_cache(AaHookContext *self, const char *d_name)
{
        char search_name[PATH_MAX] = { 0 };
        struct stat st = { 0 };

        for (size_t i = 0; i < self->n_source_dirs; i++) {
                const char *source_dir = self->source_dirs[i];
                if (snprintf(search_name, sizeof(search_name), "%s/%s", source_dir, d_name) < 0) {
                        return false;
                }

                /* Look for regular with the same name */
                if (stat(search_name, &st) != 0) {
                        continue;
                }

                if (S_ISREG(st.st_mode)) {
                        return true;
                }
        }

        return false;
}

/**
 * Long story short, we evaluate cache directory and source directories.
 * For every file in the cache not in the source directories, we nuke.
 */
bool aa_hook_context_clean_cache(AaHookContext *self)
{
        autofree(DIR) *dir = NULL;
        struct dirent *ent = NULL;
        bool ret = false;
        int fd = 0;

        if (aa_unlikely(!self)) {
                return false;
        }

        /* If the directory doesn't exist yet, it isn't fatal */
        dir = opendir(self->cache_dir);
        if (!dir) {
                if (errno == ENOENT) {
                        return true;
                }
                return false;
        }

        /* Assume success at this point */
        fd = dirfd(dir);
        ret = true;

        while ((ent = readdir(dir)) != NULL) {
                if (strlen(ent->d_name) == 1 && strcmp(ent->d_name, ".") == 0) {
                        continue;
                }

                if (strlen(ent->d_name) == 2 && strcmp(ent->d_name, "..") == 0) {
                        continue;
                }

                if (aa_hook_context_has_cache(self, ent->d_name)) {
                        continue;
                }

                /* Continue even if the wipe fails so we clean some stuff up */
                if (unlinkat(fd, ent->d_name, 0) != 0) {
                        fprintf(stderr,
                                "Unable to unlink() %s/%s: %s\n",
                                self->cache_dir,
                                ent->d_name,
                                strerror(errno));
                        ret = false;
                        continue;
                }

                /* Print for benefit of calling tool */
                fprintf(stdout, "aa_hook_context_clean_cache(): Removed %s\n", ent->d_name);
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