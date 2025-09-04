# Modern CMake build system for Snake Game RL
cmake_minimum_required(VERSION 3.15)

project(SnakeGameRL 
    VERSION 1.0.0
    DESCRIPTION "A refactored Snake Game with RL integration"
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler flags for better debugging and warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -O2)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /O2)
endif()

# Find required packages
find_package(OpenGL REQUIRED)

# Set up include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/packages/freeglut.3.0.0.v140.1.0.2/sources)

# Define source files
set(CORE_SOURCES
    src/game_controller.cpp
    src/snake.cpp
    src/apple.cpp
)

set(RL_SOURCES
    src/rl/rl_interface.cpp
)

set(GRAPHICS_SOURCES
    # Graphics implementation would go here
    # For now, we'll include a placeholder
)

# Create core library
add_library(SnakeGameCore STATIC ${CORE_SOURCES})
target_include_directories(SnakeGameCore PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Create RL library
add_library(SnakeGameRL STATIC ${RL_SOURCES})
target_include_directories(SnakeGameRL PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(SnakeGameRL SnakeGameCore)

# Create main executable
add_executable(SnakeGame src/main_refactored.cpp)
target_link_libraries(SnakeGame 
    SnakeGameCore 
    SnakeGameRL 
    ${OPENGL_LIBRARIES}
)

# Platform-specific linking
if(WIN32)
    target_link_libraries(SnakeGame 
        ${CMAKE_SOURCE_DIR}/packages/freeglut.3.0.0.v140.1.0.2/runtimes/win-x64/native/Release/freeglut.lib
        gdi32
    )
elseif(APPLE)
    find_package(GLUT REQUIRED)
    target_link_libraries(SnakeGame ${GLUT_LIBRARIES})
else()
    find_package(GLUT REQUIRED)
    target_link_libraries(SnakeGame ${GLUT_LIBRARIES})
endif()

# Enable testing
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    # TODO: Add unit tests
endif()

# Installation
install(TARGETS SnakeGame 
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ 
    DESTINATION include/SnakeGameRL
    FILES_MATCHING PATTERN "*.h"
)

# Documentation
option(BUILD_DOCS "Build documentation" OFF)
if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        configure_file(${CMAKE_SOURCE_DIR}/docs/Doxyfile.in 
                      ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
        add_custom_target(docs
            ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
        )
    endif()
endif()
