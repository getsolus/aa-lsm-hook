# aa-lsm-hook

[![License](https://img.shields.io/badge/License-GPL%202.0-blue.svg)](https://opensource.org/licenses/GPL-2.0)

`aa-lsm-hook` is a simple project to provide distro integration points for the AppArmor LSM. The core goal is to simplify said integration by means of simple binaries that can be called on boot and during package transactions when the apparmor profiles reload. The net result is ahead-of-time compilation for AppArmor profiles (using `apparmor_parser`) and avoiding boot time regressions by loading only from a cache.

If the cache is invalid or unable to load, it will be recompiled, to ensure that the boot time performance hit is very rare (i.e. kernel or parser ABI changed without cache recompilation using those ABIs).

`aa-lsm-hook` is a [Solus project](https://solus-project.com/)

![logo](https://build.solus-project.com/logo.png)

## What/Why even

This project was created to provide a reusable AppArmor integration hook for any distribution that wants it. The issue in Solus was the use of legacy bash scripts to
manage the lifetime of AppArmor profiles, and massive boot time regressions. With local testing and a working AoT cache we go from 1.3s to around 8ms load on my local
test machine.

Note that this project isn't doing any "magic". Simply it provides well defined load and compile steps that wrap `apparmor_parser`, with some basic logic for garbage
collection and ensuring invalidated caches don't cause failed boots. The focus is on management and boot time.

## Integration

The `aa-lsm-hook.service` systemd unit should be enabled instead of any provided `apparmor.service` style init. This will ensure our hook is run during early boot,
which will load **only** from the binary cache. Note this requires that a binary cache is actually present, so be sure to compile it ahead of time.

To integrate the compilation step, you'll need a hook in your package manager/update process to execute `aa-lsm-hook-compile` when the apparmor paths change on
disk, i.e. `/etc/apparmor.d`.

## Quirks

Currently we have a special-case path to compile the `snapd` profiles from `/var/lib/snapd/apparmor/profiles` into the cache if they exist, which ensures the binary
load step will work properly on boot. Without this quirk/workaround, the snapd AppArmor profiles wouldn't be loaded **until** `snapd` is directly started and causes
broken snaps. With this .. quirk, everything works correctly, boot time is not regressed, and snaps work for those that have them.

## Authors

Copyright © 2018 Solus Project

`aa-lsm-hook` is available under the terms of the `GPL-2.0` license.
