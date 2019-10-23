#!/bin/bash
set -e

VERSION="0.0.3"
NAME="aa-lsm-hook"

mv build/meson-dist/"${NAME}-${VERSION}.tar.xz" .
gpg --armor --detach-sign "${NAME}-${VERSION}.tar.xz"
gpg --verify "${NAME}-${VERSION}.tar.xz.asc"               
