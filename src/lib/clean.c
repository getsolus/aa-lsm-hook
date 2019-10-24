/*
 * This file is part of aa-lsm-hook.
 *
 * Copyright © 2018-2019 Solus Project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define _GNU_SOURCE

#include <errno.h>
#include <fts.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "hook.h"
#include "util.h"

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
        bool ret = true;
        FTSENT *curr_entry = NULL;
        FTS *file_system = NULL;

        if (aa_unlikely(!self)) {
                return false;
        }

        char *path[] = { (char *)self->cache_dir, NULL };
        file_system = fts_open(path, FTS_PHYSICAL | FTS_NOSTAT, NULL);

        if (file_system == NULL) {
                return false;
        }

        while ((curr_entry = fts_read(file_system)) != NULL && errno == 0) {
                unsigned short path_info = curr_entry->fts_info;

                if (path_info == FTS_DNR || path_info == FTS_ERR || path_info == FTS_NS) {
                        /* An error related to the file/dir occured. Signal it and move on */
                        fprintf(stderr,
                                "Unable to read %s: %s\n",
                                curr_entry->fts_path,
                                strerror(curr_entry->fts_errno));
                        ret = false;
                        continue;
                }

                if (path_info != FTS_F && path_info != FTS_NSOK && path_info != FTS_DP) {
                        continue;
                }
                if (path_info == FTS_F || path_info == FTS_NSOK) {
                        /* Ignore the .features file and profiles that are still installed */
                        if (strcmp(curr_entry->fts_name, ".features") == 0 ||
                            aa_hook_context_has_cache(self, curr_entry->fts_name)) {
                                continue;
                        }
                }
                if (remove(curr_entry->fts_path) != 0 && errno != ENOTEMPTY) {
                        /* It's OK if dir was not empty, but print other error cases */
                        fprintf(stderr,
                                "Unable to remove() %s: %s\n",
                                curr_entry->fts_path,
                                strerror(errno));
                        ret = false;
                        continue;
                }
                /* Removal succeedeed. Print it for benefit of calling tool */
                fprintf(stdout,
                        "aa_hook_context_clean_cache(): Removed %s\n",
                        curr_entry->fts_name);
        }

        if (errno != 0) {
                /* A blocking error occured */
                fprintf(stderr,
                        "Fatal error while reading %s: %s\n",
                        curr_entry->fts_path,
                        strerror(errno));
                ret = false;
        }

        fts_close(file_system);
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
