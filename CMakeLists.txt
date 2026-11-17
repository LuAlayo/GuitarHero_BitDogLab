# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

# Caminho direto para o SDK instalado no seu sistema
set(PICO_SDK_PATH "/home/usuario/pico-sdk")

# Placa definida para Raspberry Pi Pico W / H (RP2040) da BitDogLab V7
# (Mantenha pico2_w apenas se o módulo soldado na sua placa for realmente um RP2350)
set(PICO_BOARD pico2_w CACHE STRING "Board type" FORCE)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(GuitarDog C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(GuitarDog GuitarDog.c)

pico_set_program_name(GuitarDog "GuitarDog")
pico_set_program_version(GuitarDog "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(GuitarDog 0)
pico_enable_stdio_usb(GuitarDog 1)

# Add the standard library to the build
target_link_libraries(GuitarDog
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(GuitarDog PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(GuitarDog
    pico_stdlib
    hardware_i2c
    hardware_irq
    hardware_pwm
    hardware_clocks
    hardware_flash
    hardware_sync
    hardware_adc
)

pico_add_extra_outputs(GuitarDog)