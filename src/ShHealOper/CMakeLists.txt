# Copyright (C) 2012-2021  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
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


SET(ShHealOper_HEADERS
  ShHealOper_CloseContour.hxx
  ShHealOper_EdgeDivide.hxx
  ShHealOper_FillHoles.hxx
  ShHealOper_RemoveFace.hxx
  ShHealOper_RemoveInternalWires.hxx
  ShHealOper_Sewing.hxx
  ShHealOper_ShapeProcess.hxx
  ShHealOper_SpiltCurve2d.hxx
  ShHealOper_SplitCurve2d.hxx
  ShHealOper_SplitCurve3d.hxx
  ShHealOper_ChangeOrientation.hxx
  ShHealOper_Tool.hxx
  ShHealOper_ModifStats.hxx
  )
# --- sources ---

SET(ShHealOper_SOURCES
  ShHealOper_CloseContour.cxx
  ShHealOper_EdgeDivide.cxx
  ShHealOper_FillHoles.cxx
  ShHealOper_RemoveFace.cxx
  ShHealOper_RemoveInternalWires.cxx
  ShHealOper_Sewing.cxx
  ShHealOper_ShapeProcess.cxx
  ShHealOper_SplitCurve2d.cxx
  ShHealOper_SplitCurve3d.cxx
  ShHealOper_ChangeOrientation.cxx
  ShHealOper_Tool.cxx
  )

# --- rules ---


ADD_LIBRARY(ShHealOper SHARED ${ShHealOper_SOURCES})
target_include_directories(ShHealOper PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

TARGET_LINK_LIBRARIES(ShHealOper ${OpenCASCADE_LIBRARIES})

