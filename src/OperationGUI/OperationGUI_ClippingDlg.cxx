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
//  File   : OperationGUI_ClippingDlg.cxx
//  Author : Michael Zorin
//  Module : GEOM
//  $Header: 

#include "OperationGUI_ClippingDlg.h"
#include "DlgRef_SpinBox.h"

#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_RightFrame.h"
#include "VTKViewer_ViewFrame.h"
#include "OCCViewer_ViewFrame.h"
#include "QAD_MessageBox.h"
#include "utilities.h"
#include "OCCViewer_ViewPort3d.h"
#include "OCCViewer_Viewer3d.h"
#include <V3d_View.hxx>
#include <V3d.hxx>

//#include <V3d_Plane.hxx>

// QT Includes
#include <qapplication.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>

// VTK Includes
#include <vtkCamera.h>
#include <vtkRenderer.h>

//=================================================================================
// class    : OperationGUI_ClippingDlg()
// purpose  : 
//=================================================================================
OperationGUI_ClippingDlg::OperationGUI_ClippingDlg( QWidget* parent, const char* name, SALOME_Selection* Sel, bool modal, WFlags fl )
  : GEOMBase_Skeleton( parent, "OperationGUI_ClippingDlg", 0, false,
    WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  setCaption( tr( "Change clipping range" ) );
  
  GroupConstructors->close();
  GroupBoxName->close();

  /***************************************************************/
  GroupArguments = new QGroupBox( this, "GroupArguments" );
  GroupArguments->setColumnLayout(0, Qt::Vertical );
  GroupArguments->layout()->setSpacing( 0 );
  GroupArguments->layout()->setMargin( 0 );
  QGridLayout* GroupArgumentsLayout = new QGridLayout( GroupArguments->layout() );
  GroupArgumentsLayout->setAlignment( Qt::AlignTop );
  GroupArgumentsLayout->setSpacing( 6 );
  GroupArgumentsLayout->setMargin( 11 );
  
  // Controls
  TextLabelNear = new QLabel( GroupArguments, "TextLabelNear" );
  TextLabelNear->setText( tr( "Near"  ) );
  //mzn TextLabelNear->setFixedWidth(74);
  GroupArgumentsLayout->addWidget( TextLabelNear, 0, 0 );

  SpinBox_Near = new DlgRef_SpinBox( GroupArguments, "SpinBox_Near");
  GroupArgumentsLayout->addWidget( SpinBox_Near, 0, 1 );

  TextLabelFar = new QLabel( GroupArguments, "TextLabelFar" );
  TextLabelFar->setText( tr( "Far"  ) );
  //mzn TextLabelFar->setFixedWidth(74);
  GroupArgumentsLayout->addWidget( TextLabelFar, 0, 2 );
  
  SpinBox_Far = new DlgRef_SpinBox( GroupArguments, "SpinBox_Far");
  GroupArgumentsLayout->addWidget( SpinBox_Far, 0, 3 );
  
  resetButton  = new QPushButton( GroupArguments, "resetButton" );
  resetButton->setText( tr( "Reset"  ) );
  GroupArgumentsLayout->addWidget( resetButton, 0, 4 );

  TypeCB = new QComboBox( GroupArguments, "TypeCB" );
  TypeCB->insertItem(tr("OFF"));
  TypeCB->insertItem(tr("BACK"));
  TypeCB->insertItem(tr("FRONT"));
  TypeCB->insertItem(tr("SLICE"));
  GroupArgumentsLayout->addMultiCellWidget( TypeCB, 1, 1, 0, 4 );
  
  Layout1->addWidget( GroupArguments, 2, 0 );
  
  /* Initialisations */
  SpinBox_Near->RangeStepAndValidator( -999999.999, +999999.999, 10.0, 3 );
  SpinBox_Far->RangeStepAndValidator( -999999.999, +999999.999, 10.0, 3 );
  
  //GeometryGUI* aGeomGUI = GeometryGUI::GetGeomGUI();
  
  /* signals and slots connections */
  connect( buttonOk, SIGNAL( clicked() ),     this, SLOT( ClickOnOk() ) );
  connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( ClickOnCancel() ) ) ;
  connect( buttonApply, SIGNAL( clicked() ),  this, SLOT( ClickOnApply() ) );
  
  connect( resetButton, SIGNAL (clicked() ), this, SLOT( onReset() ) ) ;
  
  //  connect( aGeomGUI, SIGNAL ( SignalDeactivateActiveDialog() ), this, SLOT( onDeactivate() ) ) ;
  //connect( aGeomGUI, SIGNAL ( SignalCloseAllDialogs() ), this, SLOT( ClickOnCancel() ) ) ;

  Init();
}

