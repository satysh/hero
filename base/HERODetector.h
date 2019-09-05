#ifndef HERODetector_H
#define HERODetector_H

// ROOT
#include "TLorentzVector.h"
#include "TClonesArray.h"

// FairRoot
#include "FairDetector.h"
#include "FairVolume.h"

// HERO
#include "HEROPoint.h"

class HERODetector : public FairDetector
{

public:

    HERODetector();

    HERODetector(const char* Name, Bool_t Active, Int_t DetId=0);
    virtual ~HERODetector() { ; }

    TClonesArray* GetCollection(Int_t iColl) const;

    void ConstructGeometry();
    void AddSensetive(TString name);
    void Register();
    void EndOfEvent();
    void Print(Option_t *option="") const;
    void Reset();
    void CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset);

    Bool_t ProcessHits(FairVolume* vol = 0);
    Bool_t CheckIfSensitive(std::string name);
private:
    void StartNewPoint();
    void FinishNewPoint();

    HEROPoint* AddPoint(TClonesArray* points);

    //map of sensetive volumes points collection collection, fSenVolumes[senVol] = points
    std::map<TString,TClonesArray*> fSenVolumes;

    std::vector<TString> fSenNames;   ///< Sensetive volumes sustring, that user set

    Int_t   fVerbose;                 ///< Verbosity level

    Double_t         fFullEnergy;     ///< Sum Edep in event in sensetive volume
    Double_t         fFullLY;         ///< Sum Light Yield in event in sensetive volume

    /* current step information*/
    Int_t           fEventID;         ///< event index
    Int_t           fTrackID;         ///< track index
    Int_t           fMot0TrackID;     ///< 0th mother track index
    Int_t           fVolNb;           ///< number of fiber in pixel
    Double_t        fMass;            ///< mass
    Double_t        fLightYield;      ///< light yield
    Double32_t      fTimeIn;          ///< point start time
    Double32_t      fTimeOut;         ///< point finish time
    Double32_t      fTrackLength;     ///< track length from his origin
    Double32_t      fELoss;           ///< energy loss
    TLorentzVector  fPosIn;           ///< point start  position
    TLorentzVector  fPosOut;          ///< point finish  position
    TLorentzVector  fCurPosIn;        ///< current step position
    TLorentzVector  fMomIn;           ///< point start momentum
    TLorentzVector  fMomOut;          ///< point start momentum
    TLorentzVector  fCurMomIn;        ///< current step momentum
    TVector3        fPosInLocal;      ///< point position in sensetive volume CS
    TArrayI         fProcessesID;     ///< VMC prcess IDs in step

    ClassDef(HERODetector,1)
};

#endif /* HERODetector_H */

