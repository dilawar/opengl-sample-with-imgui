cmake_minimum_required(VERSION 3.12)

project(ImGuiOpenGL VERSION 0.1.0)

#
# Setup conan.
#
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR})

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
        "${CMAKE_BINARY_DIR}/conan.cmake"
        EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
        TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES imgui/1.85 glfw/3.3.5 glew/2.1.0
    GENERATORS cmake_find_package)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
    BUILD missing
    REMOTE conancenter
    SETTINGS ${settings})


find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(example_opengl
    main.cpp
    ${imgui_RES_DIRS}/bindings/imgui_impl_opengl3.cpp
    ${imgui_RES_DIRS}/bindings/imgui_impl_glfw.cpp)

target_include_directories(example_opengl PUBLIC ${imgui_RES_DIRS}/bindings)

target_link_libraries(example_opengl PUBLIC imgui::imgui glfw::glfw GLEW::GLEW)
