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

#include <stdio.h>
#include <stdlib.h>

#include "hook.h"

int main(int argc, char **argv)
{
        AaHookContext ctx = { 0 };

        /* Perform discovery */
        if (!aa_hook_context_init(&ctx)) {
                return EXIT_FAILURE;
        }

        /* Perform build */
        if (!aa_hook_context_build_cache(&ctx)) {
                return EXIT_FAILURE;
        }

        /* Now clean */
        return aa_hook_context_clean_cache(&ctx) ? EXIT_SUCCESS : EXIT_FAILURE;
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