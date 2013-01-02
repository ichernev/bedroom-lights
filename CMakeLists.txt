set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arduino-cmake/cmake/ArduinoToolchain.cmake)

cmake_minimum_required(VERSION 2.8)
project(BedroomLights C CXX)

print_board_list()
print_programmer_list()

generate_arduino_firmware(bedroom-lights
    SKETCH ${CMAKE_SOURCE_DIR}
    BOARD uno
    PORT /dev/ttyACM0
    PROGRAMMER usbtinyisp
    AFLAGS -V
    NO_AUTOLIBS)
