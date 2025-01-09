list(APPEND library_headers
    src/Blok.h
    src/Blokaroka.h
    src/GameState.h
    src/Physics.h
    src/Renderer.h
    src/resource.h)

file(GLOB_RECURSE box2d_headers "extern/box2d/include/box2d/*.h")
list(APPEND library_headers ${box2d_headers})

list(APPEND library_sources
    src/Physics.cpp
    src/GameState.cpp
    src/Blokaroka.cpp
    src/Blok.cpp
    src/Renderer.cpp)

set(SOURCES
    src/Blokaroka.cpp
)
