# We will use the C89 Standard because it's the most compatible one.
# Set the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and C standard version
project(banca_de_codigo_uerj C)

# Set C Standard
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 89)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
# Force CMake to use Clang
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

# Add compile options for your project (X args)
add_compile_options(
    -O3 -ggdb -g -pg -march=native -std=c89
    -Wall -Wextra -Werror -Wshadow -Wformat -Wreturn-type -Wuninitialized
    -Wstrict-prototypes -Wno-missing-braces -pedantic
)

set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    set(CMAKE_C_STANDARD 99)
    add_compile_options(-O3 -ggdb -g -pg -march=native -std=c99 -Wno-error -Wno-gcc-compat -w)
    FetchContent_MakeAvailable(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
  endif()
endif()

set(CMAKE_C_STANDARD 89)
add_compile_options(
    -O3 -ggdb -g -pg -march=native -std=c89
    -Wall -Wextra -Werror -Wshadow -Wformat -Wreturn-type -Wuninitialized
    -Wstrict-prototypes -Wno-missing-braces -pedantic
)

# Source files
set(SOURCES src/main.c) 

# Add the executable target
include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(src)

