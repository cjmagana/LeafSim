#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(LeafSim)

#Find ROOT
list(APPEND CMAKE_PREFIX_PATH ~/Documents/ROOT/build/)
find_package(ROOT REQUIRED)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
include_directories(~/Documents/ROOT/build/include/)
#link_directories(${ROOT_LIBRARY_DIR})
link_directories(~/Documents/ROOT/build/lib/)
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(LeafSim main.cc ${sources} ${headers})
target_link_libraries(LeafSim ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(main DEPENDS LeafSim)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS LeafSim DESTINATION bin)
install(FILES ${_SCRIPTS} DESTINATION share/LeafSim)

