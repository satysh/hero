#ifndef HERO_SPHERE_GENERATOR_H
#define HERO_SPHERE_GENERATOR_H

// FAIRROOT
#include "FairGenerator.h"

// ROOT
#include "Rtypes.h"                     // for Double32_t, Bool_t, kTRUE, etc
#include "TRandom.h"
#include <TF1.h>

class FairPrimaryGenerator;

class HEROSphereGenerator : public FairGenerator
{
public:

  HEROSphereGenerator();

  HEROSphereGenerator(Int_t pdgid, Int_t mult=1);

  virtual ~HEROSphereGenerator() {};

  void SetDistance(Double_t dist) { fDistance=dist; }
  void SetThetaRange(Double32_t thmin, Double32_t thmax) { fThetaMin=thmin; fThetaMax=thmax; }

  Double_t GetDistance() { return fDistance; }
  Bool_t Init();

  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);

  virtual FairGenerator* CloneGenerator() const;

protected:
  HEROSphereGenerator (const HEROSphereGenerator &);
  HEROSphereGenerator & operator=(const HEROSphereGenerator &);

private:
  Double_t EnergyGen();

private:

  Int_t fPID;
  Int_t fMult;
  Double_t fDistance;
  Double32_t fPDGMass;
  Double32_t fThetaMin;
  Double32_t fThetaMax;

  Double32_t fX, fY, fZ; // Point vertex coordinates [cm]

  TRandom fRnd;
  TF1* fEnergyInvCDF=NULL;

  ClassDef(HEROSphereGenerator, 4);
};

#endif
