compile_sources = [
    'main.c',
]

compile_link_args = []

if with_static_binary == true
    compile_link_args += '-static'
endif


# Main hook used to AoT compile
executable(
    'aa-lsm-hook-compile',
    sources: compile_sources,
    include_directories: [
        config_h_dir,
    ],
    link_args: compile_link_args,
    dependencies: [
        link_hook_lib,
    ],
    install: true,
    install_dir: path_sbindir,
)