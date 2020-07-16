# Changelog:
___
**Version 0.1.4**

 - Remove systemd Unit dependency cycle caused by sysinit
 - Fix installation permissions for config files

___
**Version 0.1.3**

 - Fixed an issue where not all profiles are processed correctly, by giving the correct args to `apparmor_parser`.

___
**Version 0.1.2**

 - Fixes an issue where duplicate entries in the cache were being removed which actually harms reloading
 - Now reloads all directories in the cache since apparmor_parser doesn't know how.

___
**Version 0.1.1**

 - Update default configuration to work with snapd out of the box
 - Force aa-lsm-hook to wait to run until after snapd is started

___
**Version 0.1.0**

 - Full rewrite in Go for maintainability
 - Relicensed as Apache 2.0
 - New Features
   - Single executable
   - Configurable AppArmor Profile directories
	 - Better `snapd` co-integration
   - Support for AppArmor 2.13+ (switch to /var/cache/apparmor)
   
