# -*- coding: iso-8859-1 -*-

import sys
import salome

import GEOM
from salome.geom import geomBuilder
import math
import SALOMEDS


def demidisk(study, r1, a1, roty=0, solid_thickness=0):
  if solid_thickness < 1e-7:
    with_solid = False
  else:
    with_solid = True

  geompy = geomBuilder.New(study)
  
  O = geompy.MakeVertex(0, 0, 0)
  OX = geompy.MakeVectorDXDYDZ(1, 0, 0) 
  OY = geompy.MakeVectorDXDYDZ(0, 1, 0)
  OZ = geompy.MakeVectorDXDYDZ(0, 0, 1)
  
  v=range(8)
  l=range(8)
  v0 = geompy.MakeVertex(0, 0, 0)
  v[0] = geompy.MakeVertex(0, r1/2.0, 0)
  v[1] = geompy.MakeVertex(0, r1, 0)
  l[1] = geompy.MakeLineTwoPnt(v[0], v[1])
  l[2] = geompy.MakeRotation(l[1], OX, a1*math.pi/180.0)
  v[4] = geompy.MakeRotation(v[0], OX, a1*math.pi/180.0)
  v[6] = geompy.MakeRotation(v[1], OX, a1*math.pi/180.0)

  v[2] = geompy.MakeVertex(0, -r1/2.0, 0)
  v[3] = geompy.MakeVertex(0, -r1, 0)
  l[3] = geompy.MakeLineTwoPnt(v[2], v[3])
  l[4] = geompy.MakeRotation(l[3], OX, -a1*math.pi/180.0)
  v[5] = geompy.MakeRotation(v[2], OX, -a1*math.pi/180.0)
  v[7] = geompy.MakeRotation(v[3], OX, -a1*math.pi/180.0)

  l[5] = geompy.MakeLineTwoPnt(v[4], v[5])
  l[6] = geompy.MakeLineTwoPnt(v[0], v[4])
  l[7] = geompy.MakeLineTwoPnt(v[2], v[5])

  v7 = geompy.MakeVertex(0, 0, r1)
  arc1 = geompy.MakeArc(v[1], v7, v[3])
  l[0] = geompy.MakeLineTwoPnt(v[1], v[3])
  face1 = geompy.MakeFaceWires([arc1, l[0]], 1)

  if with_solid:
    # Vertices
    v0 = geompy.MakeVertex(0, r1 + solid_thickness, 0)
    v1 = geompy.MakeRotation(v0, OX, a1*math.pi/180.0)
    v2 = geompy.MakeRotation(v0, OX, math.pi - (a1*math.pi/180.0))
    v3 = geompy.MakeRotation(v0, OX, math.pi)
    v.extend([v0,v1,v3,v2]) # The order is important for use in pointsProjetes
    l0 = geompy.MakeLineTwoPnt(v[1], v0)
    l2 = geompy.MakeRotation(l0, OX, a1*math.pi/180.0)
    l3 = geompy.MakeRotation(l0, OX, math.pi - (a1*math.pi/180.0))
    face2 = geompy.MakeRevolution(l0, OX, a1*math.pi/180.0)
    face3 = geompy.MakeRevolution(l2, OX, math.pi - 2*a1*math.pi/180.0)
    face4 = geompy.MakeRevolution(l3, OX, a1*math.pi/180.0)
    part0 = geompy.MakePartition([face1], [l[2], l[4], l[5], l[6], l[7]], [], [], geompy.ShapeType["FACE"], 0, [], 0, True)
    compound1 = geompy.MakeCompound([part0, face2, face3, face4])
    part1 = geompy.MakeGlueEdges(compound1,1e-7)
  else:
    part1 = geompy.MakePartition([face1], [l[2], l[4], l[5], l[6], l[7]], [], [], geompy.ShapeType["FACE"], 0, [], 0, True)

  if roty != 0:
    vrot = [ geompy.MakeRotation(vert, OY, roty*math.pi/180.0) for vert in v ]
    lrot = [ geompy.MakeRotation(lin, OY, roty*math.pi/180.0) for lin in l ]
    arc = geompy.MakeRotation(arc1, OY, roty*math.pi/180.0)
    part = geompy.MakeRotation(part1, OY, roty*math.pi/180.0)
    return vrot, lrot, arc, part
  else:
    return v, l, arc1, part1

