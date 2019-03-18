// ROOT
#include "TParticle.h"
#include "TParticlePDG.h"
#include "TDatabasePDG.h"

// FAIRROOT
#include "FairRunSim.h"

// OLV
#include "OLVMCTrack.h"

OLVMCTrack::OLVMCTrack()
  : TObject(),
    fPdgCode(0),
    fMotherId(-1),
    fPx(0.),
    fPy(0.),
    fPz(0.),
    fStartX(0.),
    fStartY(0.),
    fStartZ(0.),
    fStartT(0.),
    fNPoints(0)
{
}

OLVMCTrack::OLVMCTrack(Int_t pdgCode, Int_t motherId, Double_t px,
           Double_t py, Double_t pz, Double_t x, Double_t y,
           Double_t z, Double_t t, Int_t nPoints = 0)
  : TObject(),
    fPdgCode(pdgCode),
    fMotherId(motherId),
    fPx(px),
    fPy(py),
    fPz(pz),
    fStartX(x),
    fStartY(y),
    fStartZ(z),
    fStartT(t),
    fNPoints(0)
{
  if (nPoints >= 0) fNPoints = nPoints;
  fPx = px;
  fPy = py;
  fPz = pz;
  fMass = CalculateMass();
  fEnergy = CalculateEnergy();
}

OLVMCTrack::OLVMCTrack(const OLVMCTrack& track)
  : TObject(track),
    fPdgCode(track.fPdgCode),
    fMotherId(track.fMotherId),
    fPx(track.fPx),
    fPy(track.fPy),
    fPz(track.fPz),
    fStartX(track.fStartX),
    fStartY(track.fStartY),
    fStartZ(track.fStartZ),
    fStartT(track.fStartT),
    fNPoints(track.fNPoints),
    fMass(track.fMass),
    fEnergy(track.fEnergy)
{
}

OLVMCTrack::OLVMCTrack(TParticle* part)
  : TObject(),
    fPdgCode(part->GetPdgCode()),
    fMotherId(part->GetMother(0)),
    fPx(part->Px()),
    fPy(part->Py()),
    fPz(part->Pz()),
    fStartX(part->Vx()),
    fStartY(part->Vy()),
    fStartZ(part->Vz()),
    fStartT(part->T()*1e09),
    fNPoints(0),
    // костыль Виталика TODO
    fID(part->GetStatusCode())
{
    fMass = CalculateMass();
    fEnergy = CalculateEnergy();
}

OLVMCTrack::~OLVMCTrack() 
{
}

void OLVMCTrack::Print(Int_t trackId) const 
{
  /* TODO*/
}

Double_t OLVMCTrack::GetMass() const 
{
  return fMass;
}

Double_t OLVMCTrack::GetCharge() const 
{
  if ( TDatabasePDG::Instance() ) 
  {
    TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle(fPdgCode);
    if ( particle ) return particle->Charge();
    else return 0.;
  }
  return 0.;
}

Double_t OLVMCTrack::GetRapidity() const 
{
  Double_t e = fEnergy;
  Double_t y = 0.5 * TMath::Log( (e+fPz) / (e-fPz) );
  return y;
}

Int_t OLVMCTrack::GetNPoints(Int_t detId) const 
{
  /* TODO */
    return 0;
}


void OLVMCTrack::SetNPoints(Int_t iDet, Int_t nPoints) 
{
  /* TODO */
}


TLorentzVector OLVMCTrack::GetVector()
{
  fMomentum.SetXYZT(fPx,fPy,fPz,fEnergy);
  return fMomentum;
}

Double_t OLVMCTrack::GetTheta()
{
  fMomentum.SetXYZT(fPx,fPy,fPz,fEnergy);
  fTheta = fMomentum.Theta();
  return fTheta;
}

Double_t OLVMCTrack::GetPhi()
{
  fMomentum.SetXYZT(fPx,fPy,fPz,fEnergy);
  fPhi = fMomentum.Phi();
  return fPhi;
}

Double_t OLVMCTrack::CalculateMass() 
{
  if ( TDatabasePDG::Instance() ) 
  {
    TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle(fPdgCode);
    if ( particle ) 
    {
      TString particleName = particle->GetName();
      TObjArray *userIons  = FairRunSim::Instance()->GetUserDefIons();
      FairIon* ion = (FairIon*)userIons->FindObject(particleName);
      if (ion) 
        return ion->GetMass() + ion->GetExcEnergy();
      else 
        return particle->Mass();
    }
    else 
      return 0.;
  }
  return 0.;
}
// -------------------------------------------------------------------------
Double_t OLVMCTrack::CalculateEnergy() 
{
  return TMath::Sqrt(fMass*fMass + fPx*fPx + fPy*fPy + fPz*fPz );
}


ClassImp(OLVMCTrack)
