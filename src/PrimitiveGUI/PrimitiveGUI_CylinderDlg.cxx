//  GEOM GEOMGUI : GUI for Geometry component
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : PrimitiveGUI_CylinderDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "PrimitiveGUI_CylinderDlg.h"

#include <gp_Lin.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <Precision.hxx>
#include "QAD_Config.h"

//=================================================================================
// class    : PrimitiveGUI_CylinderDlg()
// purpose  : Constructs a PrimitiveGUI_CylinderDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
PrimitiveGUI_CylinderDlg::PrimitiveGUI_CylinderDlg(QWidget* parent, const char* name, PrimitiveGUI* thePrimitiveGUI, SALOME_Selection* Sel, bool modal, WFlags fl)
  :GEOMBase_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_CYLINDER_PV")));
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_CYLINDER_DXYZ")));
  QPixmap image2(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_CYLINDER_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_CYLINDER"));
  RadioButton1->setPixmap(image0);
  RadioButton2->setPixmap(image1);
  RadioButton3->close(TRUE);

  GroupPoints = new DlgRef_2Sel2Spin(this, "GroupPoints");
  GroupPoints->GroupBox1->setTitle(tr("GEOM_ARGUMENTS"));
  GroupPoints->TextLabel1->setText(tr("GEOM_BASE_POINT"));
  GroupPoints->TextLabel2->setText(tr("GEOM_VECTOR"));
  GroupPoints->TextLabel3->setText(tr("GEOM_RADIUS"));
  GroupPoints->TextLabel4->setText(tr("GEOM_HEIGHT"));
  GroupPoints->PushButton1->setPixmap(image2);
  GroupPoints->PushButton2->setPixmap(image2);

  GroupDimensions = new DlgRef_2Spin(this, "GroupDimensions");
  GroupDimensions->GroupBox1->setTitle(tr("GEOM_BOX_OBJ"));
  GroupDimensions->TextLabel1->setText(tr("GEOM_RADIUS"));
  GroupDimensions->TextLabel2->setText(tr("GEOM_HEIGHT"));

  Layout1->addWidget(GroupPoints, 1, 0);
  Layout1->addWidget(GroupDimensions, 1, 0);
  /***************************************************************/

  /* Initialisations */
  myPrimitiveGUI = thePrimitiveGUI;
  Init();
}


//=================================================================================
// function : ~PrimitiveGUI_CylinderDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
PrimitiveGUI_CylinderDlg::~PrimitiveGUI_CylinderDlg()
{
    // no need to delete child widgets, Qt does it all for us
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void PrimitiveGUI_CylinderDlg::Init()
{
  /* init variables */
  myConstructorId = 0;
  myEditCurrentArgument = GroupPoints->LineEdit1;

  myRadius = 100.0;
  myHeight = 300.0;
  myOkRadius = myOkHeight = true;
  myOkPoint1 = myOkDir = false;

  myEdgeFilter = new GEOM_EdgeFilter(StdSelect_Line, myGeom);
  myVertexFilter = new GEOM_ShapeTypeFilter(TopAbs_VERTEX, myGeom);
  mySelection->AddFilter(myVertexFilter);

  /* Get setting of step value from file configuration */
  QString St = QAD_CONFIG->getSetting("Geometry:SettingsGeomStep");
  step = St.toDouble();

  /* min, max, step and decimals for spin boxes & initial values */
  /* First constructor : radius */
  GroupPoints->SpinBox_DX->RangeStepAndValidator(0.001, 999.999, step, 3);
  /* First constructor : algebric height */
  GroupPoints->SpinBox_DY->RangeStepAndValidator(-999.999, 999.999, step, 3);
  /* Second constructor : radius */
  GroupDimensions->SpinBox_DX->RangeStepAndValidator(0.001, 999.999, step, 3);
  /* Second constructor : algebric height */
  GroupDimensions->SpinBox_DY->RangeStepAndValidator(-999.999, 999.999, step, 3);

  GroupPoints->SpinBox_DX->SetValue(myRadius);
  GroupPoints->SpinBox_DY->SetValue(myHeight);
  GroupDimensions->SpinBox_DX->SetValue(myRadius);  
  GroupDimensions->SpinBox_DY->SetValue(myHeight);  

  /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));
  connect(GroupConstructors, SIGNAL(clicked(int)), this, SLOT(ConstructorsClicked(int)));

  connect(GroupPoints->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPoints->PushButton2, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

  connect(GroupPoints->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(GroupPoints->LineEdit2, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(GroupPoints->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(GroupPoints->SpinBox_DY, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(GroupDimensions->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(GroupDimensions->SpinBox_DY, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));

  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), GroupPoints->SpinBox_DX, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), GroupPoints->SpinBox_DY, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), GroupDimensions->SpinBox_DX, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), GroupDimensions->SpinBox_DY, SLOT(SetStep(double)));
  
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument())) ;

  /* displays Dialog */
  GroupDimensions->hide();
  GroupPoints->show();
  this->show();

  return ;
}


