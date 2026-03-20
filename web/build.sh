#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

# Check for Emscripten
if ! command -v emcmake &> /dev/null; then
    if [ -f "$HOME/emsdk/emsdk_env.sh" ]; then
        source "$HOME/emsdk/emsdk_env.sh"
    elif [ -f "$EMSDK/emsdk_env.sh" ]; then
        source "$EMSDK/emsdk_env.sh"
    else
        echo "Error: Emscripten not found."
        echo "Install it: git clone https://github.com/emscripten-core/emsdk.git && cd emsdk && ./emsdk install latest && ./emsdk activate latest"
        echo "Then: source emsdk_env.sh"
        exit 1
    fi
fi

echo "Building Star Fox 64 for WebAssembly..."

mkdir -p build
cd build

emcmake cmake ..
emmake make -j$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)

# Copy output to dist/
mkdir -p ../dist
cp starfox64.html ../dist/index.html
cp starfox64.js ../dist/
cp starfox64.wasm ../dist/
cp starfox64.data ../dist/

echo ""
echo "Build complete! Output in web/dist/"
echo "Test locally:  cd web/dist && python3 -m http.server 8080"
echo "Then open:     http://localhost:8080"
