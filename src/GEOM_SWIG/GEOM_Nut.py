#####################################################################
#Created                :17/02/2005
#Auhtor                 :MASLOV Eugeny, KOVALTCHUK Alexey 
#####################################################################

import geompy
import salome
import os
import math

#Sketcher_1 creation
Sketcher_1 = geompy.MakeSketcher("Sketcher:F 100 -57.7:TT 100 57.7:TT 0 115.47:TT -100 57.7:TT -100 -57.7:TT 0 -115.47:WW") 
geompy.addToStudy(Sketcher_1, "Sketcher_1")
Face_1 = geompy.MakeFace(Sketcher_1, 1)
geompy.addToStudy(Face_1, "Face_1")

#Line creation
Line_1 = geompy.MakeLineTwoPnt(geompy.MakeVertex(0,0,0), geompy.MakeVertex(0,0,100))
geompy.addToStudy(Line_1, "Line_1")

#Prism creation
Prism_1 = geompy.MakePrismVecH(Face_1, Line_1, 100)
geompy.addToStudy(Prism_1, "Prism_1")

#Sketcher_2 creation
Sketcher_2 = geompy.MakeSketcher("Sketcher:F 50 0:TT 80 0:TT 112 13:TT 112 48:TT 80 63:TT 80 90:TT 50 90:WW", [0,0,0, 1,0,0, 0,1,0]) 
geompy.addToStudy(Sketcher_2, "Sketcher_2")
Face_2 = geompy.MakeFace(Sketcher_2, 1)
geompy.addToStudy(Face_2, "Face_2")

#Revolution creation
Revolution_1 = geompy.MakeRevolution(Face_2, Line_1, 2*math.pi)
geompy.addToStudy(Revolution_1, "Revolution_1")

#Common applying
Common_1 = geompy.MakeBoolean(Revolution_1, Prism_1, 1)
geompy.addToStudy(Common_1, "Common_1")

#Explode Common_1 on edges
CommonExplodedListEdges = geompy.SubShapeAll(Common_1, geompy.ShapeType["EDGE"])
for i in range(0, len(CommonExplodedListEdges)):
    name = "Edge_"+str(i+1)
    geompy.addToStudyInFather(Common_1, CommonExplodedListEdges[i], name)

#Fillet applying
#ID = geompy.GetSubShapeID(Common_1, CommonExplodedListEdges[0])
Fillet_1 = geompy.MakeFillet(Common_1, 10, geompy.ShapeType["EDGE"], [6])
geompy.addToStudy(Fillet_1, "Fillet_1")

#Chamfer applying
Chamfer_1 = geompy.MakeChamferEdge(Fillet_1, 10, 10, 16, 50 )
geompy.addToStudy(Chamfer_1, "Chamfer_1")

Chamfer_2 = geompy.MakeChamferEdge(Chamfer_1, 10, 10, 21, 31 )
geompy.addToStudy(Chamfer_2, "Chamfer_2")

#Import of the shape from "slots.brep"
thePath = os.getenv("KERNEL_ROOT_DIR")
theFileName = thePath + "/examples/slots.brep"
theShapeForCut = geompy.ImportBREP(theFileName)
geompy.addToStudy(theShapeForCut, "slot.brep_1")

#Cut applying
Cut_1 = geompy.MakeBoolean(Chamfer_2, theShapeForCut, 2)
geompy.addToStudy(Cut_1, "Cut_1")

salome.sg.updateObjBrowser(1)
