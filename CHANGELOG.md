# Changelog:
___
**Version 0.1.1**

 - Remove systemd Unit dependency cycle caused by sysinit
 - Fix installation permissions for config files

___
**Version 0.1.0**

 - Full rewrite in Go for maintainability
 - Relicensed as Apache 2.0
 - New Features
   - Single executable
   - Configurable AppArmor Profile directories
	 - Better `snapd` co-integration
   - Support for AppArmor 2.13+ (switch to /var/cache/apparmor)
   
