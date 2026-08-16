set(EXAMPLES
    hello-world
    hello-file
    objects
    pushing-variables
    method-defined
    call-methods
    using-cdata
    kwargs
    blocks
    compiling-bytecode
    sdl-wrapper)

foreach(EXAMPLE ${EXAMPLES})
  set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${EXAMPLE}")

  add_executable(${EXAMPLE} ${EXAMPLE}/${EXAMPLE}.c)

  set_property(TARGET ${EXAMPLE}
               PROPERTY RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

  # Copy Ruby files (if any) to the example's output directory
  file(GLOB RUBY_FILES
       "${EXAMPLE}/*.rb")
  file(COPY ${RUBY_FILES}
       DESTINATION ${OUTPUT_DIRECTORY})

  target_compile_options(${EXAMPLE} PRIVATE ${WARNINGS})
  target_link_libraries(${EXAMPLE} PRIVATE -lmruby -lm)

  if("${EXAMPLE}" MATCHES "sdl")
    target_include_directories(${EXAMPLE} PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${EXAMPLE} PRIVATE ${SDL2_LIBRARIES})
  endif()
endforeach()

message(STATUS "Binaries will be placed on the subdirectory bin/ relative to the build directory")