//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void PrimitiveGUI_CylinderDlg::ConstructorsClicked(int constructorId)
{
  myConstructorId = constructorId;
  mySelection->ClearFilters();
  myGeomBase->EraseSimulationShape();
  disconnect(mySelection, 0, this, 0);
  myOkHeight = myOkRadius = true;
  myRadius = 100.0;
  myHeight = 300.0;

  switch(myConstructorId)
    { 
    case 0 :
      {
	GroupDimensions->hide();
	resize(0, 0);
	GroupPoints->show();
	myOkPoint1 = myOkDir = false;

	myEditCurrentArgument = GroupPoints->LineEdit1;
	GroupPoints->LineEdit1->setText(tr(""));
	GroupPoints->LineEdit2->setText(tr(""));
	
	GroupPoints->SpinBox_DX->SetValue(myRadius);
	GroupPoints->SpinBox_DY->SetValue(myHeight);

	/* filter for next selection */
	mySelection->AddFilter(myVertexFilter);
	connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
	break;
      }
    case 1 :
      { 
	GroupPoints->hide();
	resize( 0, 0 );
	GroupDimensions->show();
	myOkPoint1 = myOkDir = true;

	GroupDimensions->SpinBox_DX->SetValue(myRadius);
	GroupDimensions->SpinBox_DY->SetValue(myHeight);
	myPoint1.SetCoord(0.0, 0.0, 0.0);
	myDir.SetCoord(0.0, 0.0, 1.0);

	MakeCylinderSimulationAndDisplay();
	break;
      }
    }
  return ;
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void PrimitiveGUI_CylinderDlg::ClickOnOk()
{
  this->ClickOnApply();
  ClickOnCancel();
  return;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void PrimitiveGUI_CylinderDlg::ClickOnApply()
{
  QAD_Application::getDesktop()->putInfo(tr(""));
  if (mySimulationTopoDs.IsNull())
    return;
  myGeomBase->EraseSimulationShape();
  mySimulationTopoDs.Nullify();

  if(myOkPoint1 && myOkDir && myOkRadius && myOkHeight) {
    gp_Dir aDir = myDir;
    /* allows user to reverse direction of construction with a negative height */
    if(this->myHeight < -Precision::Confusion())
      aDir.Reverse();
    myPrimitiveGUI->MakeCylinderAndDisplay(myPoint1, aDir, myRadius, fabs(myHeight)); 
  }
  return;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void PrimitiveGUI_CylinderDlg::SelectionIntoArgument()
{
  myGeomBase->EraseSimulationShape(); 
  myEditCurrentArgument->setText("");
  QString aString = ""; /* name of selection */

  int nbSel = myGeomBase->GetNameOfSelectedIObjects(mySelection, aString);
  if(nbSel != 1) {
    if(myEditCurrentArgument == GroupPoints->LineEdit1)
      myOkPoint1 = false;
    else if (myEditCurrentArgument == GroupPoints->LineEdit2)
      myOkDir = false;
    return;
  }

  /* nbSel == 1 */
  TopoDS_Shape S;
  if(!myGeomBase->GetTopoFromSelection(mySelection, S))
    return;
 
  /*  gp_Pnt : not used */
  if (myEditCurrentArgument == GroupPoints->LineEdit1 && myGeomBase->VertexToPoint(S, myPoint1)) {
    GroupPoints->LineEdit1->setText(aString);
    myOkPoint1 = true;
  }    
  else if (myEditCurrentArgument == GroupPoints->LineEdit2) {
    BRepAdaptor_Curve curv(TopoDS::Edge(S));
    myDir = curv.Line().Direction();
    GroupPoints->LineEdit2->setText(aString);
    myOkDir = true;
  }
  
  if(myOkPoint1 && myOkDir && myOkRadius && myOkHeight)
    MakeCylinderSimulationAndDisplay();
  return;
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void PrimitiveGUI_CylinderDlg::SetEditCurrentArgument()
{
  if(myConstructorId != 0)
    return;

  QPushButton* send = (QPushButton*)sender();
  mySelection->ClearFilters();

  if(send == GroupPoints->PushButton1) {
    GroupPoints->LineEdit1->setFocus();
    myEditCurrentArgument = GroupPoints->LineEdit1;
    mySelection->AddFilter(myVertexFilter);
  }
  else if(send == GroupPoints->PushButton2) {
    GroupPoints->LineEdit2->setFocus();
    myEditCurrentArgument = GroupPoints->LineEdit2;
    mySelection->AddFilter(myEdgeFilter);
  }
  this->SelectionIntoArgument();

  return;
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void PrimitiveGUI_CylinderDlg::LineEditReturnPressed()
{  
  QLineEdit* send = (QLineEdit*)sender();
  if(send == GroupPoints->LineEdit1)
    myEditCurrentArgument = GroupPoints->LineEdit1;
  else if (send == GroupPoints->LineEdit2)
    myEditCurrentArgument = GroupPoints->LineEdit2;
  else
    return;

  GEOMBase_Skeleton::LineEditReturnPressed();
  return;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void PrimitiveGUI_CylinderDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  if(myConstructorId == 0) {
    GroupPoints->LineEdit1->setFocus();
    myEditCurrentArgument = GroupPoints->LineEdit1;
    mySelection->AddFilter(myVertexFilter);
  }
  if(!mySimulationTopoDs.IsNull())
    myGeomBase->DisplaySimulationShape(mySimulationTopoDs);
  return;
}


//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void PrimitiveGUI_CylinderDlg::enterEvent(QEvent* e)
{
  if (GroupConstructors->isEnabled())
    return;
  this->ActivateThisDialog();
  return;
}


//=================================================================================
// function : ValueChangedInSpinBox
// purpose  :
//=================================================================================
void PrimitiveGUI_CylinderDlg::ValueChangedInSpinBox( double newValue )
{  
  myGeomBase->EraseSimulationShape();
  mySimulationTopoDs.Nullify();
  QObject* send = (QObject*)sender();
  
    /* radius */
  if(send == GroupPoints->SpinBox_DX || send == GroupDimensions->SpinBox_DX) {
    myRadius = newValue;
    myOkRadius = true;
  } /* algebric height */
  else if(send == GroupPoints->SpinBox_DY || send == GroupDimensions->SpinBox_DY) {
    myHeight = newValue;
    myOkHeight = true;
  }

  if (myOkPoint1 && myOkDir && myOkRadius && myOkHeight)
    MakeCylinderSimulationAndDisplay();
  return;
}


//=================================================================================
// function : MakeCylinderSimulationAndDisplay()
// purpose  :
//=================================================================================
void PrimitiveGUI_CylinderDlg::MakeCylinderSimulationAndDisplay() 
{
  myGeomBase->EraseSimulationShape();
  mySimulationTopoDs.Nullify();
  gp_Dir aDir = this->myDir;
  
  try {
    /* allows user to reverse direction of construction with a negative height */
    if(this->myHeight < -Precision::Confusion())
      aDir.Reverse();

    gp_Ax2 anAxis(this->myPoint1, aDir);

    mySimulationTopoDs = BRepPrimAPI_MakeCylinder(anAxis, this->myRadius, fabs(myHeight)).Shape();
    myGeomBase->DisplaySimulationShape(mySimulationTopoDs);
  }
  catch(Standard_Failure) {
    MESSAGE("Exception catched in MakeCylinderSimulationAndDisplay");
  }
  return;
}