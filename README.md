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

### Windows (Step-by-Step)

If you've never built a C++ project before, follow every step exactly.

#### 1. Install Visual Studio (the compiler)

1. Go to https://visualstudio.microsoft.com/downloads/
2. Download **Visual Studio Community** (free)
3. Run the installer
4. When it asks what to install, check **"Desktop development with C++"**
5. Click **Install** and wait for it to finish

#### 2. Install CMake

1. Go to https://cmake.org/download/
2. Under "Latest Release", download the **Windows x64 Installer** (the `.msi` file)
3. Run the installer
4. **IMPORTANT:** When it asks, select **"Add CMake to the system PATH for all users"**
5. Finish the installation

#### 3. Install Git (to download the project)

1. Go to https://git-scm.com/download/win
2. Download and run the installer
3. Use all the default options, just keep clicking **Next** and then **Install**

#### 4. Install vcpkg (this downloads the libraries for you)

1. Open **"Developer Command Prompt for VS"** (search for it in the Start Menu — NOT the regular Command Prompt)
2. Type these commands one by one, pressing Enter after each:

```cmd
cd C:\
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg install glfw3:x64-windows glm:x64-windows
```

The last command will take a few minutes. Wait until it says it's done.

#### 5. Download this project

Still in the same Developer Command Prompt:

```cmd
cd C:\
git clone https://github.com/YOUR_USERNAME/DESDE-CERO-SF.git
cd DESDE-CERO-SF
```

(Replace `YOUR_USERNAME` with the actual GitHub URL you were given.)

#### 6. Build the game

Still in the same Developer Command Prompt:

```cmd
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

If everything worked, you'll see no errors at the end.

#### 7. Run the game

```cmd
cd Release
starfox64.exe
```

If you get an error about a missing `.dll` file, copy the GLFW DLL next to the game:

```cmd
copy C:\vcpkg\installed\x64-windows\bin\glfw3.dll .
```

Then try running `starfox64.exe` again.

#### Troubleshooting

- **"cmake is not recognized"** — You didn't add CMake to PATH during install. Uninstall CMake and reinstall it, making sure to select "Add CMake to PATH".
- **"git is not recognized"** — Close and reopen the Developer Command Prompt after installing Git.
- **The game window opens but immediately crashes** — Make sure your graphics drivers are up to date. Go to your GPU manufacturer's website (NVIDIA, AMD, or Intel) and download the latest driver.
- **"Cannot find glfw3"** — Make sure you used the `-DCMAKE_TOOLCHAIN_FILE=...` part in the cmake command. That's what tells it where to find the libraries.

## Build & Run (macOS)

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
