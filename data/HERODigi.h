#ifndef HERODigi_H
#define HERODigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

class HERODigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  HERODigi();

  /** Constructor with arguments
   **/
  HERODigi(Int_t pid, Double_t Edep, Double_t lightY, Double_t time, Int_t volNb);

  /** Copy constructor **/
  HERODigi(const HERODigi&);

  /** Destructor **/
  virtual ~HERODigi();

  HERODigi& operator=(const HERODigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /* Accessors */
  Int_t ID () const {return fPID;}
  Double_t Edep() const {return fEdep;}

 protected:
  Int_t     fPID=-1;
  Double_t  fEdep=0.;
  Double_t  fLightYield=0.;
  Double_t  fTime=0.;
  Int_t     fVolNb=0;

  ClassDef(HERODigi, 1)

};

#endif
