set(LUA_HOME "${VENDOR}/lua")
set(INC "${LUA_HOME}")
set(SRC "${LUA_HOME}/onelua.c")

# Add lib source files
add_library(Lua STATIC ${SRC})

# Add lib include folder
target_include_directories(Lua PUBLIC ${INC})

# Force C++ compilation for Lua (.c interpreted in C++ and not C)
# set_source_files_properties(${LUA_SRC} PROPERTIES LANGUAGE CXX)

# Do not compile lua interpreter
target_compile_definitions(Lua PRIVATE MAKE_LIB)
