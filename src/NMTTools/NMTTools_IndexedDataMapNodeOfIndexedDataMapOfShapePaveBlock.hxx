// File generated by CPPExt (Transient)
//
//                     Copyright (C) 1991,1995 by
//  
//                      MATRA DATAVISION, FRANCE
//  
// This software is furnished in accordance with the terms and conditions
// of the contract and with the inclusion of the above copyright notice.
// This software or any other copy thereof may not be provided or otherwise
// be made available to any other person. No title to an ownership of the
// software is hereby transferred.
//  
// At the termination of the contract, the software and all copies of this
// software must be deleted.
//
#ifndef _NMTTools_IndexedDataMapNodeOfIndexedDataMapOfShapePaveBlock_HeaderFile
#define _NMTTools_IndexedDataMapNodeOfIndexedDataMapOfShapePaveBlock_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_NMTTools_IndexedDataMapNodeOfIndexedDataMapOfShapePaveBlock_HeaderFile
#include <Handle_NMTTools_IndexedDataMapNodeOfIndexedDataMapOfShapePaveBlock.hxx>
#endif

#ifndef _TopoDS_Shape_HeaderFile
#include <TopoDS_Shape.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _BOPTools_PaveBlock_HeaderFile
#include <BOPTools_PaveBlock.hxx>
#endif
#ifndef _TCollection_MapNodePtr_HeaderFile
#include <TCollection_MapNodePtr.hxx>
#endif
#ifndef _TCollection_MapNode_HeaderFile
#include <TCollection_MapNode.hxx>
#endif
class TopoDS_Shape;
class BOPTools_PaveBlock;
class TopTools_ShapeMapHasher;
class NMTTools_IndexedDataMapOfShapePaveBlock;


class NMTTools_IndexedDataMapNodeOfIndexedDataMapOfShapePaveBlock : public TCollection_MapNode {

public:

    void* operator new(size_t,void* anAddress) 
      {
        return anAddress;
      }
    void* operator new(size_t size) 
      { 
        return Standard::Allocate(size); 
      }
    void  operator delete(void *anAddress) 
      { 
        if (anAddress) Standard::Free((Standard_Address&)anAddress); 
      }
 // Methods PUBLIC
 // 
NMTTools_IndexedDataMapNodeOfIndexedDataMapOfShapePaveBlock(const TopoDS_Shape& K1,const Standard_Integer K2,const BOPTools_PaveBlock& I,const TCollection_MapNodePtr& n1,const TCollection_MapNodePtr& n2);
  TopoDS_Shape& Key1() const;
  Standard_Integer& Key2() const;
  TCollection_MapNodePtr& Next2() const;
  BOPTools_PaveBlock& Value() const;
Standard_EXPORT ~NMTTools_IndexedDataMapNodeOfIndexedDataMapOfShapePaveBlock();




 // Type management
 //
 Standard_EXPORT friend Handle_Standard_Type& NMTTools_IndexedDataMapNodeOfIndexedDataMapOfShapePaveBlock_Type_();
 Standard_EXPORT const Handle(Standard_Type)& DynamicType() const;
 Standard_EXPORT Standard_Boolean	       IsKind(const Handle(Standard_Type)&) const;

protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 


 // Fields PRIVATE
 //
TopoDS_Shape myKey1;
Standard_Integer myKey2;
BOPTools_PaveBlock myValue;
TCollection_MapNodePtr myNext2;


};

#define TheKey TopoDS_Shape
#define TheKey_hxx <TopoDS_Shape.hxx>
#define TheItem BOPTools_PaveBlock
#define TheItem_hxx <BOPTools_PaveBlock.hxx>
#define Hasher TopTools_ShapeMapHasher
#define Hasher_hxx <TopTools_ShapeMapHasher.hxx>
#define TCollection_IndexedDataMapNode NMTTools_IndexedDataMapNodeOfIndexedDataMapOfShapePaveBlock
#define TCollection_IndexedDataMapNode_hxx <NMTTools_IndexedDataMapNodeOfIndexedDataMapOfShapePaveBlock.hxx>
#define Handle_TCollection_IndexedDataMapNode Handle_NMTTools_IndexedDataMapNodeOfIndexedDataMapOfShapePaveBlock
#define TCollection_IndexedDataMapNode_Type_() NMTTools_IndexedDataMapNodeOfIndexedDataMapOfShapePaveBlock_Type_()
#define TCollection_IndexedDataMap NMTTools_IndexedDataMapOfShapePaveBlock
#define TCollection_IndexedDataMap_hxx <NMTTools_IndexedDataMapOfShapePaveBlock.hxx>

#include <TCollection_IndexedDataMapNode.lxx>

#undef TheKey
#undef TheKey_hxx
#undef TheItem
#undef TheItem_hxx
#undef Hasher
#undef Hasher_hxx
#undef TCollection_IndexedDataMapNode
#undef TCollection_IndexedDataMapNode_hxx
#undef Handle_TCollection_IndexedDataMapNode
#undef TCollection_IndexedDataMapNode_Type_
#undef TCollection_IndexedDataMap
#undef TCollection_IndexedDataMap_hxx


// other Inline functions and methods (like "C++: function call" methods)
//


#endif