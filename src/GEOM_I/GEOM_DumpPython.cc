using namespace std;

#include "GEOM_Gen_i.hh"
#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TColStd_HSequenceOfAsciiString.hxx>
#include <Resource_DataMapOfAsciiStringAsciiString.hxx>

//=======================================================================
//function : DumpPython
//purpose  : 
//=======================================================================

Engines::TMPFile* GEOM_Gen_i::DumpPython(CORBA::Object_ptr theStudy, 
					 CORBA::Boolean isPublished, 
					 CORBA::Boolean& isValidScript)
{
  SALOMEDS::Study_var aStudy = SALOMEDS::Study::_narrow(theStudy);
  if(CORBA::is_nil(aStudy))
    return new Engines::TMPFile(0);   

  SALOMEDS::SObject_var aSO = aStudy->FindComponent(ComponentDataType());
  if(CORBA::is_nil(aSO))
    return new Engines::TMPFile(0);  

  Resource_DataMapOfAsciiStringAsciiString aMap;

  SALOMEDS::ChildIterator_var Itr = aStudy->NewChildIterator(aSO);
  for(Itr->InitEx(true); Itr->More(); Itr->Next()) {
    SALOMEDS::SObject_var aValue = Itr->Value();
    CORBA::String_var IOR = aValue->GetIOR();
    if(strlen(IOR.in()) > 0) {
      CORBA::Object_var obj = _orb->string_to_object(IOR);
      GEOM::GEOM_Object_var GO = GEOM::GEOM_Object::_narrow(obj);
      if(!CORBA::is_nil(GO)) {
        CORBA::String_var aName = aValue->GetName();
        CORBA::String_var anEntry = GO->GetEntry();
	aMap.Bind( (char*)anEntry.in(), (char*)aName.in() );
      }
    }
  }

  TCollection_AsciiString aScript =
    "### This file is generated by SALOME automatically by dump python funcitonality\n"
      "### of GEOM component\n\n";
  aScript += _impl->DumpPython(aStudy->StudyId(), aMap, isPublished, isValidScript);

  int aLen = aScript.Length(); 
  unsigned char* aBuffer = new unsigned char[aLen+1];
  strcpy((char*)aBuffer, aScript.ToCString());

  CORBA::Octet* anOctetBuf =  (CORBA::Octet*)aBuffer;
  Engines::TMPFile_var aStreamFile = new Engines::TMPFile(aLen+1, aLen+1, anOctetBuf, 1); 

  return aStreamFile._retn(); 
}

//=======================================================================
//function : GetDumpName
//purpose  : 
//=======================================================================

char* GEOM_Gen_i::GetDumpName (const char* theStudyEntry)
{
  const char* name = _impl->GetDumpName( theStudyEntry );
  if ( name && strlen( name ) > 0 )
    return strdup( name );

  return NULL;
}

//=======================================================================
//function : GetAllDumpNames
//purpose  : 
//=======================================================================

GEOM::string_array* GEOM_Gen_i::GetAllDumpNames()
{
  Handle(TColStd_HSequenceOfAsciiString) aHSeq = _impl->GetAllDumpNames();
  int i = 0, aLen = aHSeq->Length();

  GEOM::string_array_var seq = new GEOM::string_array();
  seq->length(aLen);

  for (; i < aLen; i++) {
    seq[i] = CORBA::string_dup(aHSeq->Value(i + 1).ToCString());
  }

  return seq._retn();
}