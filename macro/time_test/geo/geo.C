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
  FairGeoMedium* SCint  = geoMedia->getMedium("FscScintVB");
  if (!SCint) Fatal("Main", "FairMedium FscScintVB is not found");
  geoBuild->createMedium(SCint);
  TGeoMedium* Scint = gGeoManager->GetMedium("FscScintVB");
  if (!Scint) Fatal("Main", "Medium FscScintVB is not found");

  FairGeoMedium* air  = geoMedia->getMedium("air");
  if (!air) Fatal("Main", "FairMedium air is not found");
  geoBuild->createMedium(air);
  TGeoMedium* Air = gGeoManager->GetMedium("air");
  if (!Air ) Fatal("Main", "Medium  Air is not found");

  FairGeoMedium* vac = geoMedia->getMedium("vacuum");
  if (!vac) Fatal("Main", "FairMedium vac is not found");
  geoBuild->createMedium(vac);
  TGeoMedium* Vac = gGeoManager->GetMedium("vacuum");
  if (!Vac) Fatal("Main", "Medium Vac is not found");

  TGeoVolume* box0 = gGeoManager->MakeBox("box0", Vac, 100., 100., 100.);
  Double_t sizeZ = 1.;
  TGeoVolume* box1 = gGeoManager->MakeBox("box1", Vac, 0.5*sizeZ, 0.5*sizeZ, 0.5*sizeZ);

  box0->AddNode(box1, 1);
  top->AddNode(box0, 1);

  // ---------------   Finish   -----------------------------------------------
  gGeoManager->SetTopVolume(top);
  gGeoManager->CloseGeometry();
  gGeoManager->CheckOverlaps(0.001);
  //gGeoManager->PrintOverlaps();
  gGeoManager->CheckGeometry();
  //gGeoManager->CheckGeometryFull();

  gGeoManager->GetTopVolume()->Draw("ogl");
/*
  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
*/
}