def pointsProjetes(study, vref, face):
  geompy = geomBuilder.New(study)
  vface = geompy.ExtractShapes(face, geompy.ShapeType["VERTEX"], True)
  vord = range(len(vref))
  plan = geompy.MakePlaneThreePnt(vref[0], vref[1], vref[-1], 10000)
  vproj = [ geompy.MakeProjection(vert, plan) for vert in vface ]
  for i,v in enumerate(vproj):
    dist = [ (geompy.MinDistance(v, vr), j) for j,vr in enumerate(vref) ]
    dist.sort()
    #print dist
    if dist[0][0] < 1.e-3:
      vord[dist[0][1]] = vface[i]
  return vord

def arcsProjetes(study, vf, face):
  geompy = geomBuilder.New(study)
  lface = geompy.ExtractShapes(face, geompy.ShapeType["EDGE"], True)
  lord = range(3)
  ends = [vf[1], vf[6], vf[7], vf[3]]
  for i in range(3):
    for lf in lface:
      pts = geompy.ExtractShapes(lf, geompy.ShapeType["VERTEX"], True)
      if (((geompy.MinDistance(pts[0], ends[i]) < 0.001) and (geompy.MinDistance(pts[1], ends[i+1]) < 0.001)) or
          ((geompy.MinDistance(pts[1], ends[i]) < 0.001) and (geompy.MinDistance(pts[0], ends[i+1]) < 0.001))):
        lord[i] = lf
        print "arc_%d OK"%i
        break
    pass
  return lord
 
