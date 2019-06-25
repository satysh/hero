#include "OLVESphereGenerator.h"

#include "FairPrimaryGenerator.h"
#include <FairLogger.h>

#include "TParticlePDG.h"
#include "TDatabasePDG.h"
#include "TMath.h"
#include "TRandom.h"

OLVESphereGenerator::OLVESphereGenerator() :
  fPID(0),
  fMult(0),
  fX(0.), fY(0.), fZ(0.)
{
}

OLVESphereGenerator::OLVESphereGenerator(Int_t pdgid, Int_t mult) :
  fPID(pdgid),
  fMult(mult),
  fX(0.), fY(0.), fZ(0.)
{
}

OLVESphereGenerator::OLVESphereGenerator(const OLVESphereGenerator& rhs) :
  FairGenerator(rhs),
  fPID(rhs.fPID),
  fMult(rhs.fMult),
  fX(0.), fY(0.), fZ(0.)
{
  // Copy constructor
}

OLVESphereGenerator& OLVESphereGenerator::operator=(const OLVESphereGenerator& rhs)
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

Bool_t OLVESphereGenerator::Init()
{
  TDatabasePDG* pdgBase = TDatabasePDG::Instance();
  TParticlePDG* particle = pdgBase->GetParticle(fPID);
  if (! pdgBase) {
  	LOG(FATAL) << "OLVESphereGenerator pdg Base is not availble!" << FairLogger::endl;
  	return kFALSE;
  }
  else if (! particle) {
    Fatal("OLVESphereGenerator","PDG code %d not defined.",fPID);
    return kFALSE;
  }
  return kTRUE;
}

Bool_t OLVESphereGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
  for (Int_t i = 0; i < fMult; i++) {

    Double32_t Radius = 500., pabs = 0.5,pt, theta = 0., phi = 0., px, py, pz;
    //gRandom->Uniform(-pabs, pabs);
    theta = gRandom->Uniform(0.*TMath::DegToRad(), 180.*TMath::DegToRad());
    phi = gRandom->Uniform(0.*TMath::DegToRad(), 360.*TMath::DegToRad());

    Double32_t thetaCord = gRandom->Uniform(0.*TMath::DegToRad(), 180.*TMath::DegToRad());
    Double32_t phiCord = gRandom->Uniform(0.*TMath::DegToRad(), 360.*TMath::DegToRad());

    Double32_t Rxy = Radius*TMath::Sin(thetaCord);
    fX = Rxy*TMath::Cos(phiCord);
    fY = Rxy*TMath::Sin(phiCord);
    fZ = Radius*TMath::Cos(thetaCord);
/*
    if (thetaCord <= 90.*TMath::DegToRad()) {
      theta += 180.*TMath::DegToRad();
      pt = pabs*TMath::Sin(theta);
      px = pt*TMath::Cos(phi);
      py = pt*TMath::Sin(phi);
      pz = pabs*TMath::Cos(theta);
    }
    else if (thetaCord > 90.*TMath::DegToRad() && thetaCord <= 0.*TMath::DegToRad()) {
      theta += 270.*TMath::DegToRad();
      pt = pabs*TMath::Sin(theta);
      px = pt*TMath::Cos(phi);
      py = pt*TMath::Sin(phi);
      pz = pabs*TMath::Cos(theta);
    }
    else {
      pt = pabs*TMath::Sin(theta);
      px = pt*TMath::Cos(phi);
      py = pt*TMath::Sin(phi);
      pz = pabs*TMath::Cos(theta);
    }
*/
    pt = pabs*TMath::Sin(theta);
    px = pt*TMath::Cos(phi);
    py = pt*TMath::Sin(phi);
    pz = pabs*TMath::Cos(theta);
    //fZ = -500.;
    primGen->AddTrack(fPID, px, py, pz, fX, fY, fZ);
  }

  return kTRUE;
}

FairGenerator* OLVESphereGenerator::CloneGenerator() const
{
  // Clone for worker (used in MT mode only)

  return new OLVESphereGenerator(*this);
}

ClassImp(OLVESphereGenerator)
