load_sources = [
    'main.c',
]

load_link_args = []

if with_static_binary == true
    load_link_args += '-static'
endif

# Main hook used to load at boot
executable(
    'aa-lsm-hook-load',
    sources: load_sources,
    include_directories: [
        config_h_dir,
    ],
    link_args: load_link_args,
    dependencies: [
        link_hook_lib,
    ],
    install: true,
    install_dir: path_sbindir,
)