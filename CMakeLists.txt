cmake_minimum_required(VERSION 3.13)

if((NOT DEFINED ENV{PICO_SDK_PATH}) AND (NOT PICO_SDK_PATH))
  set(PICO_SDK_FETCH_FROM_GIT_TAG "2.2.0")
  set(PICO_SDK_FETCH_FROM_GIT ON)
endif()

if((NOT DEFINED ENV{PICO_BOARD}) AND (NOT PICO_BOARD))
  set(PICO_BOARD "waveshare_rp2040_zero")
endif()

include(pico_sdk_import.cmake)

project(DonCon2040 VERSION 0.39.0)

# On RP2040 this runs clk_sys at 200MHz against a 133MHz datasheet maximum, and the
# SDK only raises the core voltage to match on RP2350 (SYS_CLK_VREG_VOLTAGE_MIN is not
# defined for RP2040). Overridable so a build can be pinned to the in-spec 125MHz with
# -DPICO_USE_FASTEST_SUPPORTED_CLOCK=0 when chasing intermittent faults.
if(NOT DEFINED PICO_USE_FASTEST_SUPPORTED_CLOCK)
  set(PICO_USE_FASTEST_SUPPORTED_CLOCK 1)
endif()

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Werror)

add_subdirectory(libs)

file(
  GLOB
  ${PROJECT_NAME}_SOURCES
  src/*.cpp
  src/usb/*.c
  src/usb/device/*.c
  src/usb/device/hid/*.c
  src/usb/device/vendor/*.c
  src/utils/*.cpp
  src/peripherals/*.cpp)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_include_directories(${PROJECT_NAME}
                           PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC tinyusb_device
         tinyusb_board
         pico_stdlib
         hardware_adc
         hardware_i2c
         hardware_spi
         pico_multicore
         pico_rand
         pico_mbedtls
         pio_ws2812
         pico_ssd1306
         mcp23017
         mcp3204)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})
