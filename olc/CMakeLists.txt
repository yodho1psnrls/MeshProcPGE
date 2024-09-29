# Define the mesh library (assuming mesh is a library, not an executable)
# add_library(olc
  # olcPixelGameEngine.cpp
# )


# Set the include directories for mesh header files
# target_include_directories(olc
 # PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} # This adds src/mesh as an include directory
# )





# Define the source files for the olc library
set(SOURCE_FILES olcPixelGameEngine.cpp)

# Create a static library for olc (not an executable)
add_library(olc STATIC ${SOURCE_FILES})

# Specify include directories for the olc library
target_include_directories(olc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# Link OLC PGE Dependancies
if (WIN32)
  target_link_libraries(olc
    PRIVATE opengl32   # Link OpenGL
    PRIVATE gdiplus    # Link GDI+ for window drawing
    PRIVATE shlwapi    # Shell Lightweight Utility Library
    PRIVATE dwmapi     # Desktop Window Manager API (needed for DwmFlush)
  )
endif()
