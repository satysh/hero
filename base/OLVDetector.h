/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed und the tms of the             *
 *         GNU Less Genal Public Licence vsion 3 (LGPL) vsion 3,        *
 *                  copied vbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef OLVDetector_H
#define OLVDetector_H

// ROOT
#include "TLorentzVector.h"
#include "TClonesArray.h"

// FairRoot
#include "FairDetector.h"

// OLV
#include "OLVPoint.h"

class OLVDetector : public FairDetector
{

public:

    OLVDetector();

    OLVDetector(const char* Name, Bool_t Active, Int_t DetId=0);
    virtual ~OLVDetector() {;}
    Int_t GetParticlePDG() { return fParticlePDG; }
    TClonesArray* GetCollection(Int_t iColl) const;
    void SetParticlePDG(Int_t pdg_int = 0) { fParticlePDG = pdg_int; }
    void ConstructGeometry();
    void AddSensetive(TString name);
    void Initialize();
    void Register();
    void EndOfEvent();
    void Print(Option_t *option="") const;
    void Reset();
    void CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset);


    virtual Bool_t ProcessHits(FairVolume* vol = 0);
    Bool_t CheckIfSensitive(std::string name);
private:
    void StartNewPoint();
    void FinishNewPoint();
    OLVPoint* AddPoint(TClonesArray* points);

    Int_t fParticlePDG;
    Int_t fNutronsNum;
    ClassDef(OLVDetector,1)

    //map of sensetive volumes points collection collection, fSenVolumes[senVol] = points
	std::map<TString,TClonesArray*> fSenVolumes;

    std::vector<TString> fSenNames;   ///< Sensetive volumes sustring, that user set 

	Int_t   fVerbose;                 ///< Verbosity level                

    Float_t fFullEnergy;              ///< Sum Edep in event in sensetive volume
    Float_t fFullLY;                  ///< Sum Light Yield in event in sensetive volume

    /* current step information*/
    Int_t fEventID;                   ///< event index
    Int_t fTrackID;                   ///< track index
    Int_t fMot0TrackID;               ///< 0th mother track index
    Double_t fMass;                   ///< mass
    TLorentzVector fPosIn;            ///< point start  position
    TLorentzVector fPosOut;           ///< point finish  position
    TLorentzVector fCurPosIn;         ///< current step position
    TVector3 fPosInLocal;             ///< point position in sensetive volume CS
    TLorentzVector fMomIn;            ///< point start momentum
    TLorentzVector fMomOut;           ///< point start momentum
    TLorentzVector fCurMomIn;         ///< current step momentum
    Double32_t     fTimeIn;           ///< point start time
    Double32_t     fTimeOut;          ///< point finish time
    Double32_t     fTrackLength;      ///< track length from his origin
    Double32_t     fELoss;            ///< energy loss
    Double_t       fLightYield;       ///< light yield
    Int_t          fVolNb;          ///< number of fiber in pixel
    TArrayI  fProcessesID;            ///< VMC prcess IDs in step
};

#endif /* OLVDetector_H */

