#!/bin/bash

# Credit to swupd developers: https://github.com/clearlinux/swupd-client

MANPAGES="man/aa-lsm-hook-compile.8 man/aa-lsm-hook-load.8"

for MANPAGE in ${MANPAGES}; do \
    ronn --roff < ${MANPAGE}.md > ${MANPAGE}; \
    ronn --html < ${MANPAGE}.md > ${MANPAGE}.html; \
done
