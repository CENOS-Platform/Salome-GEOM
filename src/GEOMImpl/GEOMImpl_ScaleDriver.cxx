
using namespace std;
#include "GEOMImpl_ScaleDriver.hxx"
#include "GEOMImpl_IScale.hxx"
#include "GEOMImpl_Types.hxx"
#include "GEOM_Function.hxx"

#include <BRepBuilderAPI_Transform.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopAbs.hxx>
#include <TopExp.hxx>

#include <Precision.hxx>
#include <gp_Pnt.hxx>

//=======================================================================
//function : GetID
//purpose  :
//======================================================================= 
const Standard_GUID& GEOMImpl_ScaleDriver::GetID()
{
  static Standard_GUID aScaleDriver("FF1BBB52-5D14-4df2-980B-3A668264EA16");
  return aScaleDriver; 
}


//=======================================================================
//function : GEOMImpl_ScaleDriver
//purpose  : 
//=======================================================================
GEOMImpl_ScaleDriver::GEOMImpl_ScaleDriver() 
{
}

//=======================================================================
//function : Execute
//purpose  :
//======================================================================= 
Standard_Integer GEOMImpl_ScaleDriver::Execute(TFunction_Logbook& log) const
{
  if (Label().IsNull()) return 0;    
  Handle(GEOM_Function) aFunction = GEOM_Function::GetFunction(Label());

  GEOMImpl_IScale aCI (aFunction);
  Standard_Integer aType = aFunction->GetType();

  TopoDS_Shape aShape;

  if (aType == SCALE_SHAPE || aType == SCALE_SHAPE_COPY) {
    Handle(GEOM_Function) aRefShape = aCI.GetShape();
    Handle(GEOM_Function) aRefPoint = aCI.GetPoint();
    TopoDS_Shape aShapeBase = aRefShape->GetValue();
    TopoDS_Shape aShapePnt  = aRefPoint->GetValue();
    if (aShapeBase.IsNull() || aShapePnt.IsNull()) return 0;
    if (aShapePnt.ShapeType() != TopAbs_VERTEX) return 0;

    gp_Pnt aP = BRep_Tool::Pnt(TopoDS::Vertex(aShapePnt));
    gp_Trsf aTrsf;
    aTrsf.SetScale(aP, aCI.GetFactor());
    BRepBuilderAPI_Transform aBRepTrsf (aShapeBase, aTrsf, Standard_False);
    aShape = aBRepTrsf.Shape();
  } else {
  }

  if (aShape.IsNull()) return 0;

  aFunction->SetValue(aShape);

  log.SetTouched(Label()); 

  return 1;    
}


//=======================================================================
//function :  GEOMImpl_ScaleDriver_Type_
//purpose  :
//======================================================================= 
Standard_EXPORT Handle_Standard_Type& GEOMImpl_ScaleDriver_Type_()
{

  static Handle_Standard_Type aType1 = STANDARD_TYPE(TFunction_Driver);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TFunction_Driver);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared); 
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("GEOMImpl_ScaleDriver",
			                                 sizeof(GEOMImpl_ScaleDriver),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}

//=======================================================================
//function : DownCast
//purpose  :
//======================================================================= 
const Handle(GEOMImpl_ScaleDriver) Handle(GEOMImpl_ScaleDriver)::DownCast(const Handle(Standard_Transient)& AnObject)
{
  Handle(GEOMImpl_ScaleDriver) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(GEOMImpl_ScaleDriver))) {
       _anOtherObject = Handle(GEOMImpl_ScaleDriver)((Handle(GEOMImpl_ScaleDriver)&)AnObject);
     }
  }

  return _anOtherObject ;
}