def build_shape(study, r1, r2, h1, h2, solid_thickness=0):
  if solid_thickness < 1e-7:
    with_solid = False
  else:
    with_solid = True
  
  geompy = geomBuilder.New(study)
  
  O = geompy.MakeVertex(0, 0, 0)
  OX = geompy.MakeVectorDXDYDZ(1, 0, 0) 
  OY = geompy.MakeVectorDXDYDZ(0, 1, 0)
  OZ = geompy.MakeVectorDXDYDZ(0, 0, 1)
  
  a1 = 45.0
  seuilmax = 0.1
  ratio = float(r2)/float(r1)
  if ratio > (1.0 -seuilmax):
    a1 = 45.0*(1.0 -ratio)/seuilmax
  
  """
  res = geompy.MakeCompound([demicyl1,demicyl2])
  return res
  """

  # --- creation des faces de la jonction
  [faci, sect45, arc1, l1, lord90, lord45, edges, arcextru] = jonction(study, r1, r2,\
                                                                       h1, h2, a1)
  if with_solid:
    [faci_ext, sect45_ext, arc1_ext, l1_ext, \
     lord90_ext, lord45_ext, edges_ext, arcextru_ext] = jonction(study, r1 + solid_thickness, r2 + solid_thickness,\
                                                                 h1, h2, a1)
    faces_jonction_ext = []
    for i,l in enumerate(lord90):
      faces_jonction_ext.append(geompy.MakeQuad2Edges(lord90[i],lord90_ext[i]))
    for i in [1, 3, 6, 7]:
      faces_jonction_ext.append(geompy.MakeQuad2Edges(edges[i],edges_ext[i]))
    for i,l in enumerate(lord45):
      faces_jonction_ext.append(geompy.MakeQuad2Edges(lord45[i],lord45_ext[i]))
   
    for i,face in enumerate(faces_jonction_ext):
      geompy.addToStudy(faces_jonction_ext[i], "faci_ext_%d"%i)

  # --- extrusion droite des faces de jonction, pour reconstituer les demi cylindres
  # TODO : ajouter les faces nécessaires à sect45 dans le cas avec solide
  if with_solid:    
    sect45 = geompy.MakeCompound([sect45]+faces_jonction_ext[-3:])
    sect45 = geompy.MakeGlueEdges(sect45, 1e-7)
    
  #return sect45, faces_jonction_ext[-3:]
  extru1 = geompy.MakePrismVecH(sect45, OX, h1+10)

  #base2 = geompy.MakeCompound(faci[5:])
  #base2 = geompy.MakeGlueEdges(base2, 1e-7)
  # RNC : perf
  faces_coupe = faci[5:]
  if with_solid:
    faces_coupe = faci[5:]+faces_jonction_ext[:3]
  base2 = geompy.MakePartition(faces_coupe, [], [], [], geompy.ShapeType["FACE"], 0, [], 0, True)
  extru2 = geompy.MakePrismVecH(base2, OZ, h2)

  # --- partition et coupe

  if with_solid:
     demiDisque = geompy.MakeFaceWires([arc1_ext, l1_ext[0]], 1)
  else:
     demiDisque = geompy.MakeFaceWires([arc1, l1[0]], 1)
  demiCylindre = geompy.MakePrismVecH(demiDisque, OX, h1)

  box = geompy.MakeBox(0, -2*(r1+h1), -2*(r1+h1), 2*(r1+h1), 2*(r1+h1), 2*(r1+h1))
  rot = geompy.MakeRotation(box, OY, 45*math.pi/180.0)

  garder = geompy.MakeCutList(demiCylindre, [extru2, rot], True)
  geompy.addToStudy(garder,"garder")
  
  faces_coupe = faci[:5]
  if with_solid:
    faces_coupe.extend(faces_jonction_ext[-7:])
  raccord = geompy.MakePartition([garder], faces_coupe + [arcextru], [], [], geompy.ShapeType["SOLID"], 0, [], 0, True)
  assemblage = geompy.MakeCompound([raccord, extru1, extru2])
  assemblage = geompy.MakeGlueFaces(assemblage, 1e-7)
  # RNC : perf
  #assemblage = geompy.MakePartition([raccord, extru1, extru2], [], [], [], geompy.ShapeType["SOLID"], 0, [], 0, True)
  
  #return extru2, garder, raccord

  box = geompy.MakeBox(-1, -(r1+r2+2*solid_thickness), -1, h1, r1+r2+2*solid_thickness, h2)
  geompy.addToStudy(box, "box")
  final = geompy.MakeCommonList([box, assemblage], True)
  
  # --- Partie inférieure
  v3, l3, arc3, part3 = demidisk(study, r1, a1, 180.0, solid_thickness)
  geompy.addToStudy(part3,"part3")
  extru3 = geompy.MakePrismVecH(part3, OX, h1)
  geompy.addToStudy(extru3,"extru3")

  # --- Symétrie

  compound = geompy.MakeCompound([final, extru3])
  plane = geompy.MakePlane(O,OX,2000)
  compound_mirrored = geompy.MakeMirrorByPlane(compound, plane)
  final = geompy.MakeCompound([compound, compound_mirrored])
  
  return final


