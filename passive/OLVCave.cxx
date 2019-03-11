#include "FairMCApplication.h"
#include "OLVCave.h"
#include "FairVolume.h"
#include "FairVolumeList.h"
#include "TGeoMCGeometry.h"
#include "TGeoManager.h"
#include "TGeoVolume.h"
#include "TGeoMedium.h"

#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairGeoRootBuilder.h"
#include "FairRuntimeDb.h"
#include "TObjArray.h"
#include "FairGeoVolume.h"
#include "FairGeoNode.h"
#include "FairRun.h"

#include "OLVGeoCave.h"
//#include "ERGeoPassivePar.h"


ClassImp(OLVCave)
void OLVCave::ConstructGeometry()
{
	FairGeoLoader *loader=FairGeoLoader::Instance();
	FairGeoInterface *GeoInterface =loader->getGeoInterface();
	OLVGeoCave *MGeo=new OLVGeoCave();
	MGeo->setGeomFile(GetGeometryFileName());
	GeoInterface->addGeoModule(MGeo);
	Bool_t rc = GeoInterface->readSet(MGeo);
	if ( rc ) MGeo->create(loader->getGeoBuilder());

    TList* volList = MGeo->getListOfVolumes();
    // store geo parameter

}

OLVCave::OLVCave(){
}

OLVCave::~OLVCave(){
}


OLVCave::OLVCave(const char * name,  const char *Title)
  : FairModule(name ,Title)
{
    world[0] = 0;
    world[1] = 0;
    world[2] = 0;
}
