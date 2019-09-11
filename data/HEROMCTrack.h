#ifndef HEROMCTRACK_H_
#define HEROMCTRACK_H_

// ROOT
#include "TObject.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TParticle.h"

class HEROMCTrack : public TObject
{

 public:

  /**  Default constructor  **/
  HEROMCTrack();


  /**  Standard constructor  **/
  HEROMCTrack(Int_t pdgCode, Int_t motherID, Double_t px, Double_t py,
       Double_t pz, Double_t x, Double_t y, Double_t z,
       Double_t t, Int_t nPoints);

  /**  Copy constructor  **/
  HEROMCTrack(const HEROMCTrack& track);


  /**  Constructor from TParticle  **/
  HEROMCTrack(TParticle* particle);


  /**  Destructor  **/
  virtual ~HEROMCTrack();


  /**  Output to screen  **/
  void Print(Int_t iTrack=0) const;

  /**  Accessors  **/

  Int_t    GetPdgCode()   const { return fPdgCode; }
  Int_t    GetMotherId()  const { return fMotherId; }
  Double_t GetPx()        const { return fPx; }
  Double_t GetPy()        const { return fPy; }
  Double_t GetPz()        const { return fPz; }
  Double_t GetStartX()    const { return fStartX; }
  Double_t GetStartY()    const { return fStartY; }
  Double_t GetStartZ()    const { return fStartZ; }
  Double_t GetStartT()    const { return fStartT; }
  Double_t GetMass()      const;
  Double_t GetCharge()    const;
  Double_t GetEnergy()    const;
  Double_t GetPt()        const { return TMath::Sqrt(fPx*fPx+fPy*fPy); }
  Double_t GetP()         const { return TMath::Sqrt(fPx*fPx+fPy*fPy+fPz*fPz); }
  Double_t GetRapidity()  const;
  Double_t GetTheta();
  Double_t GetPhi();

  void GetMomentum(TVector3& momentum) const;
  void Get4Momentum(TLorentzVector& momentum) const;
  void GetStartVertex(TVector3& vertex) const;

  TLorentzVector GetVector();

  /** Accessors to the number of MCPoints in the detectors **/
  Int_t GetNPoints(Int_t detId)  const;

  /**  Modifiers  **/
  void SetMotherId(Int_t id) { fMotherId = id; }
  void SetNPoints(Int_t iDet, Int_t np);

private:
  Double_t CalculateMass();
  Double_t CalculateEnergy();

  Int_t fID;
  Int_t fPdgCode;  // PDG particle code
  Int_t fMotherId; // Index of mother track. -1 for primary particles.
  Int_t fNPoints;

  Double32_t fPx, fPy, fPz; // Momentum components at start vertex [GeV]
  Double32_t fStartX, fStartY, fStartZ, fStartT; // Coordinates of start vertex [cm, ns]

  Double_t  fMass;
  Double_t  fEnergy;
  Double_t  fTheta,fPhi;

  TLorentzVector fMomentum;

  ClassDef(HEROMCTrack,2);
};

#endif /* HEROMCTRACK_H_ */
