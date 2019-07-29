void geo()
{
  TGeoRotation* ggZeroRotation = new TGeoRotation("ggZeroRotation");

  // -------   Load media from media file   -----------------------------------
  FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  // --------------------------------------------------------------------------
  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/geometry/time_test_geo.root";
  // --------------------------------------------------------------------------
  // --------------   Create geometry and top volume  -------------------------
  gGeoManager = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoManager->SetName("DETgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoManager->SetTopVolume(top);
  // --------------------------------------------------------------------------
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  //------------------   My Material for Plate_B10 need work from media file --------------
  FairGeoMedium* vac = geoMedia->getMedium("vacuum");
  if (!vac) Fatal("Main", "FairMedium vac is not found");
  geoBuild->createMedium(vac);
  TGeoMedium* Vac = gGeoManager->GetMedium("vacuum");
  if (!Vac) Fatal("Main", "Medium Vac is not found");

  FairGeoMedium* air  = geoMedia->getMedium("air");
  if (!air) Fatal("Main", "FairMedium air is not found");
  geoBuild->createMedium(air);
  TGeoMedium* Air = gGeoManager->GetMedium("air");
  if (!Air ) Fatal("Main", "Medium  Air is not found");

  FairGeoMedium* mylar  = geoMedia->getMedium("mylar");
  if (!mylar) Fatal("Main", "FairMedium mylar is not found");
  geoBuild->createMedium(mylar);
  TGeoMedium* Mylar = gGeoManager->GetMedium("mylar");
  if (!Mylar ) Fatal("Main", "Medium  Mylar is not found");

  TGeoVolume* box0 = gGeoManager->MakeBox("box0", Vac, 100., 100., 100.);
  Double_t sizeX = 10.;
  Double_t sizeZ = 0.0001;
  Double_t dS = 10.;
  TGeoVolume* box1 = gGeoManager->MakeBox("box1", Vac, 0.5*sizeX, 0.5*sizeX, 0.5*sizeZ);
  TGeoVolume* box2 = gGeoManager->MakeBox("box2", Vac, 0.5*sizeX, 0.5*sizeX, 0.5*sizeZ);

  box0->AddNode(box1, 1, new TGeoTranslation(0., 0., 0.5*sizeZ + dS));
  box0->AddNode(box2, 1, new TGeoTranslation(0., 0., sizeZ + 2.*dS));
  top->AddNode(box0, 1);

  // ---------------   Finish   -----------------------------------------------
  gGeoManager->SetTopVolume(top);
  gGeoManager->CloseGeometry();
  gGeoManager->CheckOverlaps(0.001);
  //gGeoManager->PrintOverlaps();
  gGeoManager->CheckGeometry();
  //gGeoManager->CheckGeometryFull();

  //gGeoManager->GetTopVolume()->Draw("ogl");

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
}
