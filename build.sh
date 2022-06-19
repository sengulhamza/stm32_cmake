if [ -d "build" ]; then
    RED='\033[0;31m'
    NC='\033[0m' # No Color
    echo "--  ${RED}build dir already exist! ${NC}"
    exit 0 #or you can delete build dir. 'rm -r build'
fi
mkdir build
cd build && cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -D "CMAKE_TOOLCHAIN_FILE=../CMake/GNU-ARM-Toolchain.cmake" ../