//  GEOM GEOMGUI : GUI for Geometry component
//
//  Copyright (C) 2003  CEA
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.salome-platform.org or email : webmaster.salome@opencascade.org
//
//
//
//  File   : OperationGUI_MaterialDlg.h
//  Author : Julia DOROVSKIKH
//  Module : GEOM
//  $Header$

#ifndef DIALOGBOX_MATERIAL_H
#define DIALOGBOX_MATERIAL_H

#include "GEOMBase_Skeleton.h"
#include "DlgRef_1List1Spin1Btn_QTD.h"


//=================================================================================
// class    : OperationGUI_MaterialDlg
// purpose  :
//=================================================================================
class OperationGUI_MaterialDlg : public GEOMBase_Skeleton
{ 
    Q_OBJECT

public:
    OperationGUI_MaterialDlg (QWidget* parent, const char* name,
			      SALOME_Selection* Sel,
			      GEOM::ListOfGO ListShapes,
			      bool modal = FALSE, WFlags fl = 0);
    ~OperationGUI_MaterialDlg();

private:
    void Init();
    void enterEvent(QEvent* e);

    QWidget* myParentDlg;

    GEOM::ListOfGO myListShapes;
    GEOM::ListOfLong   myListMaterials;

    DlgRef_1List1Spin1Btn_QTD* GroupPoints;

    int myShapeCol;
    int myMaterCol;

private slots:
    void ClickOnOk();
    void ActivateThisDialog();
    void SelectionIntoArgument();
    void SetMaterial();

};

#endif // DIALOGBOX_MATERIAL_H