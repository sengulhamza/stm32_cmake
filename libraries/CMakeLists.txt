add_subdirectory(CMSIS)

if(MCU_FAMILY MATCHES STM32F4xx)
  message("-- STM32F4xx build")
  add_subdirectory(STM32F4xx_HAL_Driver)
elseif(MCU_FAMILY MATCHES STM32F1xx)
    message("-- STM32F1xx build")
    add_subdirectory(STM32F1xx_HAL_Driver)
endif()

