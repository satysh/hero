#include "HERODigi.h"
#include<iostream>

// -----   Default constructor   -------------------------------------------
HERODigi::HERODigi()
{
}

HERODigi::HERODigi(Int_t id, Double_t Edep, Double_t lightY, Double_t time, Int_t volNb)
  :fPID(id),
  fEdep(Edep),
  fLightYield(lightY),
  fTime(time),
  fVolNb(volNb)
{
}

HERODigi::HERODigi(const HERODigi& right)
  :fPID(right.fPID),
  fEdep(right.fEdep),
  fLightYield(right.fLightYield),
  fTime(right.fTime),
  fVolNb(right.fVolNb)
{
}

// -----   Destructor   ----------------------------------------------------
HERODigi::~HERODigi()
{
}

// -----   Public method Print   -------------------------------------------
void HERODigi::Print(const Option_t* opt /* = 0 */) const
{
  std::cout << "-I- HERODigi:  " << std::endl;
  std::cout << "    Edep : " << fEdep << std::endl;
  std::cout << "    LightYield : " << fLightYield << std::endl;
}
// -------------------------------------------------------------------------

ClassImp(HERODigi)
