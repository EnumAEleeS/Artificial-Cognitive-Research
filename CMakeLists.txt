cmake_minimum_required(VERSION 3.18)
project(acr_cognitive_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Sanitizers for development (Section 3 of analysis)
option(ACR_ENABLE_SANITIZERS "Enable ASan/TSan" ON)
if(ACR_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address,thread -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,thread)
endif()

# pybind11
find_package(pybind11 REQUIRED)

pybind11_add_module(cognitive_engine
    bindings.cpp
)

target_include_directories(cognitive_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Install target
install(TARGETS cognitive_engine DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
