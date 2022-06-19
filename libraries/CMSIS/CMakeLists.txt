# Add libraries
add_library(cmsis INTERFACE)
target_include_directories(cmsis INTERFACE Core/Include) 
target_include_directories(cmsis INTERFACE Device/ST/STM32F4xx/Include)
target_compile_definitions(cmsis INTERFACE -D ${MCU_FAMILY})
