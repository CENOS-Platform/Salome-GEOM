using namespace std;

#include "GEOMImpl_IHealingOperations.hxx"

#include "GEOMImpl_HealingDriver.hxx"
#include "GEOMImpl_Types.hxx"
#include "GEOMImpl_IHealing.hxx"
#include "GEOMImpl_CopyDriver.hxx"

#include "ShHealOper_ShapeProcess.hxx"

#include "utilities.h"
#include "OpUtil.hxx"
#include "Utils_ExceptHandlers.hxx"

#include <ShapeAnalysis_FreeBounds.hxx>

#include <TopoDS_Compound.hxx>
#include <TopExp_Explorer.hxx>

#include <TColStd_HArray1OfExtendedString.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#include <TCollection_AsciiString.hxx>

#include <Standard_ErrorHandler.hxx> // CAREFUL ! position of this file is critic : see Lucien PIGNOLONI / OCC


//=============================================================================
/*!
 *   constructor:
 */
//=============================================================================

GEOMImpl_IHealingOperations::GEOMImpl_IHealingOperations (GEOM_Engine* theEngine, int theDocID)
: GEOM_IOperations(theEngine, theDocID)
{
  MESSAGE("GEOMImpl_IHealingOperations::GEOMImpl_IHealingOperations");
}

//=============================================================================
/*!
 *  destructor
 */
//=============================================================================

GEOMImpl_IHealingOperations::~GEOMImpl_IHealingOperations()
{
  MESSAGE("GEOMImpl_IHealingOperations::~GEOMImpl_IHealingOperations");
}


//=============================================================================
/*!
 *  ShapeProcess
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IHealingOperations::ShapeProcess (Handle(GEOM_Object) theObject,
                                  const Handle(TColStd_HArray1OfExtendedString)& theOperators,
                                  const Handle(TColStd_HArray1OfExtendedString)& theParams,
                                  const Handle(TColStd_HArray1OfExtendedString)& theValues)
{
  // set error code, check parameters
  SetErrorCode(KO);

  if (theObject.IsNull())
    return NULL;

  if (theOperators.IsNull() || theOperators->Length() <= 0) {
    SetErrorCode("No operators requested");
    return NULL;
  }

  Standard_Integer nbParams = 0, nbValues = 0;
  if (!theParams.IsNull()) {
    nbParams = theParams->Length();
  }
  if (!theValues.IsNull()) {
    nbValues = theValues->Length();
  }

  if (nbParams != nbValues) {
    SetErrorCode("Number of parameter values must be equal to the number of parameters");
    return NULL;
  }

  Handle(GEOM_Function) aFunction, aLastFunction = theObject->GetLastFunction();
  if (aLastFunction.IsNull()) return NULL; //There is no function which creates an object to be processed

  // Add a new object
  Handle(GEOM_Object) aNewObject = GetEngine()->AddObject( GetDocID(), GEOM_COPY );

  //Add the function
  aFunction = aNewObject->AddFunction(GEOMImpl_HealingDriver::GetID(), SHAPE_PROCESS);

  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_HealingDriver::GetID()) return NULL;

  // prepare "data container" class IHealing
  GEOMImpl_IHealing HI(aFunction);
  HI.SetOriginal(aLastFunction);
  HI.SetOperators( theOperators );
  if (nbParams > 0) {
    HI.SetParameters( theParams );
    HI.SetValues( theValues );
  }

  //Compute the translation
  try
  {
    if (!GetSolver()->ComputeFunction(aFunction))
    {
      SetErrorCode("Shape Healing algorithm failed");
      return NULL;
    }
  }
  catch (Standard_Failure)
  {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  // ...
  // ... missing ...

  SetErrorCode(OK);
  return aNewObject;
}

//=============================================================================
/*!
 *  ShapeProcess
 */
