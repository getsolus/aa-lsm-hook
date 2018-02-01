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
#include "config.h"
#include "util.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * Almost all distros will hit on the very first item, if they follow
 * either the FHS or employ usr-merge strategy. We deliberately codify
 * the path to ensure we can handle future changes in the location of
 * the parser as well as to ignore potentially incorrect/malicious
 * locations in /usr/local or via sudo with the PATH allowed to propagate.
 */
static const char *apparmor_binaries[] = {
        "/sbin/apparmor_parser",
        "/usr/sbin/apparmor_parser",
        "/bin/apparmor_parser",
        "/usr/bin/apparmor_parser",
};

/**
 * For now we only support /etc/apparmor.d as a source directory.
 * In future we intend to make this support stateless paths, pending work
 * upstream.
 */
static const char *apparmor_directories[] = {
        "/etc/apparmor.d",
};

bool aa_hook_context_init(AaHookContext *self)
{
        if (!self) {
                return false;
        }

        /* Locate system apparmor binary (executable in predetermined path) */
        for (size_t i = 0; i < ARRAY_SIZE(apparmor_binaries); i++) {
                const char *p = apparmor_binaries[i];

                if (access(p, X_OK) == 0) {
                        self->parser_binary = p;
                        break;
                }
        }

        if (aa_unlikely(!self->parser_binary)) {
                fputs("Unable to locate apparmor_parser\n", stderr);
                return false;
        }

        /* Locate system apparmor directories */
        for (size_t i = 0; i < ARRAY_SIZE(apparmor_directories); i++) {
                const char *app_dir = apparmor_directories[i];
                struct stat st = { 0 };

                if (stat(app_dir, &st) != 0) {
                        continue;
                }

                if (!S_ISDIR(st.st_mode)) {
                        continue;
                }

                /* Got a dir */
                self->source_dirs[self->n_source_dirs] = app_dir;
                ++self->n_source_dirs;
        }

        /* Just taken from build time */
        self->cache_dir = AA_CACHE_DIR;

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