# PipeWalker Ported to Mac

I don't know what else to say, I just ported my favouite game to Mac


## PipeWalker

This is a puzzle game in which you need to combine the components into
a single circuit: connect all computers to a network server, bring water
to the taps, etc.

![Screenshot](https://raw.githubusercontent.com/artemsen/pipewalker/master/.github/screenshot.png)

## Install

[![Packaging status](https://repology.org/badge/tiny-repos/pipewalker.svg)](https://repology.org/project/pipewalker/versions)

List of supported distributives can be found on the [Repology page](https://repology.org/project/pipewalker/versions).

## Build

To build the project you will need:
- C++ compiler with support for the C++20 standard;
- Meson build system;
- SDL3 library.

```shell
sudo apt install build-essential libsdl3-dev
meson setup my_build_dir
meson compile -C my_build_dir
meson install -C my_build_dir
```
