<div align="center">
    <h1><b>escape-puzzle</b>
    <h4>An escape puzzle game using raylib.</h4>
    <a href="https://github.com/bmoneill/escape-puzzle/actions/workflows/ci-linux.yml"><img src="https://github.com/bmoneill/escape-puzzle/actions/workflows/ci-linux.yml/badge.svg?branch=main" alt="Linux CI status" /></a>
    <a href="https://github.com/bmoneill/escape-puzzle/actions/workflows/ci-macos.yml"><img src="https://github.com/bmoneill/escape-puzzle/actions/workflows/ci-macos.yml/badge.svg?branch=main" alt="macOS CI status" /></a>
    <a href="https://github.com/bmoneill/escape-puzzle/actions/workflows/ci-windows.yml"><img src="https://github.com/bmoneill/escape-puzzle/actions/workflows/ci-windows.yml/badge.svg?branch=main" alt="Windows CI status" /></a>
    <a href="https://bmoneill.github.io/escape-puzzle"><img src="https://github.com/bmoneill/escape-puzzle/actions/workflows/doxygen.yml/badge.svg?branch=main" alt="Doxygen Status" /></a>
    <a href="https://github.com/bmoneill/escape-puzzle/actions/workflows/clang-format.yml"><img src="https://github.com/bmoneill/escape-puzzle/actions/workflows/clang-format.yml/badge.svg?branch=main" alt="Clang-format status" /></a>
</div>

## Table of Contents

- [Overview](#overview)
- [Building](#building)
- [Testing](#testing)
- [Documentation](#documentation)
- [Further Reading](#further-reading)
- [Bugs](#bugs
- [License](#license)

## Overview

This is a simple 2D escape puzzle game using procedurally generated levels,
written in C by Ben O'Neill and Danny Quang using
[raylib](https://www.raylib.com/) for graphics. The game is designed to be
simple and fun, with a focus on puzzle-solving and exploration.

In the future, web support, themed puzzles, and textures may be added, as well
as new puzzle types.

## Building

CMake version 3.31.6 or higher is required to build the project. If you are on Windows,
a Unix-like environment such as WSL or MinGW is required to build the project.
The project has been tested on Linux, macOS, and Windows. Dependencies are built
automatically.

To build the project, you can use the following commands:

```bash
git submodule init
git submodule update
cmake -S . -B build
cmake --build build
```

## Testing

```bash
cmake -S . -B build -DTEST=ON
cd build && ctest
```

## Documentation

Online documentation is available [here](https://bmoneill.github.io/escape-puzzle).

## Further Reading

- [Procedural Generation: An Overview](https://kentpawson123.medium.com/procedural-generation-an-overview-1b054a0f8d41)
- [raylib docs](https://www.raylib.com/cheatsheet/cheatsheet.html)

## Bugs

If you find a bug, submit an issue, PR, or email Ben at <ben@oneill.sh> with a description
and/or patch.

## License

Copyright (c) 2026 Ben O'Neill <ben@oneill.sh> and Danny Quang. This work is
released under the terms of the MIT License. See [LICENSE](LICENSE) for the
license terms.
