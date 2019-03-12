#ifndef OLVDetectorGeoPar_H
#define OLVDetectorGeoPar_H

// FAIRROOT
#include "FairParGenericSet.h"

class TObjArray;
class FairParamList;

class OLVDetectorGeoPar : public FairParGenericSet
{
  public:

    /** List of OLVGeoNodes for sensitive  volumes */
    TObjArray*      fGeoSensNodes;

    /** List of OLVGeoNodes for sensitive  volumes */
    TObjArray*      fGeoPassNodes;

    OLVDetectorGeoPar(const char* name="OLVDetectorGeoPar",
                           const char* title="Detector Geometry Parameters",
                           const char* context="TestDefaultContext");
    ~OLVDetectorGeoPar(void);
    void clear(void);
    void putParams(FairParamList*);
    Bool_t getParams(FairParamList*);
    TObjArray* GetGeoSensitiveNodes() {return fGeoSensNodes;}
    TObjArray* GetGeoPassiveNodes()   {return fGeoPassNodes;}

  private:
    OLVDetectorGeoPar(const OLVDetectorGeoPar&);
    OLVDetectorGeoPar& operator=(const OLVDetectorGeoPar&);

    ClassDef(OLVDetectorGeoPar,1)
};

#endif /* OLVDetectorGeoPar_H */
