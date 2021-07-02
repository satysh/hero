#include <iostream>

#include "TParticlePDG.h"
#include "TDatabasePDG.h"
#include "TMath.h"
#include "TGraph.h"
#include "TVectorD.h"

#include "FairPrimaryGenerator.h"
#include <FairRunSim.h>
#include <FairLogger.h>

#include "HEROSphereGenerator.h"



TGraph* energyCDFGr = NULL;
TGraph* energyInvCDFGr = NULL;

Double_t EnergyCDF(Double_t *x, Double_t *par) {
  return energyCDFGr->Eval(x[0]);
}

Double_t EnergyInvCDF(Double_t *x, Double_t *par) {
  return energyInvCDFGr->Eval(x[0]);
}

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

  TString path = TString(gSystem->Getenv("VMCWORKDIR")) + "/input/" + "flux.txt";
  std::ifstream f;
  f.open(path.Data());
  if (!f.is_open()) {
    Fatal("HEROSphereGenerator","file %s is not found.", path.Data());
    return kFALSE;
  }

  Int_t nPoints = std::count(std::istreambuf_iterator<char>(f),
                               std::istreambuf_iterator<char>(), '\n');
  f.seekg(0, std::ios::beg);
  TVectorD energy(nPoints);
  TVectorD flux(nPoints);

  LOG(INFO) << "nPoints = " << nPoints << FairLogger::endl;

  Int_t i = 0;
  while (!f.eof()) {
    if (i == nPoints) break;
      f >> energy(i) >> flux(i);
      i++;
  }

  energyCDFGr = new TGraph(energy, flux);
  energyInvCDFGr = new TGraph(flux, energy);

  TF1* energyCDF = new TF1("thetaCDF", EnergyCDF, 0.082, 20.76, 0);
  fEnergyInvCDF = new TF1("thetaInvCDF", EnergyInvCDF, 80.73, 7808.99, 0);

  fX = 0.;
  fY = 0.;
  fZ = GetDistance();
}

Bool_t HEROSphereGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
  Double32_t pabs = 1.,pt, theta = 0., phi = 0., px, py, pz;
  phi = fRnd.Uniform(0., 360.)*TMath::DegToRad();
  theta = fRnd.Uniform(-90., 90.)*TMath::DegToRad();
  pz = pabs*TMath::Cos(theta);
  pt = pabs*TMath::Sin(theta);
  px = pt*TMath::Cos(phi);
  py = pt*TMath::Sin(phi);

  primGen->AddTrack(fPID, px, py, pz, fX, fY, fZ);

  return kTRUE;
}

FairGenerator* HEROSphereGenerator::CloneGenerator() const
{
  // Clone for worker (used in MT mode only)

  return new HEROSphereGenerator(*this);
}

Double_t HEROSphereGenerator::EnergyGen()
{
  Double_t rndflux = fRnd.Uniform(80.73, 7808.99);
  Double_t eval = fEnergyInvCDF->Eval(rndflux);
  return eval;
}

ClassImp(HEROSphereGenerator)
