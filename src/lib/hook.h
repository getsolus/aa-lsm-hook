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

#pragma once

#define _GNU_SOURCE

#include <stdbool.h>
#include <stdint.h>

/* Maximum number of source dirs we'll support. */
#define MAX_SOURCE_DIRS 8

/**
 * Our AaHookContext is used to track some basic state
 */
typedef struct AaHookContext {
        const char *parser_binary; /**< Path to the discovered parser */
        const char *cache_dir;     /**<Our cache directory (defined at build time) */

        const char *source_dirs[MAX_SOURCE_DIRS]; /**<Currently known source directories */
        uint8_t n_source_dirs;                    /**<Number of source dirs we have */
} AaHookContext;

/**
 * Initialise the given context
 */
bool aa_hook_context_init(AaHookContext *context);

/**
 * Attempt hot load of the AoT cache
 */
bool aa_hook_context_load_cache(AaHookContext *context);

/**
 * Attempt construction of the cache
 */
bool aa_hook_context_build_cache(AaHookContext *context);

/**
 * Attempt to garbage collect the cache
 */
bool aa_hook_context_clean_cache(AaHookContext *context);

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