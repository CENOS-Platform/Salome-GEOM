
using namespace std;
#include "GEOMImpl_ExportDriver.hxx"
#include "GEOMImpl_IImportExport.hxx"
#include "GEOMImpl_Types.hxx"
#include "GEOM_Function.hxx"

#include <TopoDS_Shape.hxx>
#include <TCollection_AsciiString.hxx>

#include <Standard_ConstructionError.hxx>

#ifdef WNT
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#ifdef WNT
#define LibHandle HMODULE
#define LoadLib( name ) LoadLibrary( name )
#define GetProc GetProcAddress
#define UnLoadLib( handle ) FreeLibrary( handle );
#else
#define LibHandle void*
#define LoadLib( name ) dlopen( name, RTLD_LAZY )
#define GetProc dlsym
#define UnLoadLib( handle ) dlclose( handle );
#endif

typedef int (*funcPoint)(const TopoDS_Shape&, const TCollection_AsciiString&);

//=======================================================================
//function : GetID
//purpose  :
//======================================================================= 
const Standard_GUID& GEOMImpl_ExportDriver::GetID()
{
  static Standard_GUID aExportDriver("FF1BBB58-5D14-4df2-980B-3A668264EA16");
  return aExportDriver; 
}


//=======================================================================
//function : GEOMImpl_ExportDriver
//purpose  : 
//=======================================================================
GEOMImpl_ExportDriver::GEOMImpl_ExportDriver() 
{
}

//=======================================================================
//function : Execute
//purpose  :
//======================================================================= 
Standard_Integer GEOMImpl_ExportDriver::Execute(TFunction_Logbook& log) const
{
  if (Label().IsNull()) return 0;    
  Handle(GEOM_Function) aFunction = GEOM_Function::GetFunction(Label());

  GEOMImpl_IImportExport aCI (aFunction);

  // retrieve the being exported shape
  TopoDS_Shape aShape;
  Handle(GEOM_Function) aRefFunction = aCI.GetOriginal();
  if (aRefFunction.IsNull()) return 0;
  aShape = aRefFunction->GetValue();
  if (aShape.IsNull()) return 0;
  // !!! set the result of function to be used by next operations
  aFunction->SetValue(aShape);

  // retrieve the file and format names
  TCollection_AsciiString aFileName = aCI.GetFileName();
  TCollection_AsciiString aLibName  = aCI.GetPluginName();
  if (aFileName.IsEmpty() || aLibName.IsEmpty())
    return 0;

  // load plugin library
  LibHandle anExportLib = LoadLib( aLibName.ToCString() );
  funcPoint fp = 0;
  if ( anExportLib )
    fp = (funcPoint)GetProc( anExportLib, "Export" );

  if ( !fp )
    return 0;

  // perform the export
  int res = fp( aShape, aFileName );

  // unload plugin library
  UnLoadLib( anExportLib );
  if ( res )
    log.SetTouched(Label()); 

  return res;
}


//=======================================================================
//function :  GEOMImpl_ExportDriver_Type_
//purpose  :
//======================================================================= 
Standard_EXPORT Handle_Standard_Type& GEOMImpl_ExportDriver_Type_()
{

  static Handle_Standard_Type aType1 = STANDARD_TYPE(TFunction_Driver);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TFunction_Driver);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared); 
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("GEOMImpl_ExportDriver",
			                                 sizeof(GEOMImpl_ExportDriver),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}

//=======================================================================
//function : DownCast
//purpose  :
//======================================================================= 
const Handle(GEOMImpl_ExportDriver) Handle(GEOMImpl_ExportDriver)::DownCast(const Handle(Standard_Transient)& AnObject)
{
  Handle(GEOMImpl_ExportDriver) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(GEOMImpl_ExportDriver))) {
       _anOtherObject = Handle(GEOMImpl_ExportDriver)((Handle(GEOMImpl_ExportDriver)&)AnObject);
     }
  }

  return _anOtherObject ;
}