//=============================================================================
void GEOMImpl_IHealingOperations::GetShapeProcessParameters (list<string>& theOperations,
                                                             list<string>& theParams,
                                                             list<string>& theValues)
{
  ShHealOper_ShapeProcess aHealer;
  TColStd_SequenceOfAsciiString anOperators;
  int nbOperatorErrors( 0 ), nbParamValueErrors( 0 );
  if ( aHealer.GetOperators( anOperators ) )
  {
    for ( Standard_Integer i = 1; i <= anOperators.Length(); i++ )
    {
      string anOperation = anOperators.Value( i ).ToCString();
      theOperations.push_back( anOperation );
      list<string> aParams, aValues;
      if ( GetParameters( anOperation, aParams ) )
      {
        for ( list<string>::iterator it = aParams.begin(); it != aParams.end(); ++it )
        {
          TCollection_AsciiString aParam( (Standard_CString)(*it).c_str() );
          TCollection_AsciiString aValue;
          if ( aHealer.GetParameter( aParam, aValue ) )
          {
            theParams.push_back( aParam.ToCString() );
            theValues.push_back( aValue.ToCString() );
          }
	  else
            nbParamValueErrors++;
        }
      }
      else
        nbOperatorErrors++;
    }
  }
  else
  {
    SetErrorCode("ERROR retrieving operators (GEOMImpl_IHealingOperations)");
  }

  if (nbOperatorErrors || nbParamValueErrors) {
    TCollection_AsciiString aMsg ("ERRORS retrieving ShapeProcess parameters (GEOMImpl_IHealingOperations): nbOperatorErrors = ");
    aMsg += TCollection_AsciiString(nbOperatorErrors);
    aMsg += " ,nbParamValueErrors = ";
    aMsg += TCollection_AsciiString(nbParamValueErrors);
    MESSAGE(aMsg.ToCString());
  }
}

//=============================================================================
/*!
 *  GetParameters
 */
//=============================================================================
bool GEOMImpl_IHealingOperations::GetParameters (const string theOperation,
                                                 list<string>& theParams)
{
  if ( theOperation == "SplitAngle" ) {
    theParams.push_back( "SplitAngle.Angle" );
    theParams.push_back( "SplitAngle.MaxTolerance" );

  } else if ( theOperation == "SplitClosedFaces" ) {
    theParams.push_back( "SplitClosedFaces.NbSplitPoints" );

  } else if ( theOperation == "FixFaceSize" ) {
    theParams.push_back( "FixFaceSize.Tolerance" );

  } else if( theOperation == "DropSmallEdges" ) {
    theParams.push_back( "DropSmallEdges.Tolerance3d" );

  } else if( theOperation == "BSplineRestriction" ) {
    theParams.push_back( "BSplineRestriction.SurfaceMode" );
    theParams.push_back( "BSplineRestriction.Curve3dMode" );
    theParams.push_back( "BSplineRestriction.Curve2dMode" );
    theParams.push_back( "BSplineRestriction.Tolerance3d" );
    theParams.push_back( "BSplineRestriction.Tolerance2d" );
    theParams.push_back( "BSplineRestriction.RequiredDegree" );
    theParams.push_back( "BSplineRestriction.RequiredNbSegments" );
    theParams.push_back( "BSplineRestriction.Continuity3d" );
    theParams.push_back( "BSplineRestriction.Continuity2d" );

  } else if( theOperation == "SplitContinuity" ) {
    theParams.push_back( "SplitContinuity.Tolerance3d" );
    theParams.push_back( "SplitContinuity.SurfaceContinuity" );
    theParams.push_back( "SplitContinuity.CurveContinuity" );

  } else if( theOperation == "ToBezier" ) {
    theParams.push_back( "ToBezier.SurfaceMode" );
    theParams.push_back( "ToBezier.Curve3dMode" );
    theParams.push_back( "ToBezier.Curve2dMode" );
    theParams.push_back( "ToBezier.MaxTolerance" );

  } else if( theOperation == "SameParameter" ) {
    theParams.push_back( "SameParameter.Tolerance3d" );

  } else if( theOperation == "FixShape" ) {
    theParams.push_back( "FixShape.Tolerance3d" );
    theParams.push_back( "FixShape.MaxTolerance3d" );

  } else {
    return false;
  }

  return true;
}

