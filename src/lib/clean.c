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

/**
 * Long story short, we evaluate cache directory and source directories.
 * For every file in the cache not in the source directories, we nuke.
 */
bool aa_hook_context_clean_cache(AaHookContext *self)
{
        if (aa_unlikely(!self)) {
                return false;
        }

        return false;
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