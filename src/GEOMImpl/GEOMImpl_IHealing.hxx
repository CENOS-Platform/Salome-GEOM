//NOTE: This is an intreface to a function for the Healing creation.


#include "GEOM_Function.hxx"

#include <TColStd_HArray1OfInteger.hxx>
#include <TColStd_HArray1OfExtendedString.hxx>

#define ARG_SHAPE_PROCESS_OPERATORS   1
#define ARG_SHAPE_PROCESS_PARAMS      2
#define ARG_SHAPE_PROCESS_VALUES      3
#define ARG_ORIGINAL                  4
#define ARG_LIST_ARGUMENTS            5
#define ARG_IS_COMMON_VERTEX          6
#define ARG_TOLERANCE                 7
#define ARG_DEV_EDGE_VALUE            8
#define ARG_IS_BY_PARAMETER           9
#define ARG_SUBSHAPE_INDEX           10


class GEOMImpl_IHealing
{
public:
  GEOMImpl_IHealing(Handle(GEOM_Function) theFunction): _func(theFunction) {}

  void SetOperators( const Handle(TColStd_HArray1OfExtendedString)& arr ) {  if ( !arr.IsNull() ) _func->SetStringArray(ARG_SHAPE_PROCESS_OPERATORS, arr); }
  Handle(TColStd_HArray1OfExtendedString) GetOperators() { return _func->GetStringArray(ARG_SHAPE_PROCESS_OPERATORS); }

  void SetParameters( const Handle(TColStd_HArray1OfExtendedString)& arr ) {  if ( !arr.IsNull() ) _func->SetStringArray(ARG_SHAPE_PROCESS_PARAMS, arr); }
  Handle(TColStd_HArray1OfExtendedString) GetParameters() { return _func->GetStringArray(ARG_SHAPE_PROCESS_PARAMS); }

  void SetValues( const Handle(TColStd_HArray1OfExtendedString)& arr ) {  if ( !arr.IsNull() ) _func->SetStringArray(ARG_SHAPE_PROCESS_VALUES, arr); }
  Handle(TColStd_HArray1OfExtendedString) GetValues() { return _func->GetStringArray(ARG_SHAPE_PROCESS_VALUES); }

  void SetOriginal( Handle(GEOM_Function)& f ) { _func->SetReference(ARG_ORIGINAL, f); }
  Handle(GEOM_Function) GetOriginal() { return _func->GetReference(ARG_ORIGINAL); }

  void SetFaces( const Handle(TColStd_HArray1OfInteger)& arr ) { if ( !arr.IsNull() ) _func->SetIntegerArray(ARG_LIST_ARGUMENTS, arr); }
  Handle(TColStd_HArray1OfInteger) GetFaces() { return _func->GetIntegerArray(ARG_LIST_ARGUMENTS); }

  void SetWires( const Handle(TColStd_HArray1OfInteger)& arr ) { if ( !arr.IsNull() ) _func->SetIntegerArray(ARG_LIST_ARGUMENTS, arr); }
  Handle(TColStd_HArray1OfInteger) GetWires() { return _func->GetIntegerArray(ARG_LIST_ARGUMENTS); }

  void SetIsCommonVertex( Standard_Boolean val ) { _func->SetInteger(ARG_IS_COMMON_VERTEX, val ? 1 : 0 ); }
  Standard_Boolean GetIsCommonVertex() { return (_func->GetInteger(ARG_IS_COMMON_VERTEX) != 0 ); }

  void SetIsByParameter( Standard_Boolean val ) { _func->SetInteger(ARG_IS_BY_PARAMETER, val ? 1 : 0 ); }
  Standard_Boolean GetIsByParameter() { return (_func->GetInteger(ARG_IS_BY_PARAMETER) != 0 ); }

  void SetTolerance( Standard_Real val ) { _func->SetReal(ARG_TOLERANCE, val); }
  Standard_Real GetTolerance() { return _func->GetReal(ARG_TOLERANCE); }

  void SetDevideEdgeValue( Standard_Real val ) { _func->SetReal(ARG_DEV_EDGE_VALUE, val); }
  Standard_Real GetDevideEdgeValue() { return _func->GetReal(ARG_DEV_EDGE_VALUE); }

  void SetIndex( Standard_Integer val ) { _func->SetInteger(ARG_SUBSHAPE_INDEX, val); }
  Standard_Integer GetIndex() { return _func->GetInteger(ARG_SUBSHAPE_INDEX); }
            
private:
  Handle(GEOM_Function) _func;  
};