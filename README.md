# engine2

![Screenshot of the editor](assets/screenshot.png)

## TODO

- Generize Lights and Shadows
- Ability to better sort deferred renderers execution
- auto unbind through RAII
- Texture rework into the material struct
- Move shaders to the postprocess shader? (outline shader, and more)
- Antialiasing
- Gizmos (like unreal engine)

## Development Environment

As of 2023 March, we use the following tools.

Either `Visual Studio Code`:

- `C/C++` from `Microsoft`
  - Used only for debugging.
- `CMake Tools` from `Microsoft`
- `clangd` from `LLVM`
  - Accept to disable Intellisense
  - Run `clangd: download language server` to have a proper clangd
- `Clang-Format`

Or you can use `CLion`. Everything works out of the box with CLion.

## Build

We package every dependency as git-submodules. Therefore, after a clone you must run:

`git submodule update --init`

### Windows

Does not compile yet.

### Linux

We need X11 or Wayland development packages installed for libGLFW. To that end:

```bash
# If you are compiling for X11
sudo apt install xorg-dev # Debian/Ubuntu and derivatives
sudo dnf install libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel # Fedora and derivatives
pkg install xorgproto # FreeBSD

# If you are compiling for Wayland
sudo apt install libwayland-dev libxkbcommon-dev wayland-protocols extra-cmake-modules # Debian/Ubuntu and derivatives
sudo dnf install wayland-devel libxkbcommon-devel wayland-protocols-devel extra-cmake-modules # Fedora and derivatives
pkg install wayland libxkbcommon wayland-protocols kf5-extra-cmake-modules # FreeBSD
```

### MacOS

Once Xcode and Xcode developer tools are installed, compilation should work as-is.
