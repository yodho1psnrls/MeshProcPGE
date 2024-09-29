# No add_library: Since these are header-only libraries, you don’t need to define them using add_library(). The target_include_directories command will expose the header files to the rest of your project.
# Include Directories: The target_include_directories command ensures that both mesh and mesh/he directories are available for including their .hpp files in other parts of the project.
# Subdirectory: You still add he as a subdirectory to ensure its headers are properly included.


# Define the mesh library (assuming mesh is a library, not an executable)
#add_library(mesh
  # List source files here if mesh has .cpp files
  # mesh_source_file.cpp
#)


# Set the include directories for mesh header files
#target_include_directories(mesh
#  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} # This adds src/mesh as an include directory
#)

# For hpp only library
add_library(mesh INTERFACE)

# Specify include directories
target_include_directories(mesh INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


add_subdirectory(he) # Add the he subdirectory and include its headers

# Link the he library to the mesh library (if mesh depends on he)
# target_link_libraries(mesh PRIVATE he)
