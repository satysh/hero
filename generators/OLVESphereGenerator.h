#ifndef OLVE_SPHERE_GENERATOR_H
#define OLVE_SPHERE_GENERATOR_H

#include "FairGenerator.h"

class FairPrimaryGenerator;

class OLVESphereGenerator : public FairGenerator
{
public:

  OLVESphereGenerator();

  OLVESphereGenerator(Int_t pdgid, Int_t mult=1, Int_t num = 1);

  virtual ~OLVESphereGenerator() {};

  Bool_t ReadEvent(FairPrimaryGenerator* primGen);
  Bool_t Init();

  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);

  virtual FairGenerator* CloneGenerator() const;

protected:
    FairBoxGenerator(const FairBoxGenerator&);
    FairBoxGenerator& operator=(const FairBoxGenerator&);
    ClassDef(OLVESphereGenerator,4);
};


#endif