//=================================================================================
// function : ~ OperationGUI_ClippingDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
OperationGUI_ClippingDlg::~ OperationGUI_ClippingDlg()
{
  // no need to delete child widgets, Qt does it all for us
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void OperationGUI_ClippingDlg::Init()
{
  QAD_ViewFrame* aFrame = QAD_Application::getDesktop()->getActiveApp()->
    getActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame();
  
  VTKViewer_ViewFrame* aVTKFrame = dynamic_cast<VTKViewer_ViewFrame*>( aFrame );
  if ( aVTKFrame )
    { 
      TextLabelNear->setText( tr( "Near"  ) );
      TextLabelFar->setText( tr( "Far"  ) );

      TypeCB->hide();

      vtkRenderer* aRenderer = aVTKFrame->getRenderer();
      if(!aRenderer) return;
      
      vtkCamera* anActiveCamera = aRenderer->GetActiveCamera();
      if( anActiveCamera == NULL ){
	MESSAGE("Trying to reset clipping range of non-existant camera");
	return;
      }
      
      // Find the plane equation for the camera view plane
      double vn[3];
      anActiveCamera->GetViewPlaneNormal(vn);
      double  position[3];
      anActiveCamera->GetPosition(position);
      
      float bounds[6];
      aRenderer->ComputeVisiblePropBounds(bounds);
      
      double center[3];
      center[0] = (bounds[0] + bounds[1])/2.0;
      center[1] = (bounds[2] + bounds[3])/2.0;
      center[2] = (bounds[4] + bounds[5])/2.0;
      
      double width = sqrt((bounds[1]-bounds[0])*(bounds[1]-bounds[0]) +
			  (bounds[3]-bounds[2])*(bounds[3]-bounds[2]) +
			  (bounds[5]-bounds[4])*(bounds[5]-bounds[4]));
      
      double distance = sqrt((position[0]-center[0])*(position[0]-center[0]) +
			     (position[1]-center[1])*(position[1]-center[1]) +
			     (position[2]-center[2])*(position[2]-center[2]));
      
      float range[2] = {distance - width/2.0, distance + width/2.0};
      
      SpinBox_Near->SetValue(range[0]);
      SpinBox_Far->SetValue(range[1]);
      
      return;
    }
  
  OCCViewer_ViewFrame* aOCCFrame = dynamic_cast<OCCViewer_ViewFrame*>( aFrame );
  if ( aOCCFrame )
    {
      TextLabelNear->setText( tr( "Depth"  ) );
      TextLabelFar->setText( tr( "Thickness"  ) );

      Handle(V3d_View) view3d = ((OCCViewer_ViewPort3d*)aOCCFrame->getViewPort())->getView();

      double depth, thickness; 
      int ztype= view3d->ZClipping(depth, thickness);
      SpinBox_Near->SetValue(depth);
      SpinBox_Far->SetValue(thickness);
      TypeCB->setCurrentItem(ztype);

      //Handle(V3d_Plane) aPlane = new V3d_Plane( aOCCFrame->getViewer()->getViewer3d() ,0, 0, 1, -100);
      //aPlane->Display(view3d);
    }
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool OperationGUI_ClippingDlg::ClickOnApply()
{
  QAD_ViewFrame* aFrame = QAD_Application::getDesktop()->getActiveApp()->
    getActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame();
  
  VTKViewer_ViewFrame* aVTKFrame = dynamic_cast<VTKViewer_ViewFrame*>( aFrame );
  if ( aVTKFrame )
    {
      vtkRenderer* aRenderer = aVTKFrame->getRenderer();
      if(!aRenderer) return false;
      
      vtkCamera* anActiveCamera = aRenderer->GetActiveCamera();
      if( anActiveCamera == NULL ){
	MESSAGE("Trying to reset clipping range of non-existant camera");
	return false;
      }
      
      float range[2] = { SpinBox_Near->GetValue(), SpinBox_Far->GetValue() };
      if (range[0] < 0.0) range[0] = 0.0;
      anActiveCamera->SetClippingRange( range );
      
      return true;
    }

  OCCViewer_ViewFrame* aOCCFrame = dynamic_cast<OCCViewer_ViewFrame*>( aFrame );
  if ( aOCCFrame )
    {
      Handle(V3d_View) view3d = ((OCCViewer_ViewPort3d*)aOCCFrame->getViewPort())->getView();
      
      double depth = SpinBox_Near->GetValue();
      double thickness = SpinBox_Far->GetValue();
      int aType = TypeCB->currentItem();
      
      view3d->SetZClippingType(V3d_TypeOfZclipping(aType));
      view3d->SetZClippingDepth(depth);
      view3d->SetZClippingWidth(thickness);
    }

  return true;
}

//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void OperationGUI_ClippingDlg::ClickOnOk()
{
  ClickOnApply();
  ClickOnCancel();
}

	
//=================================================================================
// function : ClickOnCancel()
// purpose  :
//=================================================================================
void OperationGUI_ClippingDlg::ClickOnCancel()
{
  GEOMBase_Skeleton::ClickOnCancel();
}


//=================================================================================
// function : onActivate()
// purpose  :
//=================================================================================
void OperationGUI_ClippingDlg::onActivate()
{
  GEOMBase_Skeleton::ActivateThisDialog();
}


//=================================================================================
// function : onDeactivate()
// purpose  : public slot to deactivate if active
//=================================================================================
void OperationGUI_ClippingDlg::DeactivateActiveDialog()
{
  return;
}

//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void OperationGUI_ClippingDlg::enterEvent(QEvent* e)
{
  this->setEnabled(true);
  return;
}


//=================================================================================
// function : closeEvent
// purpose  :
//=================================================================================
void OperationGUI_ClippingDlg::closeEvent( QCloseEvent* e )
{
  QDialog::closeEvent( e );
}


//=================================================================================
// function : onReset()
// purpose  :
//=================================================================================
void OperationGUI_ClippingDlg::onReset()
{
  QAD_ViewFrame* aFrame = QAD_Application::getDesktop()->getActiveApp()->
    getActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame();
  
  VTKViewer_ViewFrame* aVTKFrame = dynamic_cast<VTKViewer_ViewFrame*>( aFrame );
  if ( aVTKFrame )
    {
      vtkRenderer* aRenderer = aVTKFrame->getRenderer();
      if(!aRenderer) return;
      
      vtkCamera* anActiveCamera = aRenderer->GetActiveCamera();
      if( anActiveCamera == NULL ){
	MESSAGE("Trying to reset clipping range of non-existant camera");
	return;
      }
      
      // Find the plane equation for the camera view plane
      double vn[3];
      anActiveCamera->GetViewPlaneNormal(vn);
      double  position[3];
      anActiveCamera->GetPosition(position);
      
      float bounds[6];
      aRenderer->ComputeVisiblePropBounds(bounds);
      
      double center[3];
      center[0] = (bounds[0] + bounds[1])/2.0;
      center[1] = (bounds[2] + bounds[3])/2.0;
      center[2] = (bounds[4] + bounds[5])/2.0;
      
      double width = sqrt((bounds[1]-bounds[0])*(bounds[1]-bounds[0]) +
			  (bounds[3]-bounds[2])*(bounds[3]-bounds[2]) +
			  (bounds[5]-bounds[4])*(bounds[5]-bounds[4]));
      
      double distance = sqrt((position[0]-center[0])*(position[0]-center[0]) +
			     (position[1]-center[1])*(position[1]-center[1]) +
			     (position[2]-center[2])*(position[2]-center[2]));
      
      float range[2] = {distance - width/2.0, distance + width/2.0};
      
      SpinBox_Near->SetValue(range[0]);
      SpinBox_Far->SetValue(range[1]);
      
      return;
    }

  OCCViewer_ViewFrame* aOCCFrame = dynamic_cast<OCCViewer_ViewFrame*>( aFrame );
  if ( aOCCFrame )
    {
      TextLabelNear->setText( tr( "Depth"  ) );
      TextLabelFar->setText( tr( "Thickness"  ) );
      
      Handle(V3d_View) view3d = ((OCCViewer_ViewPort3d*)aOCCFrame->getViewPort())->getView();
      
      view3d->SetZClippingType(V3d_TypeOfZclipping(0));
      view3d->ZFitAll();
      double depth, thickness; 
      int ztype= view3d->ZClipping(depth, thickness);
      SpinBox_Near->SetValue(0);
      SpinBox_Far->SetValue(1000);
      TypeCB->setCurrentItem(ztype);
    }



  return;
}