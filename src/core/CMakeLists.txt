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
    compiling-bytecode)

foreach(EXAMPLE ${EXAMPLES})
  set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/core/${EXAMPLE}")

  add_executable(${EXAMPLE} ${EXAMPLE}/${EXAMPLE}.c) # The path is generally src/<category>/<example_name>/<example_name>.c

  # Set the custom binary output directory
  set_property(TARGET ${EXAMPLE}
               PROPERTY RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

  # Copy Ruby files (if any) to the example's output directory
  file(GLOB RUBY_FILES
       "${EXAMPLE}/*.rb")
  file(COPY ${RUBY_FILES}
       DESTINATION ${OUTPUT_DIRECTORY})

  target_compile_options(${EXAMPLE} PRIVATE ${WARNINGS})
  target_link_libraries(${EXAMPLE} PRIVATE -lmruby ${MATH_LIBRARY})

  if("${EXAMPLE}" MATCHES "sdl")
    target_include_directories(${EXAMPLE} PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${EXAMPLE} PRIVATE ${SDL2_LIBRARIES})
  endif()
endforeach()
