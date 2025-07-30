set(SOURCES
  main.cpp
)

add_executable(server ${SOURCES})

target_include_directories(server
  PRIVATE ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(server PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_compile_options(server PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang>: -Wall -Wextra -Wpedantic -fdiagnostics-color=always >
  $<$<CXX_COMPILER_ID:MSVC>: /W4 >
)