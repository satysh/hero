#ifndef OLVE_SPHERE_GENERATOR_H
#define OLVE_SPHERE_GENERATOR_H

// FAIRROOT
#include "FairGenerator.h"

// ROOT
#include "Rtypes.h"                     // for Double32_t, Bool_t, kTRUE, etc

class FairPrimaryGenerator;

class OLVESphereGenerator : public FairGenerator
{
public:

  OLVESphereGenerator();

  OLVESphereGenerator(Int_t pdgid, Int_t mult=1);

  virtual ~OLVESphereGenerator() {};

  Bool_t Init();

  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);

  virtual FairGenerator* CloneGenerator() const;

protected:
    OLVESphereGenerator (const OLVESphereGenerator &);
    OLVESphereGenerator & operator=(const OLVESphereGenerator &);
    ClassDef(OLVESphereGenerator,4);

private:

  Int_t fPID;
  Int_t fMult;

  Double32_t fX, fY, fZ;           // Point vertex coordinates [cm]
};


#endif
