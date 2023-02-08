# engine2

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

Once XCode and proper MacOS SDK are installed (which are the default with XCode), compilation should work as-is.