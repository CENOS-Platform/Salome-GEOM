# Copyright (C) 2012-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

INCLUDE(${QT_USE_FILE})
INCLUDE(${VTK_USE_FILE})

# --- options ---

# additional include directories
INCLUDE_DIRECTORIES(
  ${VTK_INCLUDE_DIRS}
  ${PTHREAD_INCLUDE_DIR}
  ${QT_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIR}
  ${CAS_INCLUDE_DIRS}
  ${KERNEL_INCLUDE_DIRS}
  ${GUI_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/idl
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/src/OCC2VTK
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

# additional preprocessor / compiler flags
ADD_DEFINITIONS(
  ${CAS_DEFINITIONS}
  ${OMNIORB_DEFINITIONS}
  ${QT_DEFINITIONS}
  )

# libraries to link to
SET(_link_LIBRARIES
  OCC2VTK
  SalomeIDLGEOM
  ${CAS_TKBRep}
  ${KERNEL_SALOMELocalTrace}
  ${GUI_SalomeObject}
  ${GUI_SVTK}
  ${QT_QTCORE_LIBRARY}
  vtkRenderingMatplotlib
  vtkInteractionStyle
  #${VTK_LIBRARIES}
  )

# --- headers ---

SET(OBJECT_HEADERS
  GEOM_Actor.h
  GEOM_AISShape.hxx
  GEOM_AISShape.ixx
  GEOM_AISShape.jxx
  Handle_GEOM_AISShape.hxx
  GEOM_TopWireframeShape.hxx
  GEOM_TopWireframeShape.ixx
  GEOM_TopWireframeShape.jxx
  Handle_GEOM_TopWireframeShape.hxx
  GEOM_InteractiveObject.hxx
  GEOM_InteractiveObject.ixx
  GEOM_InteractiveObject.jxx
  Handle_GEOM_InteractiveObject.hxx
  GEOM_AISTrihedron.hxx
  GEOM_VTKTrihedron.hxx
  GEOM_VTKPropertyMaterial.hxx
  GEOM_AISVector.hxx
  GEOM_OBJECT_defs.hxx
  GEOM_OCCReader.h
  GEOM_SmartPtr.h
  GEOM_PainterPolyDataMapper.h
  GEOM_DeviceActor.h
  GEOM_Constants.h
  )
# --- sources ---

SET(OBJECT_SOURCES
  GEOM_Actor.cxx
  GEOM_OCCReader.cxx
  GEOM_AISShape.cxx
  GEOM_TopWireframeShape.cxx
  GEOM_InteractiveObject.cxx
  GEOM_AISTrihedron.cxx
  GEOM_VTKTrihedron.cxx
  GEOM_VTKPropertyMaterial.cxx
  GEOM_AISVector.cxx
  GEOM_PainterPolyDataMapper.cxx
  GEOM_DeviceActor.cxx
  GEOM_Constants.cxx
  )

# --- rules ---

ADD_LIBRARY(GEOMObject ${OBJECT_SOURCES})
TARGET_LINK_LIBRARIES(GEOMObject ${_link_LIBRARIES})
INSTALL(TARGETS GEOMObject EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

INSTALL(FILES ${OBJECT_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})