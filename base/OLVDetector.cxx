// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include "TGeoMatrix.h"
#include "TVirtualMC.h"
#include "TParticle.h"

// OLV
#include "OLVDetector.h"

//-------------------------------------------------------------------------------------------------
OLVDetector::OLVDetector()
: FairDetector("OLVDetector", kTRUE, -1)
{
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
}
//-------------------------------------------------------------------------------------------------
OLVDetector::OLVDetector(const char* Name, Bool_t Active, Int_t DetId/*=0*/)
: FairDetector(Name, Active, DetId)
{
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
}
//-------------------------------------------------------------------------------------------------
void OLVDetector::ConstructGeometry()
{
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root"))
  {
    LOG(INFO) << "Constructing geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  }
  else if (fileName.EndsWith(".gdml"))
  {
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
void OLVDetector::AddSensetive(TString name)
{
  fSenNames.push_back(name);
}
//-------------------------------------------------------------------------------------------------
TClonesArray* OLVDetector::GetCollection(Int_t iColl) const
{
  if (fSenVolumes.size() > iColl)
  {
    auto it = fSenVolumes.begin();
    for (Int_t i = 0; i < iColl; i++)
      it++;
    return it->second;
  }
  else
    return NULL;
}
//-------------------------------------------------------------------------------------------------
void OLVDetector::Register()
{
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
    LOG(FATAL) << "IO manager is not set" << FairLogger::endl;

  for(const auto &itSen: fSenVolumes)
  {
    ioman->Register(fName+itSen.first+TString("Point"),fName, itSen.second, kTRUE);
  }
}
//-------------------------------------------------------------------------------------------------
Bool_t OLVDetector::ProcessHits(FairVolume* vol)
{
/*
  if (gMC->TrackPid() != 2112 && gMC->TrackPid() != 1000020040)
    return kFALSE;
*/
  /*
  if ((TString)vol->GetName() != "vCub")
    cerr << "ProcessHits(" << vol->GetName()  << ")" << endl;
  */

  //if (testPid)
  if (gMC->IsTrackEntering()) // Return true if this is the first step of the track in the current volume
  {
    StartNewPoint();
  }

  fELoss += gMC->Edep(); // GeV //Return the energy lost in the current step

  if (gMC->IsTrackExiting() || //true if this is the last step of the track in the current volume
      gMC->IsTrackStop()    || //true if the track energy has fallen below the threshold
      gMC->IsTrackDisappeared())
  {
    FinishNewPoint();
  }
  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
void OLVDetector::EndOfEvent()
{
  if (fVerbose > 1)
    Print();
  Reset();
}
//-------------------------------------------------------------------------------------------------
void OLVDetector::Print(Option_t *option) const
{
  for(const auto &itSen: fSenVolumes)
  {
    TClonesArray* points = itSen.second;
    for (Int_t i_point = 0; i_point < points->GetEntriesFast(); i_point++)
    {
      OLVPoint* point = (OLVPoint*)points->At(i_point);
      point->Print();
    }
  }
}
//-------------------------------------------------------------------------------------------------
void OLVDetector::Reset()
{
  for(const auto &itSen: fSenVolumes)
  {
    TClonesArray* points = itSen.second;
    points->Clear();
  }
  fFullEnergy = 0.;
  fFullLY = 0.;
}
//-------------------------------------------------------------------------------------------------
void OLVDetector::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(DEBUG) << "OLVDetector: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  OLVPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++)
  {
    oldpoint = (OLVPoint*) cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[cl2->GetEntriesFast()]) OLVPoint(*oldpoint);
  }
  LOG(DEBUG) << "OLVDetector: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;

}
//-------------------------------------------------------------------------------------------------
Bool_t OLVDetector::CheckIfSensitive(std::string name)
{
  TString curVolName = name;
  for(const auto &volNameSubsting: fSenNames)
  {
    if(curVolName.Contains(volNameSubsting)){
      fSenVolumes[curVolName] = new TClonesArray("OLVPoint");
      cerr << "CheckIfSensitive(" << name << ")" << endl;
      return kTRUE;
    }
  }
  return kFALSE;
}
//-------------------------------------------------------------------------------------------------
void OLVDetector::StartNewPoint()
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
void OLVDetector::FinishNewPoint()
{
  gMC->TrackPosition(fPosOut);
  gMC->TrackMomentum(fMomOut);
  fTimeOut = gMC->TrackTime() * 1.0e09;

  //LOG(INFO) << "  FinishNewPoint(), time: " << fTimeOut << FairLogger::endl;
  if (fELoss > 0.)
  {
    TClonesArray* points = fSenVolumes[gMC->CurrentVolName()];
    AddPoint(points);
    fFullEnergy += fELoss;
    fFullLY += fLightYield;
  }
}
//-------------------------------------------------------------------------------------------------
OLVPoint* OLVDetector::AddPoint(TClonesArray* points)
{
  TClonesArray& clref = *points;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) OLVPoint(fEventID, fTrackID, fMot0TrackID, fVolNb,
    fMass, fPosIn.Vect(),fPosInLocal,fPosOut.Vect(),fMomIn.Vect(),fMomOut.Vect(),fTimeIn,
    fTimeOut,fTrackLength, fELoss, fLightYield,gMC->TrackPid(), gMC->TrackCharge());
}

ClassImp(OLVDetector)
