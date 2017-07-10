# $Id: CMakeLists.txt 86065 2014-11-07 08:51:15Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(g4amstest)

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
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# find boost::program_options and boost:date_time
find_package(Boost 1.35.0 COMPONENTS date_time program_options)
if(Boost_DATE_TIME_FOUND)
  add_definitions(-DHAS_BOOST_DATE_TIME)
endif(Boost_DATE_TIME_FOUND)
if(Boost_PROGRAM_OPTIONS_FOUND)
  add_definitions(-DHAS_BOOST_PROGRAM_OPTIONS)
endif(Boost_PROGRAM_OPTIONS_FOUND)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(g4amstest g4amstest.cc ${sources} ${headers})
target_link_libraries(g4amstest ${Geant4_LIBRARIES} ${Boost_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build g4amstest. This is so that we can run the executable directly
# because it relies on these scripts being in the current working directory.
#
set(G4AMSTEST_SCRIPTS
  g4amstest.in
  init_vis.mac
  vis.mac
  )

foreach(_script ${G4AMSTEST_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()


# g42so
find_program(G42SO g42so PATHS ~/src/t4g4interface/g42so)
set(LIBAMSTEST_FILES
  "${CMAKE_SOURCE_DIR}/src/AMSTestDetectorConstruction.cc"
  "${CMAKE_SOURCE_DIR}/src/AMSTestDetectorConstructionMessenger.cc"
  "${CMAKE_SOURCE_DIR}/src/AMSTestPrimaryGeneratorAction.cc"
  )
if(G42SO)
  message(STATUS "Found g42so: ${G42SO}")
  add_custom_command(OUTPUT libAMSTest.so
    COMMAND "${G42SO}" -o libAMSTest.so -I "${CMAKE_SOURCE_DIR}/include" --geant4-config "${Geant4_DIR}/../../bin/geant4-config" ${LIBAMSTEST_FILES}
    DEPENDS ${LIBAMSTEST_FILES}
    VERBATIM)
  add_custom_target(libAMSTest ALL DEPENDS libAMSTest.so)
else()
  message(STATUS "Could not find g42so")
endif()
