
# Define the library (assuming it is a library, not an executable)
#add_library(vertex
  # List source files here if it has .cpp files
  # source_file.cpp
  #)

  #target_include_directories(vertex
  #  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} # This adds src/mesh as an include directory
  #)

# For hpp only library
add_library(vertex INTERFACE)

# Specify include directories
target_include_directories(vertex INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
