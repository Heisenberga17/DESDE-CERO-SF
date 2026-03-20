cmake_minimum_required(VERSION 3.5)
project(starfox64)

# Gather all source files from src/ except main.cpp (replaced by main_web.cpp)
file(GLOB SRC_SOURCES "../src/*.cpp")
list(FILTER SRC_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Our web entry point
set(SOURCES ${SRC_SOURCES} main_web.cpp)

# Include directories: web/include first so our GLAD shim takes priority
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

# GLM is header-only — find it from common system locations.
# NO_CMAKE_FIND_ROOT_PATH allows finding host headers during cross-compilation.
find_path(GLM_INCLUDE_DIR glm/glm.hpp
    PATHS /opt/homebrew/include /usr/local/include /usr/include
    NO_CMAKE_FIND_ROOT_PATH)
if(GLM_INCLUDE_DIR)
    include_directories(SYSTEM ${GLM_INCLUDE_DIR})
else()
    message(WARNING "GLM not found. Install GLM or set GLM_INCLUDE_DIR.")
endif()

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# Emscripten link flags
set(EM_FLAGS
    "-sUSE_GLFW=3"
    "-sFULL_ES3=1"
    "-sMIN_WEBGL_VERSION=2"
    "-sMAX_WEBGL_VERSION=2"
    "-sALLOW_MEMORY_GROWTH=1"
    "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/shaders@/shaders"
    "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/../resources@/resources"
    "--exclude-file Nefertiti-100K.obj"
    "--exclude-file bunny.obj"
    "--exclude-file dog.obj"
    "--exclude-file dummy.obj"
    "--exclude-file cube.obj"
    "--exclude-file sphere.obj"
    "--shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html"
)

string(JOIN " " EM_FLAGS_STR ${EM_FLAGS})

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    SUFFIX ".html"
    LINK_FLAGS "${EM_FLAGS_STR}"
)