//=============================================================================
/*!
 *  SuppressFaces
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IHealingOperations::SuppressFaces (Handle(GEOM_Object) theObject,
                                                                const Handle(TColStd_HArray1OfInteger)& theFaces)
{
  // set error code, check parameters
  SetErrorCode(KO);

  if ( theObject.IsNull() ) // if theFaces.IsNull() - it's OK, it means that ALL faces must be removed..
    return NULL;

  Handle(GEOM_Function) aFunction, aLastFunction = theObject->GetLastFunction();
  if(aLastFunction.IsNull()) return NULL; //There is no function which creates an object to be processed

  // Add a new object
  Handle(GEOM_Object) aNewObject = GetEngine()->AddObject( GetDocID(), GEOM_COPY );

  //Add the function
  aFunction = aNewObject->AddFunction(GEOMImpl_HealingDriver::GetID(), SUPPRESS_FACES);

  if(aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if(aFunction->GetDriverGUID() != GEOMImpl_HealingDriver::GetID()) return NULL;

  // prepare "data container" class IHealing
  GEOMImpl_IHealing HI(aFunction);
  HI.SetFaces( theFaces );
  HI.SetOriginal( aLastFunction );

  //Compute the translation
  try
  {
    if (!GetSolver()->ComputeFunction(aFunction))
    {
      SetErrorCode("Healing driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure)
  {
  	Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  // ...
  // ... missing ...

  SetErrorCode(OK);
  return aNewObject;
}


//=============================================================================
/*!
 *  CloseContour
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IHealingOperations::CloseContour (Handle(GEOM_Object) theObject,
							       const Handle(TColStd_HArray1OfInteger)& theWires,
                                                               bool isCommonVertex)
{
  // set error code, check parameters
  SetErrorCode(KO);

  if (theObject.IsNull())
    return NULL;

  Handle(GEOM_Function) aFunction, aLastFunction = theObject->GetLastFunction();
  if (aLastFunction.IsNull()) return NULL; //There is no function which creates an object to be processed

  // Add a new object
  Handle(GEOM_Object) aNewObject = GetEngine()->AddObject( GetDocID(), GEOM_COPY );

  //Add the function
  aFunction = aNewObject->AddFunction(GEOMImpl_HealingDriver::GetID(), CLOSE_CONTOUR);

  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if(aFunction->GetDriverGUID() != GEOMImpl_HealingDriver::GetID()) return NULL;

  // prepare "data container" class IHealing
  GEOMImpl_IHealing HI(aFunction);
  HI.SetWires( theWires );
  HI.SetIsCommonVertex( isCommonVertex );
  HI.SetOriginal( aLastFunction );

  //Compute the translation
  try
  {
    if (!GetSolver()->ComputeFunction(aFunction))
    {
      SetErrorCode("Healing driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure)
  {
  	Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  // ...
  // ... missing ...

  SetErrorCode(OK);
  return aNewObject;
}

//=============================================================================
/*!
 *  RemoveIntWires
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IHealingOperations::RemoveIntWires (Handle(GEOM_Object) theObject,
                                                                 const Handle(TColStd_HArray1OfInteger)& theWires)
{
  // set error code, check parameters
  SetErrorCode(KO);

  if (theObject.IsNull()) // if theWires is NULL it's OK, it means that ALL wires must be removed
    return NULL;

  Handle(GEOM_Function) aFunction, aLastFunction = theObject->GetLastFunction();
  if (aLastFunction.IsNull()) return NULL; //There is no function which creates an object to be processed

  // Add a new object
  Handle(GEOM_Object) aNewObject = GetEngine()->AddObject( GetDocID(), GEOM_COPY );

  //Add the function
  aFunction = aNewObject->AddFunction(GEOMImpl_HealingDriver::GetID(), REMOVE_INT_WIRES);

  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_HealingDriver::GetID()) return NULL;

  // prepare "data container" class IHealing
  GEOMImpl_IHealing HI(aFunction);
  HI.SetWires( theWires );
  HI.SetOriginal( aLastFunction );

  //Compute the translation
  try
  {
    if (!GetSolver()->ComputeFunction(aFunction))
    {
      SetErrorCode("Healing driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure)
  {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  // ...
  // ... missing ...

  SetErrorCode(OK);
  return aNewObject;
}

//=============================================================================
/*!
 *  FillHoles
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IHealingOperations::FillHoles (Handle(GEOM_Object) theObject,
                                                            const Handle(TColStd_HArray1OfInteger)& theWires)
{
  // set error code, check parameters
  SetErrorCode(KO);

  if (theObject.IsNull()) // if theWires is NULL it's OK, it means that ALL holes must be removed
    return NULL;

  Handle(GEOM_Function) aFunction, aLastFunction = theObject->GetLastFunction();
  if (aLastFunction.IsNull()) return NULL; //There is no function which creates an object to be processed

  // Add a new object
  Handle(GEOM_Object) aNewObject = GetEngine()->AddObject( GetDocID(), GEOM_COPY );

  //Add the function
  aFunction = aNewObject->AddFunction(GEOMImpl_HealingDriver::GetID(), FILL_HOLES);

  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_HealingDriver::GetID()) return NULL;

  // prepare "data container" class IHealing
  GEOMImpl_IHealing HI(aFunction);
  HI.SetWires( theWires );
  HI.SetOriginal( aLastFunction );

  //Compute the translation
  try
  {
    if (!GetSolver()->ComputeFunction(aFunction))
    {
      SetErrorCode("Healing driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure)
  {
  	Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  // ...
  // ... missing ...

  SetErrorCode(OK);
  return aNewObject;
}

//=============================================================================
/*!
 *  Sew
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IHealingOperations::Sew (Handle(GEOM_Object) theObject,
                                                      double theTolerance)
{
  // set error code, check parameters
  SetErrorCode(KO);

  if (theObject.IsNull())
    return NULL;

  Handle(GEOM_Function) aFunction, aLastFunction = theObject->GetLastFunction();
  if (aLastFunction.IsNull()) return NULL; //There is no function which creates an object to be processed

  // Add a new object
  Handle(GEOM_Object) aNewObject = GetEngine()->AddObject( GetDocID(), GEOM_COPY );

  //Add the function
  aFunction = aNewObject->AddFunction(GEOMImpl_HealingDriver::GetID(), SEWING);

  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_HealingDriver::GetID()) return NULL;

  // prepare "data container" class IHealing
  GEOMImpl_IHealing HI(aFunction);
  HI.SetTolerance( theTolerance );
  HI.SetOriginal( aLastFunction );

  //Compute the translation
  try
  {
    if (!GetSolver()->ComputeFunction(aFunction))
    {
      SetErrorCode("Healing driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure)
  {
  	Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  // ...
  // ... missing ...

  SetErrorCode(OK);
  return aNewObject;
}

//=============================================================================
/*!
 *  DivideEdge
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IHealingOperations::DivideEdge (Handle(GEOM_Object) theObject,
                                                             int theIndex,
                                                             double theValue,
                                                             bool isByParameter)
{
  // set error code, check parameters
  SetErrorCode(KO);

  if (theObject.IsNull())
    return NULL;

  Handle(GEOM_Function) aFunction, aLastFunction = theObject->GetLastFunction();
  if (aLastFunction.IsNull()) return NULL; //There is no function which creates an object to be processed

  // Add a new object
  Handle(GEOM_Object) aNewObject = GetEngine()->AddObject( GetDocID(), GEOM_COPY );

  //Add the function
  aFunction = aNewObject->AddFunction(GEOMImpl_HealingDriver::GetID(), DIVIDE_EDGE);

  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_HealingDriver::GetID()) return NULL;

  // prepare "data container" class IHealing
  GEOMImpl_IHealing HI(aFunction);
  HI.SetIndex( theIndex );
  HI.SetDevideEdgeValue( theValue );
  HI.SetIsByParameter( isByParameter );
  HI.SetOriginal( aLastFunction );

  //Compute the translation
  try
  {
    if (!GetSolver()->ComputeFunction(aFunction))
    {
      SetErrorCode("Healing driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure)
  {
  	Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  // ...
  // ... missing ...

  SetErrorCode(OK);
  return aNewObject;
}

//=============================================================================
/*!
 *  GetFreeBoundary
 */
