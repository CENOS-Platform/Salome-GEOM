#
cmake_minimum_required (VERSION 3.8)

project ("SalomeGeom")
if (NOT DEFINED THIRD_PARTY_DIR)
    message(FATAL_ERROR  "THIRD_PARTY_DIR not defined")
endif()


find_package(VTK 9.2 REQUIRED HINTS "${THIRD_PARTY_DIR}/vtk-${CMAKE_BUILD_TYPE}")


find_package(OpenCASCADE REQUIRED PATHS ${THIRD_PARTY_DIR}/OpenCASCADE-${CMAKE_BUILD_TYPE} NO_DEFAULT_PATH)
message (STATUS "OpenCASCADE_DIR = ${OpenCASCADE_DIR}")


include_directories(
  ${OpenCASCADE_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

# to prevent inclusion of Basics_OCCTVersion.hxx
MATH(EXPR OCC_VERSION_LARGE "${OpenCASCADE_MAJOR_VERSION} << 24 | ${OpenCASCADE_MINOR_VERSION}  << 16 | ${OpenCASCADE_MAINTENANCE_VERSION}  << 8" )

add_subdirectory("src/BlockFix")
add_subdirectory("src/GEOMAlgo")
add_subdirectory("src/ShHealOper")

set(bin_destination "${CMAKE_INSTALL_PREFIX}/bin")
set(inc_destination "${CMAKE_INSTALL_PREFIX}/inc")
set(lib_destination "${CMAKE_INSTALL_PREFIX}/lib")
install(TARGETS BlockFix GeomAlgo ShHealOper DESTINATION ${bin_destination} LIBRARY DESTINATION ${lib_destination})


install(DIRECTORY  "src/BlockFix/" DESTINATION ${inc_destination}
    FILES_MATCHING PATTERN "*.h*")
install(DIRECTORY  "src/GEOMAlgo/" DESTINATION ${inc_destination}
    FILES_MATCHING PATTERN "*.h*")
install(DIRECTORY  "src/ShHealOper/" DESTINATION ${inc_destination}
    FILES_MATCHING PATTERN "*.h*")