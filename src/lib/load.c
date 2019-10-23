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
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "hook.h"
#include "util.h"

/**
 * Our job is very simple, attempt to load the cache directory
 * as binary input for the parser, if the cache directory actually
 * exists.
 */
bool aa_hook_context_load_cache(AaHookContext *self)
{
        struct stat st = { 0 };

        char *command[] = {
                NULL,                    /* parser_binary path */
                "-rB",                   /* Replace rules, read binary input */
                (char *)self->cache_dir, /* Target cache */
                "--abort-on-error",      /* We need error codes */
                NULL,                    /* Terminator */
        };

        command[0] = (char *)self->parser_binary;

        if (stat(self->cache_dir, &st) != 0) {
                if (errno == ENOENT) {
                        return true;
                }
                fprintf(stderr,
                        "Cannot access cache directory %s: %s\n",
                        self->cache_dir,
                        strerror(errno));
                return true;
        }

        /* Must be a directory */
        if (!S_ISDIR(st.st_mode)) {
                fprintf(stderr, "Cache directory is not a directory %s\n", self->cache_dir);
                return false;
        }

        /* Warn about load failure */
        if (aa_lsm_hook_exec_command(command) != 0) {
                fprintf(stderr,
                        "aa_hook_context_load_cache(): Unable to load cache %s\n",
                        self->cache_dir);
                return false;
        }

        return true;
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
