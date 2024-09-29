# Create the main executable with .cpp files in the src directory
add_executable(proj
  main.cpp
  # Add other .cpp files
)

# Include directories for the headers in the src directory
#target_include_directories(proj
#  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
#)



# Specify where the header files (.hpp) are located
target_include_directories(proj
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}   # Includes headers in the src directory
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mesh   # Includes headers in the mesh directory
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/point_cloud   # Includes headers in vert_cloud directory
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vertex  # Includes headers in vertex directory
)

# Add subdirectories for mesh and he (header-only libraries)
add_subdirectory(mesh)
add_subdirectory(point_cloud)
add_subdirectory(vertex)


