// ROOT
#include "TObjArray.h"

// FAIRROOT
#include "FairParamList.h"

// OLV
#include "OLVDetectorGeoPar.h"

OLVDetectorGeoPar ::OLVDetectorGeoPar(const char* name, const char* title, const char* context)
  : FairParGenericSet(name,title,context),
    fGeoSensNodes(new TObjArray()),
    fGeoPassNodes(new TObjArray())
{
}

OLVDetectorGeoPar::~OLVDetectorGeoPar(void)
{
}

void OLVDetectorGeoPar::clear(void)
{
  if(fGeoSensNodes) 
    delete fGeoSensNodes; 
  if(fGeoPassNodes) 
    delete fGeoPassNodes; 
}

void OLVDetectorGeoPar::putParams(FairParamList* l)
{
  if (!l) 
    return; 
  l->addObject("OLVGeoNodes Sensitive List", fGeoSensNodes);
  l->addObject("OLVGeoNodes Passive List", fGeoPassNodes);
}

Bool_t OLVDetectorGeoPar::getParams(FairParamList* l)
{
  if (!l) 
    return kFALSE; 
  if (!l->fillObject("OLVGeoNodes Sensitive List", fGeoSensNodes))
    return kFALSE; 
  if (!l->fillObject("OLVGeoNodes Passive List", fGeoPassNodes)) 
    return kFALSE;

  return kTRUE;
}

ClassImp(OLVDetectorGeoPar)