# DESDE-CERO-SF

A modified fork of [vdinovi/Starfox64](https://github.com/vdinovi/Starfox64) — a Starfox 64 recreation built with C++ and OpenGL.

## Changes

- Purple sky and green sea color scheme
- CMake compatibility fixes for modern toolchains

## Requirements

- CMake >= 3.5
- GLFW3
- GLM
- OpenGL

### macOS (Homebrew)

```bash
brew install cmake glfw glm
```

## Build & Run

```bash
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)
./starfox64
```

## Controls

Fly the Arwing around the level, shoot enemies, and dodge obstacles — just like the original Starfox 64.

## Credits

- Original project by [Vittorio Dinovi](https://github.com/vdinovi) (Cal Poly CPE 471 Final Project)
