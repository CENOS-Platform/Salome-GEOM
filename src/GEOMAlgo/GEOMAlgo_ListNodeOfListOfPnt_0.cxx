// File generated by CPPExt (Transient)
//
//                     Copyright (C) 1991 - 2000 by  
//                      Matra Datavision SA.  All rights reserved.
//  
//                     Copyright (C) 2001 - 2004 by
//                     Open CASCADE SA.  All rights reserved.
// 
// This file is part of the Open CASCADE Technology software.
//
// This software may be distributed and/or modified under the terms and
// conditions of the Open CASCADE Public License as defined by Open CASCADE SA
// and appearing in the file LICENSE included in the packaging of this file.
//  
// This software is distributed on an "AS IS" basis, without warranty of any
// kind, and Open CASCADE SA hereby disclaims all such warranties,
// including without limitation, any warranties of merchantability, fitness
// for a particular purpose or non-infringement. Please see the License for
// the specific terms and conditions governing rights and limitations under the
// License.

#include <GEOMAlgo_ListNodeOfListOfPnt.hxx>

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

#ifndef _gp_Pnt_HeaderFile
#include <gp_Pnt.hxx>
#endif
#ifndef _GEOMAlgo_ListOfPnt_HeaderFile
#include <GEOMAlgo_ListOfPnt.hxx>
#endif
#ifndef _GEOMAlgo_ListIteratorOfListOfPnt_HeaderFile
#include <GEOMAlgo_ListIteratorOfListOfPnt.hxx>
#endif
GEOMAlgo_ListNodeOfListOfPnt::~GEOMAlgo_ListNodeOfListOfPnt() {}
 


Standard_EXPORT Handle_Standard_Type& GEOMAlgo_ListNodeOfListOfPnt_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(TCollection_MapNode);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TCollection_MapNode);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("GEOMAlgo_ListNodeOfListOfPnt",
			                                 sizeof(GEOMAlgo_ListNodeOfListOfPnt),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(GEOMAlgo_ListNodeOfListOfPnt) Handle(GEOMAlgo_ListNodeOfListOfPnt)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(GEOMAlgo_ListNodeOfListOfPnt) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(GEOMAlgo_ListNodeOfListOfPnt))) {
       _anOtherObject = Handle(GEOMAlgo_ListNodeOfListOfPnt)((Handle(GEOMAlgo_ListNodeOfListOfPnt)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& GEOMAlgo_ListNodeOfListOfPnt::DynamicType() const 
{ 
  return STANDARD_TYPE(GEOMAlgo_ListNodeOfListOfPnt) ; 
}
Standard_Boolean GEOMAlgo_ListNodeOfListOfPnt::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(GEOMAlgo_ListNodeOfListOfPnt) == AType || TCollection_MapNode::IsKind(AType)); 
}
Handle_GEOMAlgo_ListNodeOfListOfPnt::~Handle_GEOMAlgo_ListNodeOfListOfPnt() {}
#define Item gp_Pnt
#define Item_hxx <gp_Pnt.hxx>
#define TCollection_ListNode GEOMAlgo_ListNodeOfListOfPnt
#define TCollection_ListNode_hxx <GEOMAlgo_ListNodeOfListOfPnt.hxx>
#define TCollection_ListIterator GEOMAlgo_ListIteratorOfListOfPnt
#define TCollection_ListIterator_hxx <GEOMAlgo_ListIteratorOfListOfPnt.hxx>
#define Handle_TCollection_ListNode Handle_GEOMAlgo_ListNodeOfListOfPnt
#define TCollection_ListNode_Type_() GEOMAlgo_ListNodeOfListOfPnt_Type_()
#define TCollection_List GEOMAlgo_ListOfPnt
#define TCollection_List_hxx <GEOMAlgo_ListOfPnt.hxx>
#include <TCollection_ListNode.gxx>
