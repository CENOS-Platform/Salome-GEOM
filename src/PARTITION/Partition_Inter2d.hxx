// File generated by CPPExt (Value)
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

#ifndef _Partition_Inter2d_HeaderFile
#define _Partition_Inter2d_HeaderFile

#ifndef _Handle_BRepAlgo_AsDes_HeaderFile
#include <Handle_BRepAlgo_AsDes.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class BRepAlgo_AsDes;
class TopoDS_Face;
class TopTools_MapOfShape;
class TopoDS_Vertex;
class TopTools_ListOfShape;
class TopoDS_Edge;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

class Partition_Inter2d  {

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
Standard_EXPORT static  void CompletPart2d(const Handle(BRepAlgo_AsDes)& AsDes,const TopoDS_Face& F,const TopTools_MapOfShape& NewEdges) ;
Standard_EXPORT static  TopoDS_Vertex FindEndVertex(const TopTools_ListOfShape& VertList,const Standard_Real f,const Standard_Real l,const TopoDS_Edge& E,Standard_Boolean& First,Standard_Real& DU) ;
Standard_EXPORT static  TopoDS_Vertex AddVonE(const TopoDS_Vertex& V,const TopoDS_Edge& E1,const TopoDS_Edge& E2,const Handle(BRepAlgo_AsDes)& AsDes) ;





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


};





// other Inline functions and methods (like "C++: function call" methods)
//


#endif