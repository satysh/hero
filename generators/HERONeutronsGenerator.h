#ifndef HERONeutronsGenerator_H
#define HERONeutronsGenerator_H

// STD
#include <vector>

// ROOT
#include "TString.h"

// FairRoot
#include "FairGenerator.h"

class HERONeutronsGenerator : public FairGenerator
{
public:
  HERONeutronsGenerator();
  HERONeutronsGenerator(TString inFileName);
  ~HERONeutronsGenerator();

  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);
  virtual Bool_t Init();

  void SetPosZ(Double_t zz, Double_t sigma=0.);

  Double_t GetNumberOfNeutronsInEvent(Int_t ev);

private:
  std::vector<Int_t> fBufferOfNeutronsNumberInEachEvent;
  std::vector<Double_t> fNeutronsBuffer;
  TString fInFileName="";

  Int_t fPID=2112;
  Int_t fMult=0;
  Int_t fEvent=0;

  Double_t fPx=0.;
  Double_t fPy=0.;
  Double_t fPz=0.;
  Double_t fX=0.;
  Double_t fY=0.;
  Double_t fZ=0.;
  Double_t fSigmaZ=0.;
  Double_t fTime=0.;

  Bool_t fZIsSet=kFALSE;
private:
  void SpreadingParams();
  virtual Bool_t ReadEventsFromTxt();

  HERONeutronsGenerator(const HERONeutronsGenerator&);
  HERONeutronsGenerator& operator=(const HERONeutronsGenerator&);

  ClassDef(HERONeutronsGenerator, 1);
};

#endif // HERONeutronsGenerator_H