//=============================================================================
bool GEOMImpl_IHealingOperations::GetFreeBoundary (Handle(GEOM_Object) theObject,
						   Handle(TColStd_HSequenceOfTransient)& theClosed,
						   Handle(TColStd_HSequenceOfTransient)& theOpen )
{
  // set error code, check parameters
  SetErrorCode(KO);

  if ( theObject.IsNull() || theClosed.IsNull() || theOpen.IsNull() )
    return false;

  TopoDS_Shape aShape = theObject->GetValue();
  if ( aShape.IsNull() )
    return false;

  // get free boundary shapes
  ShapeAnalysis_FreeBounds anAnalizer( aShape );
  TopoDS_Compound aClosed = anAnalizer.GetClosedWires();
  TopoDS_Compound anOpen = anAnalizer.GetOpenWires();

  // iterate through shapes and append them to the return sequence
  Handle(GEOM_Object) anObj;
  Handle(GEOM_Function) aFunction;
  TopExp_Explorer anExp;
  for ( anExp.Init( aClosed, TopAbs_WIRE ); anExp.More(); anExp.Next() )
  {
    anObj = GetEngine()->AddObject( GetDocID(), GEOM_FREE_BOUNDS );
    aFunction = anObj->AddFunction( GEOMImpl_CopyDriver::GetID(), COPY_WITHOUT_REF );
    TopoDS_Shape aValueShape = anExp.Current();
    aFunction->SetValue( aValueShape );
    theClosed->Append(anObj);
  }
  for ( anExp.Init( anOpen, TopAbs_WIRE ); anExp.More(); anExp.Next() )
  {
    anObj = GetEngine()->AddObject( GetDocID(), GEOM_FREE_BOUNDS );
    aFunction = anObj->AddFunction( GEOMImpl_CopyDriver::GetID(), COPY_WITHOUT_REF );
    TopoDS_Shape aValueShape = anExp.Current();
    aFunction->SetValue( aValueShape );
    theOpen->Append(anObj);
  }

  SetErrorCode(OK);
  return true;
}