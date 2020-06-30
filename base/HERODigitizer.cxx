#include "HERODigitizer.h"
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include <TClonesArray.h>

#include "HERODigi.h"
#include "HEROPoint.h"

//UInt_t iEvent = 0;
HERODigitizer::HERODigitizer()
  : FairTask()
{
  cout << "HERODigitizer::HERODigitizer()" << endl;
}

HERODigitizer::~HERODigitizer()
{
  cout << "HERODigitizer::~HERODigitizer()" << endl;
}

InitStatus HERODigitizer::Init()
{
  cout << "HERODigitizer::Init" << endl;
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
    Fatal("Init", "No FairRootManager");
  TList* BranchsNamesList = ioman->GetBranchNameList();
  TIter next(BranchsNamesList);
  TObjString* branchName;
  while (branchName = (TObjString*)next()) {
    TString curBranchName = branchName->GetString();
    cout << "Current Branch Name is: " << curBranchName << endl;
    if (curBranchName.Contains("Point")) {
      cout << "Current Branch Name is: " << curBranchName << endl;
      // Input
      fPoints[curBranchName] = (TClonesArray*) ioman->GetObject(curBranchName); // input branch name
      if (!fPoints[curBranchName]) {
        Fatal("Init", "Can't find input collection %s!", curBranchName.Data());
      }

      // Output
      fDigis[curBranchName] = new TClonesArray("HERODigi"); // class name
      ioman->Register(curBranchName(0, curBranchName.Length()-5)+"Digi", curBranchName, fDigis[curBranchName], kTRUE);
    }
  }
  return kSUCCESS;
}

void HERODigitizer::Exec(Option_t* option)
{
  cout << "HERODigitizer::Exec" << endl;
  static Int_t iEvent = 0;
  for(const auto &itPoints: fPoints) {
    TClonesArray* curBranchArray = itPoints.second;
    Int_t curPID = GetInterestingPID();
    Double_t curEdep = 0.;
    Double_t curLightYield = 0.;
    Double_t curTime = 0.;

    Int_t curEntries = curBranchArray->GetEntries();
    cout << "iEvent: " << iEvent << "\t" << "Entries: " << curEntries << endl;
    for (UInt_t i = 0; i < curEntries; i++) {
      HEROPoint* curPoint = (HEROPoint*)curBranchArray->At(i);
      if (curPID == curPoint->GetPID() || curPID == -1) {// pid=-1 means that calculations are working for all particles
        curEdep += curPoint->GetEnergyLoss();
        curLightYield += curPoint->GetLightYield();
        curTime += curPoint->GetTime();
      }
    } // for end
    TClonesArray* curOutBra = fDigis[itPoints.first];
    AddOutputDigi(*curOutBra, curPID, curEdep, curLightYield, curTime);
  } // for end
  iEvent++;
}

void HERODigitizer::FinishEvent()
{
  cout << "HERODigitizer::FinishEvent()" << endl;
  for (const auto &itfDigis: fDigis) {
    itfDigis.second->Clear();
  }
}

void HERODigitizer::AddOutputDigi(TClonesArray& clref, Int_t pid, Double_t Edep, Double_t lightY, Double_t Time)
{
  cout << "HERODigitizer::AddOutputDigi(pid=" << pid << ", Edep=" << Edep << ", lightY=" << lightY
       << ", Time=" << Time << ")" << endl;
  UInt_t size = clref.GetEntries();
  new(clref[size])HERODigi(pid, Edep, lightY, Time, 1);
}

ClassImp(HERODigitizer)