def jonction(study, r1, r2, h1, h2, a1):
  
  O = geompy.MakeVertex(0, 0, 0)
  OX = geompy.MakeVectorDXDYDZ(1, 0, 0) 
  OY = geompy.MakeVectorDXDYDZ(0, 1, 0)
  OZ = geompy.MakeVectorDXDYDZ(0, 0, 1)
  
  # --- sections droites des deux demi cylindres avec le partionnement
  v1, l1, arc1, part1 = demidisk(study, r1, a1, 0.)
  v2, l2, arc2, part2 = demidisk(study, r2, a1, 90.0)
  #elems_disk1 = [v1, l1, arc1, part1]
  #elems_disk2 = [v2, l2, arc2, part2]

  # --- extrusion des sections --> demi cylindres de travail, pour en extraire les sections utilisées au niveau du Té
  #     et enveloppe cylindrique du cylindre principal

  demicyl1 = geompy.MakePrismVecH(part1, OX, h1)
  demicyl2 = geompy.MakePrismVecH(part2, OZ, h2)
  arcextru = geompy.MakePrismVecH(arc1, OX, h1)
  
  # --- plan de coupe à 45° sur le cylindre principal,
  #     section à 45° du cylndre principal,
  #     section du cylindre secondaire par l'enveloppe cylindrique du cylindre principal

  plan1 = geompy.MakePlane(O, OX, 4*r1)
  planr = geompy.MakeRotation(plan1, OY, 45*math.pi/180.0)
  geompy.addToStudy(planr, 'planr')

  sect45 = geompy.MakeCommonList([demicyl1, planr], True)
  geompy.addToStudy(sect45, 'sect45')

  sect90 = geompy.MakeCommonList([demicyl2, arcextru], True)
  geompy.addToStudy(sect90, 'sect90')
  
  # --- liste ordonnée des points projetés sur les deux sections

  vord45 = pointsProjetes(study, v1, sect45)
  vord90 = pointsProjetes(study, v2, sect90)
  for i,v in enumerate(vord45):
    geompy.addToStudyInFather(sect45, v, 'v%d'%i)
  for i,v in enumerate(vord90):
    geompy.addToStudyInFather(sect90, v, 'v%d'%i)

  # --- identification des projections des trois arcs de cercle, sur les deux sections.

  lord45 = arcsProjetes(study, vord45, sect45)
  lord90 = arcsProjetes(study, vord90, sect90)
  for i,l in enumerate(lord45):
    geompy.addToStudyInFather(sect45, l, 'l%d'%i)
  for i,l in enumerate(lord90):
    geompy.addToStudyInFather(sect90, l, 'l%d'%i)

  # --- abaissement des quatre points centraux de la section du cylindre secondaire

  #if with_solid:
    #dz = -(r2 + solid_thickness)/2.0
  #else:
    #dz = -r2/2.0
  dz = -r2/2.0
  for i in (0, 2, 4, 5):
    vord90[i] = geompy.TranslateDXDYDZ(vord90[i], 0, 0, dz, True)
    geompy.addToStudyInFather(sect90, vord90[i], 'vm%d'%i)
  #if with_solid:
    #for i in (1, 3, 6, 7):
      #vord90[i] = geompy.TranslateDXDYDZ(vord90[i], 0, 0, dz*solid_thickness/(r2+solid_thickness), True)

  """
  res=vord90
  return res
  """
    
  # --- création des deux arêtes curvilignes sur l'enveloppe cylindrique du cylindre principal, à la jonction

  curv = [None for i in range(4)] # liaisons entre les points 1, 3, 6 et 7 des 2 sections

  curv[0] = geompy.MakeArcCenter(O, vord90[1] , vord45[1], False)
  curv[1] = geompy.MakeArcCenter(O, vord90[3] , vord45[3], False)

  lipts = ((6, 6, 4), (7, 7, 5))
  for i, ipts in enumerate(lipts):
    print i, ipts
    p0 = vord90[ipts[0]]
    p1 = vord45[ipts[1]]
    p2 = vord45[ipts[2]]
    plan = geompy.MakePlaneThreePnt(p0, p1, p2, 10000)
    #geompy.addToStudy(plan, "plan%d"%i)
    section = geompy.MakeSection(plan, arcextru, True)
    secpart = geompy.MakePartition([section], [sect45, sect90], [], [], geompy.ShapeType["EDGE"], 0, [], 0, True)
    geompy.addToStudy(secpart, "secpart%d"%i)
    lsec = geompy.ExtractShapes(secpart, geompy.ShapeType["EDGE"], True)
    #print "len(lsec)", len(lsec)

    # TODO : revoir ça dans le cas avec solide
    for l in lsec:
      pts = geompy.ExtractShapes(l, geompy.ShapeType["VERTEX"], True)
      if (((geompy.MinDistance(pts[0], p0) < 0.001) and (geompy.MinDistance(pts[1], p1) < 0.001)) or
          ((geompy.MinDistance(pts[1], p0) < 0.001) and (geompy.MinDistance(pts[0], p1) < 0.001))):
        curv[i+2] =l
        print "curv_%d OK"%i
        break
  # RNC : commente temporairement
  #for i,l in enumerate(curv):
  #  geompy.addToStudyInFather(arcextru, l, "curv%d"%i)
    
  # --- creation des arêtes droites manquantes, des faces et volumes pour les quatre volumes de la jonction

  edges = [None for i in range(8)]
  edges[0] = geompy.MakeLineTwoPnt(vord45[0], vord90[0])
  edges[1] = curv[0]
  edges[2] = geompy.MakeLineTwoPnt(vord45[2], vord90[2])
  edges[3] = curv[1]
  edges[4] = geompy.MakeLineTwoPnt(vord45[4], vord90[4])
  edges[5] = geompy.MakeLineTwoPnt(vord45[5], vord90[5])
  edges[6] = curv[2]
  edges[7] = curv[3]
  for i,l in enumerate(edges):
    print i
    geompy.addToStudy( l, "edge%d"%i)

  ed45 = [None for i in range(8)]
  ed45[0] = geompy.MakeLineTwoPnt(vord45[0], vord45[2])
  ed45[1] = geompy.MakeLineTwoPnt(vord45[0], vord45[1])
  ed45[2] = geompy.MakeLineTwoPnt(vord45[4], vord45[6])
  ed45[3] = geompy.MakeLineTwoPnt(vord45[2], vord45[3])
  ed45[4] = geompy.MakeLineTwoPnt(vord45[5], vord45[7])
  ed45[5] = geompy.MakeLineTwoPnt(vord45[4], vord45[5])
  ed45[6] = geompy.MakeLineTwoPnt(vord45[0], vord45[4])
  ed45[7] = geompy.MakeLineTwoPnt(vord45[2], vord45[5])
  for i,l in enumerate(ed45):
    geompy.addToStudyInFather(sect45, l, "ed45_%d"%i)

  ed90 = [None for i in range(8)]
  ed90[0] = geompy.MakeLineTwoPnt(vord90[0], vord90[2])
  ed90[1] = geompy.MakeLineTwoPnt(vord90[0], vord90[1])
  ed90[2] = geompy.MakeLineTwoPnt(vord90[4], vord90[6])
  ed90[3] = geompy.MakeLineTwoPnt(vord90[2], vord90[3])
  ed90[4] = geompy.MakeLineTwoPnt(vord90[5], vord90[7])
  ed90[5] = geompy.MakeLineTwoPnt(vord90[4], vord90[5])
  ed90[6] = geompy.MakeLineTwoPnt(vord90[0], vord90[4])
  ed90[7] = geompy.MakeLineTwoPnt(vord90[2], vord90[5])
  for i,l in enumerate(ed90):
    geompy.addToStudyInFather(sect90, l, "ed90_%d"%i)

  faci = []
  faci.append(geompy.MakeFaceWires([ed45[6], edges[0], ed90[6], edges[4]], 0))
  faci.append(geompy.MakeFaceWires([ed45[7], edges[2], ed90[7], edges[5]], 0))
  faci.append(geompy.MakeFaceWires([ed45[2], edges[4], ed90[2], edges[6]], 0))
  faci.append(geompy.MakeFaceWires([ed45[5], edges[4], ed90[5], edges[5]], 0))
  faci.append(geompy.MakeFaceWires([ed45[4], edges[5], ed90[4], edges[7]], 0))
  faci.append(geompy.MakeFaceWires([ed90[0], ed90[6], ed90[5], ed90[7]], 0))
  faci.append(geompy.MakeFaceWires([ed90[1], ed90[6], ed90[2], lord90[0]], 0))
  faci.append(geompy.MakeFaceWires([ed90[2], ed90[5], ed90[4], lord90[1]], 0))
  faci.append(geompy.MakeFaceWires([ed90[3], ed90[7], ed90[4], lord90[2]], 0))
  for i,f in enumerate(faci):
    geompy.addToStudy(f, "faci_%d"%i)

  return faci, sect45, arc1, l1, lord90, lord45, edges, arcextru

if __name__=="__main__":
  """For testing purpose"""
  salome.salome_init()
  theStudy = salome.myStudy
  geompy = geomBuilder.New(theStudy)
  res = build_shape(theStudy, 80., 20., 100., 100., 10.)
  """
  for i,v in enumerate(res):
    geompy.addToStudy(v,"v%d"%i)
  """

  #res = demidisk(theStudy, 80, 45, 0, 10)
  #geompy.addToStudy(res[3], "res")
  #for i,v in enumerate(res[0]):
  #  geompy.addToStudy(v,"v%d"%i)
  geompy.addToStudy(res, "res")
  