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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "hook.h"
#include "util.h"

int main(__aa_unused__ int argc, __aa_unused__ char **argv)
{
        AaHookContext ctx = { 0 };

        if (geteuid() != 0) {
                fputs("This program must be run as root\n", stderr);
                return EXIT_FAILURE;
        }

        /* Perform discovery */
        if (!aa_hook_context_init(&ctx)) {
                return EXIT_FAILURE;
        }

        /* Perform load */
        if (aa_hook_context_load_cache(&ctx)) {
                return EXIT_SUCCESS;
        }

        fputs("Attempting recompilation of cache due to failed loading\n", stderr);

        /* Try recompilation because it failed to load (abi change perhaps) */
        if (!aa_hook_context_build_cache(&ctx)) {
                fputs("Recompilation failed, aborting\n", stderr);
                return EXIT_FAILURE;
        }

        fputs("Attempting load of recompiled cache\n", stderr);

        /* At this point its all up to a second load. */
        return aa_hook_context_load_cache(&ctx) ? EXIT_SUCCESS : EXIT_FAILURE;
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