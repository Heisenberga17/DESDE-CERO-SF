# Star Fox 64 - Fan Recreation

A fan-made recreation of Star Fox 64 built from scratch with C++ and OpenGL. Fly your Arwing through waves of enemies, dodge obstacles, and shoot everything in sight.

> Based on [vdinovi/Starfox64](https://github.com/vdinovi/Starfox64) (Cal Poly CPE 471 Final Project by [Vittorio Dinovi](https://github.com/vdinovi)), with custom modifications.

---

## What is this game?

This is a 3D space combat game inspired by Star Fox 64 for the Nintendo 64. You pilot an Arwing spacecraft, flying through a level while shooting down enemy ships that come at you in waves. The game features:

- A fully controllable Arwing ship with pitch, yaw, roll, boost, and brake
- Multiple enemy types (Bandits, Grangas, Invaders) that spawn in waves
- A health and lives system
- Laser projectiles for both you and the enemies
- A scrolling environment with sky and water
- PS5 DualSense controller support
- Game Over and restart functionality

---

## Controls

| Action | Keyboard | PS5 Controller |
|---|---|---|
| Fly Up / Down | Arrow Up / Down | Left Stick Up / Down |
| Fly Left / Right | Arrow Left / Right | Left Stick Left / Right |
| Barrel Roll Left | A | Left Bumper (L1) |
| Barrel Roll Right | D | Right Bumper (R1) |
| Boost | W | Right Trigger (R2) |
| Brake | S | Left Trigger (L2) |
| Shoot | Space | X Button (Cross) |
| Pause | P | Options Button |
| Restart (Game Over) | R | Circle Button |

---

## Windows - Complete Setup Guide (No Experience Needed)

This guide assumes you have **never programmed before** and are starting from zero. Follow every step exactly, in order. **Do NOT skip any step.**

If something goes wrong, check the [Troubleshooting](#troubleshooting) section at the bottom.

---

### Before you start

You will need:
- A computer running **Windows 10 or Windows 11**
- An **internet connection** (you'll be downloading about 5-8 GB total)
- About **30-60 minutes** of time (most of it is waiting for downloads)
- About **10 GB of free disk space**

You will be installing 3 programs:
1. **Visual Studio** - this is what compiles (converts) the game's code into a program your computer can run
2. **CMake** - this is a tool that organizes the build process
3. **Git** - this is a tool that downloads the game's source code from the internet

---

### Step 1: Install Visual Studio

> **IMPORTANT:** This is NOT "Visual Studio Code" (VS Code). They are different programs. You need the full **Visual Studio**. It is completely free.

1. Open your web browser (Chrome, Edge, Firefox — any of them)
2. Go to this website: **https://visualstudio.microsoft.com/downloads/**
3. You will see three options: Community, Professional, and Enterprise
4. Under **"Community"** (the free one on the left), click the big **"Free download"** button
5. A file called `VisualStudioSetup.exe` will download to your Downloads folder
6. Go to your Downloads folder and **double-click** `VisualStudioSetup.exe` to run it
7. Windows will ask: *"Do you want to allow this app to make changes to your device?"* — click **Yes**
8. Wait for the installer to load (this takes about 30 seconds to a minute)
9. You will see a screen with boxes called **"Workloads"**. These are packages of tools you can install
10. Find the box that says **"Desktop development with C++"** and **click the checkbox** on it. A checkmark should appear
11. **Do NOT check any other boxes.** Leave everything else as-is
12. Click the big **"Install"** button at the bottom right corner
13. The installation will begin. **This takes 10-20 minutes** depending on your internet speed. You will see a progress bar. You can use your computer while it installs
14. When it finishes, it may ask you to **restart your computer**. If it does, save any open work and click **Restart**
15. After restarting, you don't need to open Visual Studio. The tools it installed are what we need

---

### Step 2: Install CMake

> CMake is a small program that tells Visual Studio how to build the game. Without it, Visual Studio wouldn't know what to do with the game's code.

1. Open your web browser
2. Go to: **https://cmake.org/download/**
3. Scroll down until you see a section labeled **"Latest Release"** with a table of download links
4. In that table, look for a file that ends in **`windows-x86_64.msi`**. It will look something like: `cmake-3.XX.X-windows-x86_64.msi`
5. Click it to download it
6. Go to your Downloads folder and **double-click** the `.msi` file to run it
7. An installer window will open. Click **Next**
8. Read (or skip) the license agreement. Check **"I accept the terms"** and click **Next**
9. **THIS IS THE MOST IMPORTANT STEP IN THE ENTIRE GUIDE:**
   You will see a screen that asks about the PATH. You will see radio buttons (circles you can click). Select:
   - **"Add CMake to the system PATH for all users"**
   - If you skip this or choose the wrong option, **none of the later steps will work**
10. Click **Next**
11. Leave the install location as-is (default). Click **Next**
12. Click **Install**. If Windows asks for permission, click **Yes**
13. Click **Finish**

---

### Step 3: Install Git

> Git is a tool that downloads source code from GitHub (a website where programmers share code). We need it to download the game.

1. Open your web browser
2. Go to: **https://git-scm.com/download/win**
3. The download should start automatically within a few seconds. If it doesn't, click the link that says **"Click here to download manually"**
4. Go to your Downloads folder and **double-click** the downloaded file (it will be called something like `Git-2.XX.X-64-bit.exe`)
5. Windows will ask for permission — click **Yes**
6. An installer with many screens will appear. **Do NOT change any options.** Just keep clicking **Next** on every single screen
7. On the final screen, click **Install**
8. When it finishes, click **Finish**

---

### Step 4: Open the Developer Command Prompt

> This is a special command prompt that comes with Visual Studio. It knows where all the compilation tools are. **You MUST use this one** — the regular Command Prompt or PowerShell will NOT work.

1. Click the **Start Menu** button (the Windows icon at the bottom-left corner of your screen, or press the Windows key on your keyboard)
2. Start typing: **Developer Command Prompt**
3. You should see a result called **"Developer Command Prompt for VS 2022"** (the year might be different)
4. **Right-click** on it
5. Click **"Run as administrator"**
6. Windows will ask *"Do you want to allow this app to make changes to your device?"* — click **Yes**
7. A window will open with a black background and white text. This is called the **command prompt** (or **terminal**)
8. You should see something like:
   ```
   **********************************************************************
   ** Visual Studio 2022 Developer Command Prompt
   **********************************************************************
   C:\Program Files\Microsoft Visual Studio\2022\Community>
   ```
9. This is where you will type all the commands in the next steps

> **IMPORTANT: Keep this window open for ALL remaining steps. Do NOT close it. If you accidentally close it, go back to this step and open it again.**

---

### Step 5: Install the game's libraries (GLFW and GLM)

> The game uses two libraries (pre-built pieces of code) called GLFW and GLM. GLFW handles the window and keyboard/controller input. GLM handles the 3D math. We'll use a tool called **vcpkg** to install them automatically.

Type each of the following commands **one at a time**. After typing each line, press **Enter** and **wait for it to finish** before typing the next one. "Finished" means you see the blinking cursor on a new line and no more text is appearing.

**Command 1** — Navigate to the C: drive:
```cmd
cd C:\
```

**Command 2** — Download vcpkg (the library manager):
```cmd
git clone https://github.com/microsoft/vcpkg.git
```
You'll see some text about "Cloning into 'vcpkg'..." — wait for it to finish.

**Command 3** — Enter the vcpkg folder:
```cmd
cd vcpkg
```

**Command 4** — Set up vcpkg:
```cmd
bootstrap-vcpkg.bat
```
You'll see a bunch of text scroll by. Wait until it finishes and you see the cursor blinking again.

**Command 5** — Install the libraries:
```cmd
vcpkg install glfw3:x64-windows glm:x64-windows
```

> **This command takes 3-10 minutes.** You will see a LOT of text scrolling by — this is completely normal. It's downloading and compiling code. Just wait. When it's done, you'll see the blinking cursor again with something like "elapsed time" at the end.

---

### Step 6: Download the game

Type each command one at a time:

**Command 1** — Go back to C: drive:
```cmd
cd C:\
```

**Command 2** — Download the game's source code:
```cmd
git clone https://github.com/Heisenberga17/DESDE-CERO-SF.git
```
Wait for it to finish (should be just a few seconds).

**Command 3** — Enter the game's folder:
```cmd
cd DESDE-CERO-SF
```

---

### Step 7: Build the game

**Command 1** — Create a folder for the build files:
```cmd
mkdir build
```

**Command 2** — Enter the build folder:
```cmd
cd build
```

**Command 3** — Configure the build (tell CMake where everything is):
```cmd
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

> Wait for this to finish. You should see lines that say `-- Configuring done` and `-- Generating done` near the end. If you see red text with the word "error", check the [Troubleshooting](#troubleshooting) section.

**Command 4** — Compile the game (turn the code into an actual program):
```cmd
cmake --build . --config Release
```

> **This takes 1-3 minutes.** You'll see files being compiled one by one. When it's done, the very last line should say something like:
> ```
> Build succeeded.
>     0 Warning(s)
>     0 Error(s)
> ```

If it says `0 Error(s)` — congratulations, the game is built!

---

### Step 8: Run the game!

**Command 1** — Copy a required file into the game's folder:
```cmd
copy C:\vcpkg\installed\x64-windows\bin\glfw3.dll Release\
```

**Command 2** — Go into the Release folder:
```cmd
cd Release
```

**Command 3** — Launch the game:
```cmd
starfox64.exe
```

> A window should open with the game running! You can play with the keyboard (see the [Controls](#controls) table above) or plug in a PS5 DualSense controller via USB.

---

### Running the game again later

You don't need to repeat all the installation steps. After the first time, just do this:

1. Click the **Start Menu**
2. Type **Developer Command Prompt** and open **"Developer Command Prompt for VS 2022"**
3. Type these two commands:

```cmd
cd C:\DESDE-CERO-SF\build\Release
```
```cmd
starfox64.exe
```

That's it — the game will launch.

---

### Creating a desktop shortcut (so you can double-click to play)

If you don't want to open the command prompt every time:

1. Open **File Explorer** (the folder icon on your taskbar)
2. Navigate to `C:\DESDE-CERO-SF\build\Release`
3. Find the file called **`starfox64.exe`**
4. **Right-click** on it
5. Click **"Show more options"** (on Windows 11) if needed, then click **"Create shortcut"**
6. Windows may say *"Do you want to place the shortcut on the desktop?"* — click **Yes**
7. Now you have a shortcut on your desktop. Double-click it anytime to play!

> **Note:** If the game crashes when launched from the shortcut, make sure that `glfw3.dll` is in the same folder as `starfox64.exe` (you already did this in Step 8, Command 1).

---

### Uninstalling everything

If you want to remove everything you installed:

1. **Delete the game:** Open File Explorer, go to `C:\`, and delete the `DESDE-CERO-SF` folder
2. **Delete vcpkg:** In the same `C:\` location, delete the `vcpkg` folder
3. **Uninstall Visual Studio:** Go to Settings > Apps > Installed Apps, find "Visual Studio Community 2022", click the three dots, and click Uninstall
4. **Uninstall CMake:** Same place, find "CMake", and uninstall it
5. **Uninstall Git:** Same place, find "Git", and uninstall it

---

### Troubleshooting

#### "'cmake' is not recognized as an internal or external command"

You didn't select "Add CMake to PATH" during CMake installation. Fix:
1. Go to **Settings > Apps > Installed Apps**
2. Find **CMake** and click **Uninstall**
3. Go back to [Step 2](#step-2-install-cmake) and install it again
4. **Make sure** you select **"Add CMake to the system PATH for all users"** this time
5. **Close and reopen** the Developer Command Prompt before trying again

#### "'git' is not recognized as an internal or external command"

Git was just installed but the command prompt doesn't know about it yet. Fix:
- **Close** the Developer Command Prompt window
- **Open it again** from the Start Menu (go back to [Step 4](#step-4-open-the-developer-command-prompt))
- Try the git command again

#### "Cannot find glfw3" or "Could not find a package configuration file provided by glfw3"

CMake can't find the libraries. This usually means the toolchain file path is wrong. Fix:
1. Make sure you typed the cmake command **exactly** as shown in Step 7, Command 3
2. The important part is: `-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake`
3. Make sure vcpkg is actually at `C:\vcpkg`. Open File Explorer and check that the folder exists
4. If you installed vcpkg somewhere else, adjust the path accordingly

#### "The application was unable to start correctly (0xc000007b)"

You built the wrong version (32-bit instead of 64-bit). Fix:
1. Go back to [Step 5](#step-5-install-the-games-libraries-glfw-and-glm)
2. Make sure you typed `glfw3:x64-windows` (not `glfw3:x86-windows`)
3. Then redo [Step 7](#step-7-build-the-game) and [Step 8](#step-8-run-the-game)

#### "Entry point not found" or "glfw3.dll was not found" or other missing DLL errors

You forgot to copy the DLL file. Fix:
1. Run this command:
   ```cmd
   copy C:\vcpkg\installed\x64-windows\bin\glfw3.dll C:\DESDE-CERO-SF\build\Release\
   ```
2. Try running `starfox64.exe` again

#### The game window opens but shows a black screen or crashes immediately

Your graphics drivers are probably outdated. Your computer's GPU (graphics card) needs up-to-date drivers to run OpenGL. Fix:
1. Figure out what GPU you have: press **Windows key + R**, type `dxdiag`, press Enter, and click the **Display** tab. The "Name" field shows your GPU
2. Download the latest drivers from your GPU manufacturer:
   - **NVIDIA:** https://www.nvidia.com/Download/index.aspx
   - **AMD:** https://www.amd.com/en/support
   - **Intel:** https://www.intel.com/content/www/us/en/download-center
3. Install the driver, **restart your computer**, and try the game again

#### The build fails with many errors about "stb_image" or "tiny_obj_loader"

You're using the wrong command prompt. Fix:
- **Close** whatever command prompt you have open
- Go back to [Step 4](#step-4-open-the-developer-command-prompt) and make sure you open the **Developer Command Prompt for VS 2022**, NOT the regular Command Prompt or PowerShell

#### "Access is denied" errors

You didn't run the command prompt as administrator. Fix:
- Close the command prompt
- Go back to [Step 4](#step-4-open-the-developer-command-prompt)
- Make sure you **right-click** and choose **"Run as administrator"**

#### "fatal: destination path 'DESDE-CERO-SF' already exists"

You already downloaded the game before. This is fine! Just skip the `git clone` command and continue with:
```cmd
cd C:\DESDE-CERO-SF
```

#### Something else went wrong and I'm stuck

1. **Read the error message carefully.** It usually tells you what's wrong
2. **Copy the error message** and search for it on Google — someone has probably had the same problem
3. **Start fresh:** Delete the `C:\DESDE-CERO-SF` and `C:\vcpkg` folders, then redo everything from [Step 5](#step-5-install-the-games-libraries-glfw-and-glm)
4. Open an issue on the GitHub repository: https://github.com/Heisenberga17/DESDE-CERO-SF/issues

---

## macOS - Setup Guide

### Install dependencies

```bash
brew install cmake glfw glm
```

If you don't have Homebrew, install it first from https://brew.sh

### Build and run

```bash
git clone https://github.com/Heisenberga17/DESDE-CERO-SF.git
cd DESDE-CERO-SF
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)
./starfox64
```

### Running again later

```bash
cd DESDE-CERO-SF/build
./starfox64
```

---

## Linux - Setup Guide

### Install dependencies

**Ubuntu/Debian:**
```bash
sudo apt install cmake git build-essential libglfw3-dev libglm-dev
```

**Fedora:**
```bash
sudo dnf install cmake git gcc-c++ glfw-devel glm-devel
```

**Arch Linux:**
```bash
sudo pacman -S cmake git glfw glm
```

### Build and run

```bash
git clone https://github.com/Heisenberga17/DESDE-CERO-SF.git
cd DESDE-CERO-SF
mkdir build && cd build
cmake ..
make -j$(nproc)
./starfox64
```

---

## Project Structure

```
DESDE-CERO-SF/
├── src/                    # Game source code (C++)
│   ├── main.cpp            # Main game loop, input, rendering
│   ├── Arwing.cpp/h        # Player ship logic
│   ├── Enemy.cpp/h         # Enemy AI and spawning
│   ├── Environment.cpp/h   # Sky, water, and terrain
│   ├── Interface.cpp/h     # HUD and crosshair
│   ├── Shape.cpp/h         # 3D model loading
│   ├── Program.cpp/h       # Shader program management
│   ├── GLSL.cpp/h          # OpenGL shader utilities
│   ├── MatrixStack.cpp/h   # Matrix transformation stack
│   └── WindowManager.cpp/h # Window and input management
├── shaders/                # GLSL vertex and fragment shaders
├── resources/              # 3D models (.obj) and textures
├── ext/                    # External dependencies (GLAD)
└── CMakeLists.txt          # Build configuration
```

## Credits

- Original project by [Vittorio Dinovi](https://github.com/vdinovi) (Cal Poly CPE 471 Final Project)
- Modified by [Heisenberga17](https://github.com/Heisenberga17)
