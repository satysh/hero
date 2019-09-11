void geo_solid()
{
  Double_t dx = 150.;
  Double_t dy = 150.;
  Double_t dz = 150.;
  // Create a zero rotation
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
  TString geoFileName = geoPath + "/geometry/HERO_detector.geo.root";
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

  FairGeoMedium    * SCint  = geoMedia->getMedium("FscScintVB");
  if ( ! SCint ) Fatal("Main", "FairMedium FscScintVB not found");
  geoBuild->createMedium(SCint);
  TGeoMedium* Scint = gGeoManager->GetMedium("FscScintVB");
  if ( ! Scint ) Fatal("Main", "Medium FscScintVB not found");


  // Shapes and volumes ---------------------------------------------------------------

  TGeoVolume* vCub = gGeoManager->MakeBox("vCub", Scint, dx, dy, dz);

  // Container for 1 detector
  TGeoVolumeAssembly* vDetContainer = new TGeoVolumeAssembly("vDetContainer");

  // Volume hierarchy -----------------------------------------------------------------
  vDetContainer->AddNode(vCub, 1);

  top->AddNode(vDetContainer, 1);

  // ---------------   Finish   -----------------------------------------------
  gGeoManager->SetTopVolume(top);
  gGeoManager->CloseGeometry();
  gGeoManager->CheckOverlaps(0.001);
  gGeoManager->PrintOverlaps();
  gGeoManager->CheckGeometry();
  //gGeoManager->CheckGeometryFull();

  //gGeoManager->GetTopVolume()->Draw("ogl");

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
  // --------------------------------------------------------------------------
}

