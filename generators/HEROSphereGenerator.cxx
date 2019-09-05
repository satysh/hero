#include "HEROSphereGenerator.h"

#include "FairPrimaryGenerator.h"

#include "TParticlePDG.h"
#include "TDatabasePDG.h"
#include "TMath.h"

HEROSphereGenerator::HEROSphereGenerator() :
  fPID(0),
  fMult(0),
  fX(0.), fY(0.), fZ(0.)
{
}

HEROSphereGenerator::HEROSphereGenerator(Int_t pdgid, Int_t mult) :
  fPID(pdgid),
  fMult(mult),
  fX(0.), fY(0.), fZ(0.)
{
}

HEROSphereGenerator::HEROSphereGenerator(const HEROSphereGenerator& rhs) :
  FairGenerator(rhs),
  fPID(rhs.fPID),
  fMult(rhs.fMult),
  fX(0.), fY(0.), fZ(0.)
{
  // Copy constructor
}

HEROSphereGenerator& HEROSphereGenerator::operator=(const HEROSphereGenerator& rhs)
{
  // Assignment operator

  // check assignment to self
  if (this == &rhs) return *this;

  // base class assignment
  TNamed::operator=(rhs);

  // assignment operator
  fPID = rhs.fPID;
  fMult = rhs.fMult;

  return *this;
}

Bool_t HEROSphereGenerator::Init()
{
  TDatabasePDG* pdgBase = TDatabasePDG::Instance();
  TParticlePDG* particle = pdgBase->GetParticle(fPID);

  if (! particle) {
    Fatal("HEROSphereGenerator","PDG code %d not defined.",fPID);
  }

}

Bool_t HEROSphereGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
  Double32_t pabs = 1.,pt, theta = 0., phi = 0., px, py, pz;
  pz = pabs*TMath::Cos(theta);
  pt = pabs*TMath::Sin(theta);
  px = pt*TMath::Cos(phi);
  py = pt*TMath::Sin(phi);
  fZ = -500.;

  primGen->AddTrack(fPID, px, py, pz, fX, fY, fZ);

  return kTRUE;
}

FairGenerator* HEROSphereGenerator::CloneGenerator() const
{
  // Clone for worker (used in MT mode only)

  return new HEROSphereGenerator(*this);
}

ClassImp(HEROSphereGenerator)
