project(
    'aa-lsm-hook',
    ['c'],
    version: '0.0.1',
    license: [
        'GPL-2.0',
    ],
    default_options: [
        'c_std=c11',
        'prefix=/usr',
        'sysconfdir=/etc',
        'localstatedir=/var',
    ],
)

am_cflags = [
    '-fstack-protector',
    '-Wall',
    '-pedantic',
    '-Wstrict-prototypes',
    '-Wundef',
    '-fno-common',
    '-Werror-implicit-function-declaration',
    '-Wformat',
    '-Wformat-security',
    '-Werror=format-security',
    '-Wconversion',
    '-Wunused-variable',
    '-Wunreachable-code',
    '-W',
]

# Add our main flags
add_global_arguments(am_cflags, language: 'c')

# Get configuration bits together
path_prefix = get_option('prefix')
path_sysconfdir = join_paths(path_prefix, get_option('sysconfdir'))
path_mandir = join_paths(path_prefix, get_option('mandir'))
path_datadir = join_paths(path_prefix, get_option('datadir'))
path_sbindir = join_paths(path_prefix, get_option('sbindir'))
path_vardir = join_paths(path_prefix, get_option('localstatedir'), 'cache', meson.project_name())

# Grab systemd specifics
message('Asking pkg-config for systemd\'s directories')
dep_systemd = dependency('systemd')
path_systemd_unit_dir = dep_systemd.get_pkgconfig_variable('systemdsystemunitdir')

# Sort out config.h now
cdata = configuration_data()

# General options..
cdata.set_quoted('PACKAGE_NAME', meson.project_name())
cdata.set_quoted('PACKAGE_VERSION', meson.project_version())
cdata.set_quoted('PACKAGE_URL', 'https://solus-project.com')
cdata.set_quoted('AA_CACHE_DIR', path_vardir)

# Allow building with static binaries
with_static_binary = get_option('with-static-binary')

# Write config.h now
config_h = configure_file(
     configuration: cdata,
     output: 'config.h',
)
config_h_dir = include_directories('.')


# Build the project
subdir('src')
subdir('data')

report = [
    '    Build configuration:',
    '    ====================',
    '',
    '    prefix:                                 @0@'.format(path_prefix),
    '    datadir:                                @0@'.format(path_datadir),
    '    sysconfdir:                             @0@'.format(path_sysconfdir),
    '    mandir:                                 @0@'.format(path_mandir),
    '    sbindir:                                @0@'.format(path_sbindir),
    '    cache directory:                        @0@'.format(path_vardir),
    '    static binary:                          @0@'.format(with_static_binary),
    '    systemd unit directory:                 @0@'.format(path_systemd_unit_dir),
]

# Output some stuff to validate the build config
message('\n\n\n' + '\n'.join(report) + '\n\n')
