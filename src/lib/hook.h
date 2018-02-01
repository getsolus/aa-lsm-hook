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

#include <stdbool.h>

/**
 * Our AaHookContext is used to track some basic state
 */
typedef struct AaHookContext {
        const char *parser_binary; /**< Path to the discovered parser */
        const char *cache_dir;     /**<Our cache directory (defined at build time) */
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