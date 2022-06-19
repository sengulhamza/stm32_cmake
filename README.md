# How to build

- Install arm toolchain from https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
- Install `cmake` and `make` build tools
- Install `openocd` to upload binaries to the Flash. https://github.com/xpack-dev-tools/openocd-xpack/releases/tag/v0.11.0-1/

```
git clone https://github.com/sengulhamza/stm32_cmake.git
cd stm32_cmake
./build.sh
cd build
make
make flash
```

## Source hierarchy

- `src` is the main application source directory.
- `src/app` for application related code files. ie: main
- `src/drv` for driver code files.
- `src/core` system files related to STM32

---
## Branch workflow
- Before create a new branch follow the below flow
```
git checkout master
git pull
git checkout -b feature/branch-name
```
- If your commit fixing a bug branch name will be like `bugfix/branch-name`
- Before push your commit, write a clear and concise explanation with -m option
---
## Before commit
- Make sure your project builds without any warning
- Make sure the code style is in a correct format. You can run below command to fix
```
cd stm32_cmake
./code-format.sh
```
---
