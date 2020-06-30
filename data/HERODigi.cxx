#include "HERODigi.h"
#include<iostream>

using namespace std;

// -----   Default constructor   -------------------------------------------
HERODigi::HERODigi()
  :fID(-1),
  fEdep(0.)
{
}

HERODigi::HERODigi(Int_t id, Float_t Edep, Double_t time,Int_t volNb)
  :fID(id),
  fEdep(Edep),
  fTime(time),
  fVolNb(volNb)
{
}

HERODigi::HERODigi(const HERODigi& right)
  :fID(right.fID),
  fEdep(right.fEdep),
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
  std::cout << "-I- HERODigi:  " << endl;
  std::cout << "    Edep : " << fEdep << endl;
}
// -------------------------------------------------------------------------

ClassImp(HERODigi)
