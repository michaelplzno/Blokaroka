cmake_minimum_required (VERSION 3.12)

message (STATUS "Building Blokaroka .. ")
project(Blokaroka LANGUAGES CXX)
set(executable_name "Blokaroka")

# Set CMP0079 policy to NEW
cmake_policy(SET CMP0079 NEW)

# Add Box2D library
add_subdirectory(extern/box2d)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/extern/box2d/include
    ${PROJECT_SOURCE_DIR}/src
)

# Include source files
include(src/sources.cmake)
# Include source files
set(SOURCES
    src/Blokaroka.cpp
    src/Renderer.cpp
    src/GameState.cpp
    src/Physics.cpp
    src/Blok.cpp
    src/resource.h
    src/resource.rc
    # Add other source files here if needed
)

# Add executable target
add_executable(${executable_name} ${SOURCES})

# Link Box2D library
target_link_libraries(${executable_name} box2d)

# Specify subsystem and entry point for Windows
if (WIN32)
    set_target_properties(${executable_name} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:WinMainCRTStartup"
    )
endif()

# Add release build options
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob2 /DNDEBUG")

# Add a task to build the release build
add_custom_target(build_release
    COMMAND ${CMAKE_COMMAND} --build . --config Release
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building the release version of ${executable_name}"
)

add_subdirectory(src)


