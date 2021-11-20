set(FMOD_HOME "${VENDOR}/fmod")

if(WIN32)
  set(FMOD_LIBS "${FMOD_HOME}/lib/x64")
  add_library(FMOD INTERFACE)
  target_link_directories(FMOD INTERFACE ${FMOD_LIBS})
  target_link_libraries(FMOD INTERFACE fmodL_vc)
endif()

target_include_directories(FMOD INTERFACE "${FMOD_HOME}/inc")