cmake_minimum_required(VERSION 3.18)    # Requires cmake version 3.18 or higher!

# (More specific is more better apperently)
set(CXX_STANDARD 11)    # Tells cmake that c++ standard 11 is used 
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
project(Hello LANGUAGES CXX C)  # Sets the name of the project and specifies wich languages are used 

add_executable(main exercises/shader-class-test.cpp libraries/glad-4.6/src/glad.c) # Add entry point
add_subdirectory(libraries/glfw-3.4)

target_sources(main PUBLIC ${sources})
target_link_libraries(main glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(main OpenGL::GL)

include_directories(SYSTEM libraries/glad-4.6/include)
include_directories(SYSTEM shaders*)