#
cmake_minimum_required (VERSION 3.8)

project ("SalomeGeom")
set(third_party "D:/source/cenos-backend/third-party")
set(FREETYPE-RELEASE "${third_party}/freetype-RELEASE")
set(TCL-RELEASE "${third_party}/tcl8.6.11-RELEASE")

find_package(VTK 8.2 REQUIRED HINTS "${third_party}/vtk-8.2.0-${CMAKE_BUILD_TYPE}")


find_package(OpenCASCADE REQUIRED PATHS ${third_party}/OpenCASCADE-${CMAKE_BUILD_TYPE} NO_DEFAULT_PATH)
message (STATUS "OpenCASCADE_DIR = ${OpenCASCADE_DIR}")


include_directories(
  ${OpenCASCADE_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

# to prevent inclusion of Basics_OCCTVersion.hxx
MATH(EXPR OCC_VERSION_LARGE "${OpenCASCADE_MAJOR_VERSION} << 24 | ${OpenCASCADE_MINOR_VERSION}  << 16 | ${OpenCASCADE_MAINTENANCE_VERSION}  << 8" )

add_subdirectory("src/BlockFix")
add_subdirectory("src/GeomAlgo")
add_subdirectory("src/ShHealOper")

set(bin_destination "${PROJECT_SOURCE_DIR}/bin")
set(inc_destination "${PROJECT_SOURCE_DIR}/inc")
set(lib_destination "${PROJECT_SOURCE_DIR}/lib")
install(TARGETS BlockFix GeomAlgo ShHealOper DESTINATION ${bin_destination} LIBRARY DESTINATION ${lib_destination})


install(DIRECTORY  "src/BlockFix/" DESTINATION ${inc_destination}
    FILES_MATCHING PATTERN "*.h*")
install(DIRECTORY  "src/GeomAlgo/" DESTINATION ${inc_destination}
    FILES_MATCHING PATTERN "*.h*")
install(DIRECTORY  "src/ShHealOper/" DESTINATION ${inc_destination}
    FILES_MATCHING PATTERN "*.h*")