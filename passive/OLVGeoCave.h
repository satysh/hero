#ifndef OLVGEOCAVE_H
#define OLVGEOCAVE_H

#include "FairGeoSet.h"
#include "TString.h"
#include "FairGeoMedia.h"
#include <fstream>
class  OLVGeoCave : public FairGeoSet {
protected:
  TString name;
public:
  OLVGeoCave();
  ~OLVGeoCave() {}
  const char* getModuleName(Int_t) {return name.Data();}  
  Bool_t read(std::fstream&,FairGeoMedia*);
  void addRefNodes();
  void write(std::fstream&);
  void print();
  ClassDef(OLVGeoCave,0) // Class for the geometry of CAVE
};

#endif  //OLVGEOCAVE_H
