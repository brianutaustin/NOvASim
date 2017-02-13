#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(nova)

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

# Setup ROOT include directory and compile definition
find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
#include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include
                    /Users/dphan/Workspace/Geant4/cry_v1.7/src
                    /Users/dphan/Workspace/ROOT/root-v5-34-36-install/include/
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB headers /Users/dphan/Workspace/Geant4/cry_v1.7/src/*.h)
file(GLOB headers /Users/dphan/Workspace/ROOT/root-v5-34-36-install/include/*.h)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(nova nova.cc ${sources} ${headers})
target_link_libraries(nova
${Geant4_LIBRARIES}
${ROOT_LIBRARIES}
/Users/dphan/Workspace/Geant4/cry_v1.7/lib/libCRY.a
/Users/dphan/Workspace/ROOT/root-v5-34-36-install/lib/libTree.so
/Users/dphan/Workspace/ROOT/root-v5-34-36-install/lib/libCore.so
/Users/dphan/Workspace/ROOT/root-v5-34-36-install/lib/libRIO.so)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build LXe. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(nova_SCRIPTS
  FiberBulkAbsorb.cfg
	GraphExit.C
	NOVA.cfg
	PMMABulkAbsorb.dat
	PSTBulkAbsorb.cfg
	UPS923.cfg
	WLSabsorbguess.cfg
	WLSemit.cfg
	apd_qe_1.dat
	bad.WLSabsorb.cfg
	bisMSB_emission.txt
	fiberPSTabsorb.dat
	full_popop_emission.cfg
	graphfromfile.C
	nova.NOVA.cfg
	pTP_emission.dat
	pl_file_1.dat
	ppo_emission.txt
	proto.NOVA.cfg
	qe_file_2.dat
	rutile.txt
	scripts/AbsProfile.C
  scripts/CompRef.C
  scripts/CompileStats.C
  scripts/FindAvg.C
  scripts/PlotEdepStuff.C
  scripts/Spectrum.C
  scripts/TGraph.C
  scripts/ViewSpectrum.C
  scripts/readpoints.C
  scripts/reflections.C
  scripts/runnova.csh
  scripts/runprototype.csh
  scripts/runprototype_center.csh
	setgun.mac
	setsource.mac
	setsource1.mac
	setsource2.mac
	setsource3.mac
	setsource4.mac
	setsource5.mac
	setsource6.mac
	setsource7.mac
	y11_abs_length.dat
)

foreach(_script ${nova_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS nova DESTINATION bin)
