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

#INCLUDE(UseQt4Ext)
INCLUDE(${QT_USE_FILE})

# --- options ---

# additional include directories
INCLUDE_DIRECTORIES(
  ${QT_INCLUDE_DIRS}
  ${PTHREAD_INCLUDE_DIR}
  ${VTK_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIR}
  ${CAS_INCLUDE_DIRS}
  ${KERNEL_INCLUDE_DIRS}
  ${GUI_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/idl
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/src/OBJECT
  ${PROJECT_SOURCE_DIR}/src/GEOMClient
  ${PROJECT_SOURCE_DIR}/src/GEOMImpl
  ${PROJECT_SOURCE_DIR}/src/GEOMGUI
  ${PROJECT_SOURCE_DIR}/src/GEOMBase
  ${PROJECT_SOURCE_DIR}/src/DlgRef
  ${PROJECT_BINARY_DIR}/src/DlgRef
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

# additional preprocessor / compiler flags
ADD_DEFINITIONS(
  ${CAS_DEFINITIONS}
  ${OMNIORB_DEFINITIONS}
  ${QT_DEFINITIONS}
  )

# libraries to link to
SET(_link_LIBRARIES
  GEOMBase
  )

# --- resources ---

# resource files / to be processed by uic
SET(_uic_files
  )

# --- headers ---

SET(GroupGUI_HEADERS
  GroupGUI.h
  GroupGUI_BooleanDlg.h
  GroupGUI_GroupDlg.h
  )

# header files / to be processed by moc
SET(_moc_HEADERS
  GroupGUI.h
  GroupGUI_BooleanDlg.h
  GroupGUI_GroupDlg.h
  )

# header files / uic wrappings
QT4_WRAP_UI(_uic_HEADERS ${_uic_files})

# --- sources ---

# sources / moc wrappings
QT4_WRAP_CPP(_moc_SOURCES ${_moc_HEADERS})

SET(GroupGUI_SOURCES
  GroupGUI.cxx
  GroupGUI_BooleanDlg.cxx
  GroupGUI_GroupDlg.cxx
  ${_moc_SOURCES}
  ${_uic_files}
  )

# --- rules ---

ADD_LIBRARY(GroupGUI ${GroupGUI_SOURCES})
TARGET_LINK_LIBRARIES(GroupGUI ${_link_LIBRARIES})
INSTALL(TARGETS GroupGUI EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

