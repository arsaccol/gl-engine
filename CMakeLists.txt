cmake_minimum_required(VERSION 3.16)

set(PROJ_NAME "gl-engine")
set(EXECUTABLE "game")
message("Project name: ${PROJ_NAME}")
message("vcpkg root: ${VCPKG_ROOT}")

project(${PROJ_NAME} CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

find_path(STB_INCLUDE_DIRS "stb_image.h")
find_package(assimp CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

message("buga buga buga")


#add_executable(${EXECUTABLE} test.cpp stb_include.cpp)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${EXECUTABLE} ${sources})

target_include_directories(${EXECUTABLE} PUBLIC ${STB_INCLUDE_DIRS})
target_link_libraries(${EXECUTABLE} PRIVATE assimp::assimp)
target_link_libraries(${EXECUTABLE} PRIVATE EnTT::EnTT)
target_link_libraries(${EXECUTABLE} PRIVATE glad::glad)
target_link_libraries(${EXECUTABLE} PRIVATE glfw)
target_link_libraries(${EXECUTABLE} PRIVATE glm::glm)
target_link_libraries(${EXECUTABLE} PRIVATE imgui::imgui)
