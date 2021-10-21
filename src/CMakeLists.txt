include_guard(GLOBAL)

include(sources.cmake)
add_executable(${executable_name} ${library_headers} ${library_sources})

if(MSVC)
    set(OPTIMIZATION_FLAGS /fp:fast)
else()
    set(OPTIMIZATION_FLAGS --fast-math)
endif()

target_compile_options(${executable_name} PRIVATE $<$<CONFIG:Release>:${OPTIMIZATION_FLAGS}>)
target_include_directories(
        ${executable_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${executable_name})

target_link_libraries(${executable_name} box2d)

