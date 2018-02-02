hook_sources = [
    'clean.c',
    'hook.c',
    'util.c',
]

# Static utility library for the CLI entry points
hook_lib = static_library(
    'hook',
    sources: hook_sources,
    install: false,
    include_directories: [
        config_h_dir,
    ],
)

# Allow linking against the utility library
link_hook_lib = declare_dependency(
    link_with: hook_lib,
    include_directories: [
        include_directories('.'),
    ]
)