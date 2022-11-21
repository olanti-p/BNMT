# Bright Nights Mapgen Editor

## Compiling

1. Install Meson and Ninja. Instructions are available here: https://mesonbuild.com/Getting-meson.html
   On Linux, should be doable by running (`sudo apt install python3 ninja-build && pip3 install --user meson`)
2. Download and extract source code, or clone the repository (`git clone https://github.com/olanti-p/BNME.git`)
3. (Windows only) Open `x64 Native Tools Command Prompt for VS 20**` from Windows Start Menu
4. `cd` into code source folder
5. Configure meson build by running `meson setup builddir`. Add `--buildtype=release` for Release build (faster, but not as debuggable).
6. Compile project by running `meson compile -C builddir`

## Making binary disctribution
1. (Optionally) Strip debug symbols
```
strip builddir/cataclysm-tiles
```
2. Run python script
```
python build-scripts/make_release.py
```
