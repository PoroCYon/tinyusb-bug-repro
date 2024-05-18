set(PROJECT "vkart-aaa")

set(FAMILY "rp2040" CACHE STRING "Board/MCU family, decides which drivers to use. Set to RP2040 by default.")
set(BOARD "raspberry_pi_pico" CACHE STRING "Board used, determines the pinout. Defaults to the Raspberry Pi Pico.")

# use version from repo instead of default SDK version
set(PICO_TINYUSB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../tinyusb/")

cmake_minimum_required(VERSION 3.11)


# TOP is absolute path to root directory of TinyUSB git repo

# Check for -DFAMILY=
option(PICO_NO_FLASH "Disable writing the compiled program to flash, and only load it to RAM. Useful for testing, but not much else (OFF by default)." ON)
option(PICO_COPY_TO_RAM "Run all code in RAM, while the program is also stored on flash. On bootup, everything will be copied to RAM (OFF by default)." OFF)

cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

project(${PROJECT})
pico_sdk_init()

add_executable(${PROJECT})

pico_enable_stdio_uart(${PROJECT} 0)
pico_enable_stdio_usb(${PROJECT} 0)
# Example source
target_sources(${PROJECT} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/../common-src/dfu.c
	${CMAKE_CURRENT_SOURCE_DIR}/../common-src/main.c
	${CMAKE_CURRENT_SOURCE_DIR}/../common-src/usb_descriptors.c

	${CMAKE_CURRENT_SOURCE_DIR}/src/debug.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/hardfault.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/stdio_uart_dma.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tusb_app.c
)

# Example include
target_include_directories(${PROJECT} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc/
	${CMAKE_CURRENT_SOURCE_DIR}/../common-inc/
)

# Example defines
target_compile_definitions(${PROJECT} PUBLIC
)

target_link_libraries(${PROJECT}
	pico_stdlib pico_unique_id hardware_dma hardware_uart hardware_timer
	hardware_pwm cmsis_core pico_time pico_multicore tinyusb_device_unmarked
	pico_usb_reset_interface_headers pico_unique_id
)

pico_add_extra_outputs(${PROJECT})

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=kernel-address -DMcuASAN_CONFIG_IS_ENABLED=1")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror=implicit-function-declaration -Werror=return-type -Werror=maybe-uninitialized")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--cref")

