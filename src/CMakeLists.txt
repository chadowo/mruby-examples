add_subdirectory(core)
if(BUILD_SDL)
  add_subdirectory(sdl2)
endif()

message(STATUS "Binaries will be placed on the subdirectory bin/ of the build directory")
