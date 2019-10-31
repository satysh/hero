// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include "TGeoMatrix.h"
#include "TVirtualMC.h"
#include "TParticle.h"

// HERO
#include "HERODetector.h"

//-------------------------------------------------------------------------------------------------
HERODetector::HERODetector()
: FairDetector("HERODetector", kTRUE, -1)
{
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
}
//-------------------------------------------------------------------------------------------------
HERODetector::HERODetector(const char* Name, Bool_t Active, Int_t DetId/*=0*/)
: FairDetector(Name, Active, DetId)
{
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
}
//-------------------------------------------------------------------------------------------------
void HERODetector::ConstructGeometry()
{
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    LOG(INFO) << "Constructing geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  }
  else if (fileName.EndsWith(".gdml")) {
    LOG(INFO) << "Constructing geometry from GDML file " << fileName.Data() << FairLogger::endl;
    TGeoRotation *zeroRotation = new TGeoRotation();
    zeroRotation->RotateX(0.);
    zeroRotation->RotateY(0.);
    zeroRotation->RotateZ(0.);
    ConstructGDMLGeometry(new TGeoCombiTrans(.0,.0,.0, zeroRotation));
  }
  else
    LOG(FATAL) << "Geometry file name is not set" << FairLogger::endl;
}
//-------------------------------------------------------------------------------------------------
void HERODetector::AddSensetive(TString name)
{
  fSenNames.push_back(name);
}
//-------------------------------------------------------------------------------------------------
TClonesArray* HERODetector::GetCollection(Int_t iColl) const
{
  if (fSenVolumes.size() > iColl) {
    auto it = fSenVolumes.begin();
    for (Int_t i = 0; i < iColl; i++)
      it++;
    return it->second;
  }
  else
    return NULL;
}
//-------------------------------------------------------------------------------------------------
void HERODetector::Register()
{
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
    LOG(FATAL) << "IO manager is not set" << FairLogger::endl;

  for(const auto &itSen: fSenVolumes) {
    ioman->Register(fName+itSen.first+TString("Point"),fName, itSen.second, kTRUE);
  }
}
//-------------------------------------------------------------------------------------------------
Bool_t HERODetector::ProcessHits(FairVolume* vol)
{
  static const Double_t constBirPer = 20.; // [g/GeV/cm^2]
  static const Double_t constBir = 12.6; // [cm/GeV] Birk's constant
  //if (testPid)
  if (gMC->IsTrackEntering()) { // Return true if this is the first step of the track in the current volume
    StartNewPoint();
  }

  fELoss += gMC->Edep(); // [GeV] It returns the energy lost in the current step

  if (gMC->TrackCharge() != 0) { // uncharged particles doesn't make any light yield
    if (gMC->TrackStep() > 0) {
      Double_t ratiodEdx = gMC->Edep()/gMC->TrackStep(); //[GeV/cm]
      Double_t curLightYield = gMC->Edep()/(1. + constBir*ratiodEdx); //[GeV]
      fLightYield += constBirPer*curLightYield;
    }
  }

  if (gMC->IsTrackExiting() || //true if this is the last step of the track in the current volume
      gMC->IsTrackStop()    || //true if the track energy has fallen below the threshold
      gMC->IsTrackDisappeared()) {
    FinishNewPoint();
  }
  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
void HERODetector::EndOfEvent()
{
  if (fVerbose > 1)
    Print();
  Reset();
}
//-------------------------------------------------------------------------------------------------
void HERODetector::Print(Option_t *option) const
{
  for(const auto &itSen: fSenVolumes) {
    TClonesArray* points = itSen.second;
    for (Int_t i_point = 0; i_point < points->GetEntriesFast(); i_point++) {
      HEROPoint* point = (HEROPoint*)points->At(i_point);
      point->Print();
    }
  }
}
//-------------------------------------------------------------------------------------------------
void HERODetector::Reset()
{
  for(const auto &itSen: fSenVolumes) {
    TClonesArray* points = itSen.second;
    points->Clear();
  }
  fFullEnergy = 0.;
  fFullLY = 0.;
}
//-------------------------------------------------------------------------------------------------
void HERODetector::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(DEBUG) << "HERODetector: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  HEROPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
    oldpoint = (HEROPoint*) cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[cl2->GetEntriesFast()]) HEROPoint(*oldpoint);
  }
  LOG(DEBUG) << "HERODetector: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;

}
//-------------------------------------------------------------------------------------------------
Bool_t HERODetector::CheckIfSensitive(std::string name)
{
  TString curVolName = name;
  for(const auto &volNameSubsting: fSenNames) {
    if(curVolName.Contains(volNameSubsting)) {
      fSenVolumes[curVolName] = new TClonesArray("HEROPoint");
      cerr << "CheckIfSensitive(" << name << ")" << endl;
      return kTRUE;
    }
  }
  return kFALSE;
}
//-------------------------------------------------------------------------------------------------
void HERODetector::StartNewPoint()
{
  fELoss  = 0.;
  fLightYield = 0.;
  fEventID = gMC->CurrentEvent();
  gMC->TrackPosition(fPosIn);
  gMC->TrackMomentum(fMomIn);
  fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
  fTimeIn   = gMC->TrackTime() * 1.0e09;  //current time of flight of the track being transported
  fTrackLength = gMC->TrackLength(); //length of the current track from its origin (in cm)
  fMot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
  fMass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
  Int_t curVolId, corOffVolId;
  curVolId = gMC->CurrentVolID(fVolNb);
  TGeoHMatrix matrix;
  gMC->GetTransformation(gMC->CurrentVolPath(), matrix);
  Double_t globalPos[3],localPos[3];
  fPosIn.Vect().GetXYZ(globalPos);
  matrix.MasterToLocal(globalPos,localPos);
  fPosInLocal.SetXYZ(localPos[0],localPos[1],localPos[2]);
  //LOG(INFO) << "  StartNewPoint(), time: " << fTimeIn << FairLogger::endl;
}
//-------------------------------------------------------------------------------------------------
void HERODetector::FinishNewPoint()
{
  gMC->TrackPosition(fPosOut);
  gMC->TrackMomentum(fMomOut);
  fTimeOut = gMC->TrackTime() * 1.0e09;

  //LOG(INFO) << "  FinishNewPoint(), time: " << fTimeOut << FairLogger::endl;
  if (fELoss > 0.) {
    TClonesArray* points = fSenVolumes[gMC->CurrentVolName()];
    AddPoint(points);
    fFullEnergy += fELoss;
    fFullLY += fLightYield;
  }
}
//-------------------------------------------------------------------------------------------------
HEROPoint* HERODetector::AddPoint(TClonesArray* points)
{
  static const Double_t scintE = 1.; // scintillator efficiency for Birk's low
  fLightYield *= scintE;
  TClonesArray& clref = *points;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) HEROPoint(fEventID, fTrackID, fMot0TrackID, fVolNb,
    fMass, fPosIn.Vect(),fPosInLocal,fPosOut.Vect(),fMomIn.Vect(),fMomOut.Vect(),fTimeIn,
    fTimeOut,fTrackLength, fELoss, fLightYield,gMC->TrackPid(), gMC->TrackCharge());
}

ClassImp(HERODetector)
