# Collie Defence

## How To Play

1. Download the latest .gba rom file in the [release page](https://github.com/vayan/collie-defence/releases)
2. Open the rom with an emulator (e.g [mGBA](https://mgba.io/downloads.html))

## How to contribute

### Requirements

- devkitARM <https://devkitpro.org/wiki/Getting_Started>
- Butano <https://gvaliente.github.io/butano/getting_started.html>
- Python <https://www.python.org/downloads/>
- Poetry <https://python-poetry.org/docs/#installation>
- LDtk <https://ldtk.io/>
- mGBA <https://mgba.io/downloads.html>

### Instructions

Setup your GBA dev toolchain: https://gvaliente.github.io/butano/getting_started.html

make sure you have those envs set for Butano to find everything:

```bash
DEVKITPRO=/opt/devkitpro
DEVKITARM=$DEVKITPRO/devkitARM
LIBBUTANO=/path/to/butano/butano
```

And that you have Aseprite in your PATH, if you want to update the sprites, something like:
```bash
export PATH="$PATH:/Applications/Aseprite.app/Contents/MacOS"
```

You can also add mGBA to you path if you want `/Applications/mGBA.app/Contents/MacOS/`

### Install our dependencies

`poetry install`

### Generate levels code

`make levels`

### Build .gba rom file

`make -j8`

### Run rom file with debug log

`mgba collie-defence.gba -l 4`

### Generate graphics (optionals)

(Need Aseprite installed)

`make graphics`


# Licenses

Artwork and music are licenced under [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/)

All of the code, under [GPLv3](https://www.gnu.org/licenses/gpl-3.0.html).
