# Current Build v0.2.02 Downloadable Here: [Blokaroka](https://blokaroka.com)
One of my first projects: an attempt to make bricks appear on top of the UI in Windows. This will help you pass the time while your program compiles in the background, or if you are at work and need to keep your brain active during a dull stretch. Playing with blocks improves creativity and dynamic thinking, so having this app running might even make you more effective at work, or help you cut your stress!

If the project catches on, hopefully we can work together to make it a popular app.

![Screenshot on Windows 10](https://raw.githubusercontent.com/michaelplzno/DesktopBricks/master/Blokaroka.PNG)

Hit the ESC key to terminate the program, or right click the icon in the start bar and hit CLOSE, or now there is a System Tray icon that when right clicked creates a menu that allows you to change your blok colors or exit. The app is designed to appear on top of the Windows UI, so it has no window frame or X in the corner to close it. 

When the program is working correctly it will spawn a bunch of little bricks on the desktop you can play with virtually by dragging them and connecting them. Also it will save the layout of the bricks into a gamestate so if you run it again your setup will be reloaded. 

Check the issues section for a list of current known issues, and let me know if you would like to fix them. Feel free to add issues to the list! If you would like to contribute, just let me know!

This project is now officially Open Source under GPL V2.0, It is my hope that we can build this up into something bigger, maybe even an open source engine for games played on top of the operating system UI. 

"Let this be a kiss to the whole world!"

# Overhaul 1/9/2025 v0.2.0 -
1) Build now works with version 3.1.0 of Box2D
2) Add cmake files, you may need to install cmake to build but you can now build with visual studio code.
3) Rename icons that were spelled wrong.
4) add .vscode files for launching and building.
5) add resource.rc which was missing.
6) Fix small issues with turning gravity on and off.
7) Add debug drawing of physics and option to enable it in menu.
8) Fix small bug with how bottom bloks are attached.

# 1/18/2025 v0.2.02 - 
Small fixes to brick attaching.

---

# Compiling Guide 
# -1- Install Required Tools
1. **Install VSCode**  
   Download and install Visual Studio Code from [VSCode](https://code.visualstudio.com/).

2. **Install CMake**  
   - Download and install CMake from [CMake Download](https://cmake.org/download/).  
   - Add CMake to your system's PATH during installation.

3. **Install a C++ Compiler**  
   - Install MSVC (Microsoft Visual C++) by downloading and installing Visual Studio Build Tools from [Visual Studio Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/).  
   - During installation, select the *Desktop development with C++* workload.

4. **Install Git**  
   Download and install Git from [Git Download](https://git-scm.com/).  
   Add Git to your system's PATH during installation.

5. **Install Ninja (Optional)**  
   If you prefer using Ninja as the build system, download it from [Ninja Build](https://ninja-build.org/) and add it to your PATH.

# -2- Set Up VSCode Extensions
Install the following extensions in VSCode:

- **CMake Tools:**  
  Provides CMake integration for VSCode.  
  Install it from the Extensions Marketplace or [here](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools).

- **C/C++:**  
  Provides IntelliSense, debugging, and code formatting for C++.  
  Install it from the Extensions Marketplace or [here](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools).

- **Clang-Format:**  
  Enables code formatting using `.clang-format`.  
  Install it from the Extensions Marketplace or [here](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format).

# -3- Clone the Project
If you haven't already cloned the project, use Git to clone it.

```
git clone https://github.com/michaelplzno/Blokaroka Blokaroka
cd Blokaroka
```

You may need to configure git to work on your machine if this is the first github project you've used, search online for extra instructions.


# -4- Download and Add Box2D to the `extern` Directory
1. **Download Box2D:**  
   Clone the Box2D repository into the `extern` directory.
```
git clone https://github.com/erincatto/box2d.git extern/box2d
```

3. **Ensure Box2D is Included in the Build:**  
   The `CMakeLists.txt` file already includes the following line to add Box2D.
```
add_subdirectory(extern/box2d)
```

# -5- Configure the C++ Formatter (The line width for the project is 80 characters!)
1. **Configure VSCode to Use clang-format:**  
   - Open your VSCode settings (`Ctrl+,` or `Cmd+,` on macOS).  
   - Add the following settings to your `settings.json` file.
```
{
  "editor.formatOnSave": true,
  "C_Cpp.clang_format_style": "file",
  "C_Cpp.clang_format_fallbackStyle": "Microsoft"
}
```
2. **Format Your Code:**  
   To format your code manually, press `Shift+Alt+F` (or `Shift+Option+F` on macOS) while editing a `.cpp` or `.h` file.

# -6- Configure the Project in VSCode
1. **Open the Project in VSCode:**  
   Open the *Blokaroka* folder in VSCode.

2. **Configure CMake Tools:**  
   - Open the Command Palette (`Ctrl+Shift+P` or `Cmd+Shift+P` on macOS).  
   - Search for `CMake: Configure` and select it.  
   - Choose the generator (e.g., *Ninja* or *Visual Studio 17 2022*).  
   - Select the build variant (e.g., *Debug* or *Release*).

3. **Set the Build Directory:**  
   By default, CMake Tools will create a build directory in the project root.

# -7- Build the Project
1. **Build Using CMake Tools:**  
   - Open the Command Palette (`Ctrl+Shift+P` or `Cmd+Shift+P`).  
   - Search for `CMake: Build` and select it.  
   - This will build the project using the selected configuration.

2. **Build from the Terminal:**  
   Alternatively, you can build the project manually from the terminal.
```
cmake --build build --config Debug
```

# -8- Run the Application
1. **Run from VSCode:**  
   - Open the Command Palette (`Ctrl+Shift+P` or `Cmd+Shift+P`).  
   - Search for `CMake: Run Without Debugging` and select it.

2. **Run from the Terminal:**  
   Navigate to the build directory and run the executable.


# -9- Troubleshooting
1. **Missing Dependencies:**  
   Ensure Box2D is correctly cloned into the `box2d` directory.

2. **Common Errors:**  
   - `wcscpy_s` errors: Ensure you are using the correct arguments for `wcscpy_s` (destination buffer, buffer size, source string).  
   - Linker errors: Ensure all source files are listed in the `CMakeLists.txt` file under `set(SOURCES ...)`.


# Summary of Tools and Extensions

| Tool/Extension          | Purpose                            | Download Link                                            |
|--------------------------|------------------------------------|---------------------------------------------------------|
| VSCode                  | IDE for development               | [VSCode](https://code.visualstudio.com/)                |
| CMake                   | Build system generator            | [CMake Download](https://cmake.org/download/)           |
| MSVC                    | Compiler for building the project | [Visual Studio Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/) |
| Git                     | Version control                   | [Git Download](https://git-scm.com/)                    |
| Clang-Format            | Code formatting                   | [Clang-Format Extension](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format) |
| CMake Tools Extension   | CMake integration for VSCode      | [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) |
| C/C++ Extension         | IntelliSense and debugging for C++| [C/C++ Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) |

By following these steps, you should be able to successfully set up, build, and run the *Blokaroka* project in VSCode. Let me know if you encounter any issues!
