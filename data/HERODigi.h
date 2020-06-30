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
  HERODigi(Int_t id, Float_t Edep, Double_t time, Int_t volNb);

  /** Copy constructor **/
  HERODigi(const HERODigi&);

  /** Destructor **/
  virtual ~HERODigi();

  HERODigi& operator=(const HERODigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /* Accessors */
  Int_t ID () const {return fID;}
  Float_t Edep() const {return fEdep;}

 protected:
  Int_t     fID;

  Int_t     fVolNb;
  Double_t  fTime;
  Float_t   fEdep;

  ClassDef(HERODigi,1)

};

#endif
