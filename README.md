# aa-lsm-hook

[![License](https://img.shields.io/badge/License-GPL%202.0-blue.svg)](https://opensource.org/licenses/GPL-2.0)

`aa-lsm-hook` is a simple project to provide distro integration points for the AppArmor LSM. The core goal is to simplify said integration by means of simple binaries that can be called on boot and during package transactions when the apparmor profiles reload. The net result is ahead-of-time compilation for AppArmor profiles (using `apparmor_parser`) and avoiding boot time regressions by loading only from a cache.

If the cache is invalid or unable to load, it will be recompiled, to ensure that the boot time performance hit is very rare (i.e. kernel or parser ABI changed without cache recompilation using those ABIs).

`aa-lsm-hook` is a [Solus project](https://solus-project.com/)

![logo](https://build.solus-project.com/logo.png)


## WORK IN PROGRESS

This repo is currently work in progress. Gimme a few hours.

## Authors

Copyright © 2018 Solus Project

`aa-lsm-hook` is available under the terms of the `GPL-2.0` license.
