cmake_minimum_required (VERSION 2.8.12)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  message("-- Debug build")
endif()

# set search path for CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

# include OpenOCD module, but make it not required
find_package(Openocd)

# Project specific settings
set(application_name "stm32_cmake")

cmake_policy(SET CMP0048 NEW)
project (${application_name} VERSION 0.1.1 LANGUAGES C CXX ASM)

configure_file(${CMAKE_SOURCE_DIR}/src/app/inc/app/version.h.in
  ${CMAKE_SOURCE_DIR}/src/app/inc/app/version.h)

# platform-specific configurations
include(CMake/platform.cmake)

# Target-specific flags
set(MCU_ARCH ARMCM3)
set(MCU_FAMILY STM32F4xx)
set(MCU_NAME STM32F407xx)
set(__FPU_PRESENT 0)
option(USE_LL_LIB "Enable LL library" ON)
option(USE_HAL_LIB "Enable HAL library" ON)
option(USE_SYSTEM_VIEW "Enable Segger SystemView library" OFF)

message(STATUS "MCU_FAMILY: ${MCU_FAMILY}")
message(STATUS "MCU_NAME: ${MCU_NAME}")
message(STATUS "Specified C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Linker script: ${LINKER_SCRIPT}")
message(STATUS "Use LL library: ${USE_LL_LIB}")
message(STATUS "Use HAL library: ${USE_HAL_LIB}")
message(STATUS "Use Segger SystemView library: ${USE_SYSTEM_VIEW}")

# Used linker file (just for testing)
get_filename_component(LINKER_SCRIPT libraries/linker/stm32f407vgtx.ld ABSOLUTE)

# Use LTO? WARNING: This might break the code
#set(ADDITIONAL_CORE_FLAGS -flto)

# OpenOCD config file
get_filename_component(OPENOCD_CONFIG libraries/debug//stm32f4discovery.cfg ABSOLUTE)

# add libraries
add_subdirectory(libraries)
add_subdirectory(ThirdParty)

# add main project
add_subdirectory(src)
