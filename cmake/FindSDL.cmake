# Set the variable -DSDL2_DIR on windows to the SDL devel directory
# More information on https://trenki2.github.io/blog/2017/06/02/using-sdl2-with-cmake/

if(WIN32)
    set(SDL2_DIR "${VENDOR}/windows/SDL2-2.0.16")
endif()

#######################################
# SDL2

find_package(SDL2 REQUIRED)

#######################################
# SDL2_image
# You must unzip the SDL2_image zip in the same directory as ${SDL2_DIR}

link_directories(${SDL2_DIR}/lib/x64)
list(APPEND SDL2_LIBRARIES ${SDL2_DIR}/lib/x64/SDL2_image.lib)
