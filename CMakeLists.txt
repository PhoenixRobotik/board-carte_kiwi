####################################################################
#   Project configuration
####################################################################
cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

project (PhoenixBoard_CarteKiwi)
set     (PhoenixBoard_CarteKiwi_VERSION_MAJOR 0)
set     (PhoenixBoard_CarteKiwi_VERSION_MINOR 1)

include(${GenericCmakeLists})

####################################################################
#   Source
####################################################################
## Déclaration de la librairie aux projets qui utilisent celui-ci
# Déclaration des sources de la librairie
add_library(${PROJECT_NAME}Lib STATIC
    definitions/timers_cpp.cpp
    can.cpp
    clock.cpp
    eeprom.cpp
    leds.cpp
    pwm.cpp
)
# Includes publics
target_include_directories(${PROJECT_NAME}Lib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(${PROJECT_NAME}Lib INTERFACE
    m
)

# Librairie à linker
export(TARGETS ${PROJECT_NAME}Lib FILE Librairie.cmake)

add_executable(${PROJECT_NAME}Tests
    ${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp
)
add_executable(main_binary ALIAS ${PROJECT_NAME}Tests)
target_link_libraries(${PROJECT_NAME}Tests
    ${PROJECT_NAME}Lib
    ${Toolchain_LFlags}
)
