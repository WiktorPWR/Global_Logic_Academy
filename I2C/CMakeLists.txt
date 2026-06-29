cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME I2C)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
    Core/Inc/UART_Commands.h)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Plik UART.c dodajemy do źródeł (ze zwykłym slashem '/')
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    "Core/Src/UART.c"
    "Core/Src/UART_Commands.c"
    "Core/Src/PCA9685/Interface.c"

    # Add user sources here
)

# Tutaj wskazujemy FOLDER, w którym znajduje się UART.h
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    "Core/Inc"

    # Add user defined include paths
)

# Folder dla bibliotek (zostaw pusty, jeśli nie linkujesz zewnętrznych plików .a/.lib)
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE

    # Add user defined library search paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE

    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)