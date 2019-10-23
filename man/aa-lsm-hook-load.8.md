aa-lsm-hook-load(8) -- Load AppArmor cache
==========================================


## SYNOPSIS

`aa-lsm-hook-load`


## DESCRIPTION

`aa-lsm-hook-load` is a system binary used during the boot process to load the
AppArmor profiles from the cache into the kernel. If for some reason the load
should fail, it will fallback to recompilation of the caches and then attempt
a load for a second time.

`aa-lsm-hook-load` does not take any arguments, and is intended to be executed
by the service file.

## EXIT STATUS

On success, 0 is returned. A non-zero return code signals a failure.


## COPYRIGHT

 * Copyright © 2018-2019 Solus Project, License: CC-BY-SA-3.0


## SEE ALSO

 * apparmor(7)
 * apparmor_parser(8)
 * https://github.com/solus-project/aa-lsm-hook

## NOTES

Creative Commons Attribution-ShareAlike 3.0 Unported

 * http://creativecommons.org/licenses/by-sa/3.0/
