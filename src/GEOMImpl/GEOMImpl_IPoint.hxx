//NOTE: This is an intreface to a function for the point creation.


#include "GEOM_Function.hxx"

#define ARG_X     1
#define ARG_Y     2
#define ARG_Z     3

#define ARG_REF   4

#define ARG_PARAM 5
#define ARG_CURVE 6

class GEOMImpl_IPoint
{
 public:

  GEOMImpl_IPoint(Handle(GEOM_Function) theFunction): _func(theFunction) {}

  void SetX(double theX) { _func->SetReal(ARG_X, theX); }
  void SetY(double theY) { _func->SetReal(ARG_Y, theY); }
  void SetZ(double theZ) { _func->SetReal(ARG_Z, theZ); }

  double GetX() { return _func->GetReal(ARG_X); }
  double GetY() { return _func->GetReal(ARG_Y); }
  double GetZ() { return _func->GetReal(ARG_Z); }

  void SetRef(Handle(GEOM_Function) theRefPoint) { _func->SetReference(ARG_REF, theRefPoint); }

  Handle(GEOM_Function) GetRef() { return _func->GetReference(ARG_REF); }

  void SetCurve(Handle(GEOM_Function) theRef) { _func->SetReference(ARG_CURVE, theRef); }

  Handle(GEOM_Function) GetCurve() { return _func->GetReference(ARG_CURVE); }

  void SetParameter(double theParam) { _func->SetReal(ARG_PARAM, theParam); }

  double GetParameter() { return _func->GetReal(ARG_PARAM); }

 private:

  Handle(GEOM_Function) _func;
};