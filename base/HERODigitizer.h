#ifndef HERODigitizer_H
#define HERODigitizer_H

#include "FairTask.h" // mother class

#include <map>

class TClonesArray;

class HERODigitizer : public FairTask
{
public:
  /** Default constructor **/
  HERODigitizer();

  /** Destructor **/
  virtual ~HERODigitizer();

  /** Virtual method Init **/
  virtual InitStatus Init();

  virtual void Exec(Option_t* option = "");

  virtual void FinishEvent();

  void SetInterestingPID(Int_t pid) { fPID=pid; }
  Double_t GetInterestingPID() { return fPID; }

protected:
  // Input
  std::map<TString,TClonesArray*> fPoints;

  // Output
  std::map<TString,TClonesArray*> fDigis;

private:
  /** Write data to output file
    clref - output branch adress for current detector
    Edep - energy loss for current detector
    lightY - light yield for current detector
    Time - current time interval
  **/
  void AddOutputDigi(TClonesArray& clref, Int_t pid, Double_t Edep, Double_t lightY, Double_t Time);

  Int_t fPID=-1; // particle Id
  Bool_t fInsteadFlag=kFALSE; // flag to identify for the particle is skipped is set

  ClassDef(HERODigitizer, 1);
};

#endif // HERODigitizer_H
