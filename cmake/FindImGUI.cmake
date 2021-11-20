set(IMGUI_HOME "${VENDOR}/imgui")
set(IMGUI_SRC
    ${IMGUI_HOME}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_HOME}/backends/imgui_impl_sdl.cpp
    ${IMGUI_HOME}/imgui.cpp
    ${IMGUI_HOME}/imgui_demo.cpp
    ${IMGUI_HOME}/imgui_draw.cpp
    ${IMGUI_HOME}/imgui_tables.cpp
    ${IMGUI_HOME}/imgui_widgets.cpp)

add_library(ImGUI STATIC ${IMGUI_SRC})
target_include_directories(ImGUI PUBLIC "${VENDOR}/imgui/")
target_include_directories(ImGUI PUBLIC "${VENDOR}/imgui/backends")