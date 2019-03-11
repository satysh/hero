#ifndef OLVCAVE_H
#define OLVCAVE_H

#include "FairDetector.h"
#include "FairModule.h"


class OLVCave : public FairModule {
  public:
	OLVCave(const char * name, const char *Title="Expert Cave");
	OLVCave();
    virtual ~OLVCave();
    virtual void ConstructGeometry();
private:
     Double_t world[3]; //!
     ClassDef(OLVCave,1)
};

#endif //OLVCAVE_H

