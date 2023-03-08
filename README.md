# engine2

## TODO

- Antialiasing
- Refactor draw functions into systems
- Gizmos (like unreal engine)
- no more renderer context
- ability to view different frame_buffers
  - before post process
  - depth buffer of the scene
  - the refacto that goes with it
- Shadows

## Development Environment

As of 2023 February, we use Visual Studio Code with the following extensions :

- `C/C++` from `Microsoft`
  - Used only for debugging.
- `CMake Tools` from `Microsoft`
- `clangd` from `LLVM`
  - Accept to disable Intellisense
  - Run `clangd: download language server` to have a proper clangd
- `Clang-Format`

## Build

We package every dependency as git-submodules. Therefore, after a clone you must run:

`git submodule update --init`

### Windows

Not tested.

### Linux

We need X11 or Wayland development packages installed for libGLFW. To that end:

```bash
sudo apt install xorg-dev # For Debian/Ubuntu on X11
sudo dnf install libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel # For Fedora and derivatives on X11
pkg install xorgproto # For FreeBSD on X11

sudo apt install libwayland-dev libxkbcommon-dev wayland-protocols extra-cmake-modules # For Debian/Ubuntu on Wayland
sudo dnf install wayland-devel libxkbcommon-devel wayland-protocols-devel extra-cmake-modules # For Fedora and derivatives on Wayland
pkg install wayland libxkbcommon wayland-protocols kf5-extra-cmake-modules # For FreeBSD on Wayland
```

### MacOS

Once Xcode and proper MacOS SDK are installed (which are the default with XCode), compilation should work as-is.
