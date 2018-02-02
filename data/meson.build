data_conf = configuration_data()
data_conf.set('SBINDIR', path_sbindir)

configure_file(
    input: 'aa-lsm-hook.service.in',
    output: 'aa-lsm-hook.service',
    configuration: data_conf,
    install_dir: path_systemd_unit_dir,
)