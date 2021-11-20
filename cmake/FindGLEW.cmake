set(GLEW_HOME "${VENDOR}/glew")
set(GLEW_INC "${GLEW_HOME}/include")
set(GLEW_SRC "${GLEW_HOME}/src/glew.c")

add_library(GLEW ${GLEW_SRC})
target_include_directories(GLEW PUBLIC ${GLEW_INC})
target_compile_definitions(GLEW PUBLIC GLEW_STATIC)
set_target_properties(GLEW PROPERTIES LINKER_LANGUAGE C)