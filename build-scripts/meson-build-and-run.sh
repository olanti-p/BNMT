#!/bin/sh

mold -run meson compile -C builddir && builddir/cataclysm-tiles --world BNME-Test --editor
