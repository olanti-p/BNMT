#!/bin/sh

meson compile -C builddir && gdb -ex=r --args builddir/cataclysm-tiles --world BNME-Test --editor
