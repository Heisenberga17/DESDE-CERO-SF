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

### Windows (Step-by-Step Guide for Beginners)

Follow every step exactly, in order. Do NOT skip any step.

---

#### Step 1: Install Visual Studio (the compiler that builds the game)

This is NOT "Visual Studio Code" — it's the full Visual Studio. It's free.

1. Open your browser and go to: https://visualstudio.microsoft.com/downloads/
2. Under **"Community"** (the free one), click the **"Free download"** button
3. A file called `VisualStudioSetup.exe` will download. Double-click it to run it
4. If Windows asks "Do you want to allow this app to make changes?", click **Yes**
5. The installer will load. Wait for it
6. You'll see a screen with checkboxes called **"Workloads"**. Check the box that says:
   - **"Desktop development with C++"**
7. Don't change anything else. Click the **"Install"** button at the bottom right
8. Wait for it to finish (this can take 10-20 minutes depending on your internet)
9. When it's done, it may ask you to restart your computer. If it does, **restart**

---

#### Step 2: Install CMake (the build system)

1. Open your browser and go to: https://cmake.org/download/
2. Scroll down to the section that says **"Latest Release"**
3. Look for the file that says **`cmake-X.XX.X-windows-x86_64.msi`** (the `.msi` one) and click it to download
4. Double-click the downloaded `.msi` file to run it
5. Click **Next**
6. Accept the license, click **Next**
7. **THIS IS THE MOST IMPORTANT STEP:** You'll see a screen with radio buttons. Select:
   - **"Add CMake to the system PATH for all users"**
   - (If you don't do this, nothing will work later)
8. Click **Next**, then **Install**, then **Finish**

---

#### Step 3: Install Git (to download the game code)

1. Open your browser and go to: https://git-scm.com/download/win
2. The download should start automatically. If not, click **"Click here to download manually"**
3. Double-click the downloaded file to run it
4. Just keep clicking **Next** on every screen — don't change any options
5. Click **Install** at the end, then **Finish**

---

#### Step 4: Open the Developer Command Prompt

This is a special command prompt that knows where Visual Studio is. You MUST use this one, NOT the regular Command Prompt or PowerShell.

1. Click the **Start Menu** (Windows icon at the bottom left)
2. Type: **Developer Command Prompt**
3. You should see **"Developer Command Prompt for VS 2022"** (or similar) appear
4. **Right-click it** and choose **"Run as administrator"**
5. If Windows asks "Do you want to allow this?", click **Yes**
6. A black window with white text will open. This is where you'll type all the commands below

**Keep this window open for all remaining steps. Do NOT close it.**

---

#### Step 5: Install the game's libraries (GLFW and GLM)

Type these commands one at a time. Press **Enter** after each line. Wait for each command to finish before typing the next one.

```cmd
cd C:\
```
```cmd
git clone https://github.com/microsoft/vcpkg.git
```
```cmd
cd vcpkg
```
```cmd
bootstrap-vcpkg.bat
```
```cmd
vcpkg install glfw3:x64-windows glm:x64-windows
```

The last command downloads and compiles the libraries. **This will take 3-10 minutes.** You'll see a lot of text scrolling by — that's normal. Wait until you see the blinking cursor again with no more text appearing.

---

#### Step 6: Download the game

Type these commands one at a time:

```cmd
cd C:\
```
```cmd
git clone https://github.com/Heisenberga17/DESDE-CERO-SF.git
```
```cmd
cd DESDE-CERO-SF
```

---

#### Step 7: Build the game

Type these commands one at a time:

```cmd
mkdir build
```
```cmd
cd build
```
```cmd
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

Wait for it to finish. You should see lines like `-- Configuring done` and `-- Generating done`. If you see errors instead, check the Troubleshooting section below.

Now compile:

```cmd
cmake --build . --config Release
```

**This will take 1-3 minutes.** Wait until you see the cursor blinking again. The last line should say something like `Build succeeded` or `0 Error(s)`.

---

#### Step 8: Run the game

```cmd
copy C:\vcpkg\installed\x64-windows\bin\glfw3.dll Release\
```
```cmd
cd Release
```
```cmd
starfox64.exe
```

The game window should open. You can play with the keyboard (arrow keys to fly, Space to shoot) or connect a PS5 controller via USB.

---

#### If you want to run the game again later

You don't need to repeat all the steps. Just:

1. Open **Developer Command Prompt for VS 2022** from the Start Menu
2. Type:

```cmd
cd C:\DESDE-CERO-SF\build\Release
```
```cmd
starfox64.exe
```

---

#### Troubleshooting

**"'cmake' is not recognized as an internal or external command"**
> You didn't select "Add CMake to PATH" during installation. Uninstall CMake from Settings > Apps, then install it again from Step 2 and make sure you select that option.

**"'git' is not recognized as an internal or external command"**
> Close the Developer Command Prompt and open it again. Git needs a fresh window after installation.

**"Cannot find glfw3" or "Could not find a package configuration file provided by glfw3"**
> Make sure you typed the cmake command exactly as shown, including the `-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake` part. That tells cmake where to find the libraries.

**"The application was unable to start correctly (0xc000007b)"**
> You built the wrong architecture. Go back to Step 7 and make sure you used `glfw3:x64-windows` (not `glfw3:x86-windows`) in Step 5.

**The game window opens but it's a black screen or crashes immediately**
> Your graphics drivers are probably outdated. Go to your GPU manufacturer's website and update them:
> - NVIDIA: https://www.nvidia.com/Download/index.aspx
> - AMD: https://www.amd.com/en/support
> - Intel: https://www.intel.com/content/www/us/en/download-center

**"Entry point not found" or missing DLL errors**
> Make sure you ran the `copy` command in Step 8 to copy `glfw3.dll` into the Release folder.

**The build fails with many errors about "stb_image" or "tiny_obj_loader"**
> Make sure you're using the Developer Command Prompt, not the regular one. Close whatever you have open and go back to Step 4.

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
