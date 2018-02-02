load_sources = [
    'main.c',
]

# Main hook used to load at boot
executable(
    'aa-lsm-hook-load',
    sources: load_sources,
    include_directories: [
        config_h_dir,
    ],
    dependencies: [
        link_hook_lib,
    ],
    install: true,
    install_dir: path_sbindir,
)