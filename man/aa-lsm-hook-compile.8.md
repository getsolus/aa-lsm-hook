aa-lsm-hook-compile(8) -- compile AppArmor cache
================================================


## SYNOPSIS

`aa-lsm-hook-compile`


## DESCRIPTION

`aa-lsm-hook-compile` is a system binary used during system update transactions
to compile the AppArmor profiles into a cache for Ahead Of Time (AoT) compilation.
This cache is then read by `aa-lsm-hook-load(8)` on boot to maintain fast boot times
while still correctly loading AppArmor profiles early.

`aa-lsm-hook-compile` does not take any arguments, and is intended to be executed
by the service file.

## EXIT STATUS

On success, 0 is returned. A non-zero return code signals a failure.


## COPYRIGHT

 * Copyright © 2018 Solus Project, License: CC-BY-SA-3.0


## SEE ALSO

 * apparmor(7)
 * apparmor_parser(8)
 * aa-lsm-hook-load(8)
 * https://github.com/solus-project/aa-lsm-hook

## NOTES

Creative Commons Attribution-ShareAlike 3.0 Unported

 * http://creativecommons.org/licenses/by-sa/3.0/
