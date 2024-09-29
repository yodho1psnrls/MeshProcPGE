# Define the point cloud library
# (assuming that it is a library, not an executable)
#add_library(point_cloud
  # List source files here if it has .cpp files
  # source_file.cpp
  #)

  #target_include_directories(point_cloud
  #  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} # This adds src/mesh as an include directory
  #)

# For hpp only library
add_library(point_cloud INTERFACE)

# Specify include directories
target_include_directories(point_cloud INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
