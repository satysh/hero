#ifndef HERO_SPHERE_GENERATOR_H
#define HERO_SPHERE_GENERATOR_H

// FAIRROOT
#include "FairGenerator.h"

// ROOT
#include "Rtypes.h"                     // for Double32_t, Bool_t, kTRUE, etc

class FairPrimaryGenerator;

class HEROSphereGenerator : public FairGenerator
{
public:

  HEROSphereGenerator();

  HEROSphereGenerator(Int_t pdgid, Int_t mult=1);

  virtual ~HEROSphereGenerator() {};

  Bool_t Init();

  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);

  virtual FairGenerator* CloneGenerator() const;

protected:
    HEROSphereGenerator (const HEROSphereGenerator &);
    HEROSphereGenerator & operator=(const HEROSphereGenerator &);
    ClassDef(HEROSphereGenerator,4);

private:

  Int_t fPID;
  Int_t fMult;

  Double32_t fX, fY, fZ;           // Point vertex coordinates [cm]
};


#endif
