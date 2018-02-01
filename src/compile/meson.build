compile_sources = [
    'main.c',
]

# Main hook used to AoT compile
executable(
    'aa-lsm-hook-compile',
    sources: compile_sources,
    include_directories: [
        config_h_dir,
    ],
    install: true,
    install_dir: path_sbindir,
)