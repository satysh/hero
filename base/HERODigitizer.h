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

protected:

    // Input
    std::map<TString,TClonesArray*> fPoints;

    // Output
    std::map<TString,TClonesArray*> fDigis;

    //TClonesArray* fPoints;
    //TClonesArray* fDigis;

private:
    /** Write data to output file
        clref - output branch adress for current detector
        Edep - energy loss for current detector
        Time - current time interval
    **/
    void AddOutputDigi(TClonesArray& clref, Double_t Edep = 0., Double_t Time = 0.);

    ClassDef(HERODigitizer, 1);
};

#endif // HERODigitizer_H
