// STD
#include <fstream>
#include <string>
#include <iostream>
#include <ostream>
using std::cerr;
using std::endl;

// ROOT
#include "TParticlePDG.h"
#include "TDatabasePDG.h"
#include "TMath.h"

// FairRoot
#include "FairPrimaryGenerator.h"
#include "FairLogger.h"

// HERO
#include "HERONeutronsGenerator.h"

HERONeutronsGenerator::HERONeutronsGenerator()
  : FairGenerator()
{
}

HERONeutronsGenerator::HERONeutronsGenerator(TString inFileName)
  : FairGenerator(), fInFileName(inFileName)
{
}

HERONeutronsGenerator::~HERONeutronsGenerator()
{
  if (fNeutronsBuffer.size())
    fNeutronsBuffer.clear();
  if (fBufferOfNeutronsNumberInEachEvent.size())
    fBufferOfNeutronsNumberInEachEvent.clear();
}

Bool_t HERONeutronsGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
  LOG(INFO) << "HERONeutronsGenerator::ReadEvent" << FairLogger::endl;
  static UInt_t neutronPositionInBuffer = 0;
  neutronPositionInBuffer++;
  fMult=fBufferOfNeutronsNumberInEachEvent[fEvent];
  if (fMult == 0) {
    cerr << "Event " << fEvent+1 << " doesn't have neutrons!" << endl;
    primGen->AddTrack(fPID, 0., 0., 0., 0., 0., fZ);
    fEvent++;
    return kTRUE;
  }
  cerr << "In Event=" << fEvent+1 << ", " << fBufferOfNeutronsNumberInEachEvent[fEvent] << endl;
  for (Int_t ii = 0; ii < fBufferOfNeutronsNumberInEachEvent[fEvent]; ii++) {
    if (fNeutronsBuffer[neutronPositionInBuffer+1] == 13) neutronPositionInBuffer++; // to skip neutron id in data
    fPx=fNeutronsBuffer[++neutronPositionInBuffer];
    fPy=fNeutronsBuffer[++neutronPositionInBuffer];
    fPz=fNeutronsBuffer[++neutronPositionInBuffer];
    fX=fNeutronsBuffer[++neutronPositionInBuffer];
    fY=fNeutronsBuffer[++neutronPositionInBuffer];
    neutronPositionInBuffer+=2; // to skip time end waight
    cerr << fPx << ", " << fPy << ", " << fPz << ", "
              << fX << ", " << ", " << fY << ", " << fZ << endl;
    primGen->AddTrack(fPID, fPx, fPy, fPz, fX, fY, fZ);
    fout << TMath::Sqrt(fPx*fPx+fPy*fPy+fPz*fPz) << "\t" << fX << "\t" << fY << endl;
  }
  fEvent++;
  return kTRUE;
}

Bool_t HERONeutronsGenerator::Init()
{
  LOG(INFO) << "HERONeutronsGenerator::Init" << FairLogger::endl;
  TDatabasePDG* pdgBase = TDatabasePDG::Instance();
  TParticlePDG* particle = pdgBase->GetParticle(fPID);

  if (! particle) {
    LOG(FATAL) << "HERONeutronsGenerator PDG code " << fPID << " not defined." << FairLogger::endl;
  }

  if (!ReadEventsFromTxt()) return kFALSE;
  Int_t counter=0;

  return kTRUE;
}

void HERONeutronsGenerator::SpreadingParams()
{
/*
  for (Int_t ii = 0; ii < fBufferOfNeutronsNumberInEachEvent[fEvent]; ii++) {
    for (Int_t jj = fEvent+1; jj < fEvent+8; jj++) {
      fPx=
    }
  }
*/
  Double_t pabs = 1.;
  fPx = 1.;
  fPy= 1.;
  fPz = 1.;
}

Bool_t HERONeutronsGenerator::ReadEventsFromTxt()
{
  LOG(INFO) << "HERONeutronsGenerator::ReadEventsFromTxt" << FairLogger::endl;
  std::ifstream fin(fInFileName.Data());
  if (!fin.is_open()) {
    LOG(FATAL) << "Can't read Input Txt << inFileName!" << FairLogger::endl;
    return kFALSE;
  }
  else {
    LOG(INFO) << fInFileName << " was read successfully!" << FairLogger::endl;
  }

  while (!fin.eof()) {
    std::string curString;
    fin >> curString;
    Int_t neutronsNumberInCurEvent = -1;
    if (curString == "N") {
      Int_t neutronsNumInCurEvent = -1;
      while (true) {
        Int_t curPositionInFile = fin.tellg();
        fin >> curString;
        if (fin.eof()) break;
        fin.seekg(curPositionInFile);
        if (curString != "starting") {
          Double_t num;
          fin >> num;
          fNeutronsBuffer.push_back(num);
        }
        else break;
        neutronsNumInCurEvent++;
      }
      fBufferOfNeutronsNumberInEachEvent.push_back(neutronsNumInCurEvent/8);
    }
  }
/*
  for (UInt_t ii = 0; ii < fBufferOfNeutronsNumberInEachEvent.size(); ii++) {
    cerr << "In Event: " << ii+1 << " neusNum is " << fBufferOfNeutronsNumberInEachEvent[ii] << endl;
    UInt_t limit = fBufferOfNeutronsNumberInEachEvent[Event]+8;
    for (Int_t jj = -1; jj < limit; jj++) {
      cerr << ", " << fNeutronsBuffer[jj+1] << ", ";
    }
    cerr << endl;

  }
*/
  fin.clear();
  fin.close();
  return kTRUE;
}

HERONeutronsGenerator::HERONeutronsGenerator(const HERONeutronsGenerator& rhs) :
  FairGenerator(rhs),
  fPID(rhs.fPID),
  fMult(rhs.fMult),
  fX(0.), fY(0.), fZ(0.)
{
  // Copy constructor
}

void HERONeutronsGenerator::SetPosZ(Double_t zz, Double_t sigma)
{
  fZ=zz; fSigmaZ=sigma; fZIsSet=kTRUE;
}

Double_t HERONeutronsGenerator::GetNumberOfNeutronsInEvent(Int_t ev)
{
  return fBufferOfNeutronsNumberInEachEvent[ev];
}

ClassImp(HERONeutronsGenerator)
