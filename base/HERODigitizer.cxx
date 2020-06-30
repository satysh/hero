#include "HERODigitizer.h"
/*
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
*/
#include <TClonesArray.h>

#include "HERODigi.h"
#include "HEROPoint.h"

//UInt_t iEvent = 0;
HERODigitizer::HERODigitizer()
  : FairTask()
{
    //cout << "HERODigitizer::HERODigitizer()" << endl;
}

HERODigitizer::~HERODigitizer()
{
    //cout << "HERODigitizer::~HERODigitizer()" << endl;
}

InitStatus HERODigitizer::Init()
{
//    cout << "HERODigitizer::Init" << endl;
    FairRootManager* ioman = FairRootManager::Instance();
    if ( ! ioman ) Fatal("Init", "No FairRootManager");

    TList* BranchsNamesList = ioman->GetBranchNameList();
    TIter next(BranchsNamesList);
    TObjString* branchName;
    while (branchName = (TObjString*)next()) {
        TString curBranchName = branchName->GetString();
//        cout << "Current Branch Name is: " << curBranchName << endl;
        if (curBranchName.Contains("Point")) {
//            cout << "Current Branch Name is: " << curBranchName << endl;
            // Input
            fPoints[curBranchName] = (TClonesArray*) ioman->GetObject(curBranchName); // input branch name
            if (!fPoints[curBranchName]) {
                Fatal("Init", "Can't find input collection DetectorvDetGasPartPoint!");
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
//    cout << "HERODigitizer::Exec" << endl;
    for(const auto &itPoints: fPoints) {
        TClonesArray* curBranchArray = itPoints.second;
        Double_t curEdep = 0.;
        Double_t curTime = 0.;
        Int_t curEntries = curBranchArray->GetEntries();
//        cout << "iEvent: " << iEvent << "\t" << "Entries: " << curEntries << endl;
        for (UInt_t i = 0; i < curEntries; i++) {
            HEROPoint* curPoint = (HEROPoint*)curBranchArray->At(i);
            curEdep = curEdep + curPoint->GetEnergyLoss();
            curTime = curTime + curPoint->GetTime();
/*
            if (curPoint->GetCharge() == 5) {
                cout << "B11 has been found" << endl;
                cout << "Event: " << iEvent << "\t" << "Branch: " << itPoints.first << endl;
                cout << "Time: " << curTime << endl;
                cout << "VolNb: " << curPoint->GetVolNb() << endl;
                cout << "curEdep: " << curEdep << endl;
            }
*/
        } // for end
        TClonesArray* curOutBra = fDigis[itPoints.first];
        AddOutputDigi(*curOutBra, curEdep, curTime);
    } // for end
    //iEvent++;
}

void HERODigitizer::AddOutputDigi(TClonesArray& clref, Double_t Edep, Double_t Time)
{
/*    cout << "HERODigitizer::AddOutputDigi(Edep = " << Edep << ", "
           << "Time = " << Time << ")" << endl;
*/
    UInt_t size = clref.GetEntries();
    new(clref[size])HERODigi(1, Edep, Time, 1); // id = 1, Edep = Edep, time = Time, volNb = 1
}

void HERODigitizer::FinishEvent()
{
//    cout << "HERODigitizer::FinishEvent()" << endl;
    for (const auto &itfDigis: fDigis) {
        itfDigis.second->Clear();
    }
}

ClassImp(HERODigitizer)
