void geo()
{
  // -------   Load media from media file   -----------------------------------
  FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  // --------------------------------------------------------------------------

  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/geometry/rogov_geo.root";
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



  FairGeoMedium    * vacuum  = geoMedia->getMedium("vacuum");
  if ( ! vacuum  ) Fatal("Main", "FairMedium vacuum   not found");
  geoBuild->createMedium(vacuum);
  TGeoMedium* Avacuum = gGeoManager->GetMedium("vacuum");
  if ( !  Avacuum ) Fatal("Main", "Medium  Avacuum not found");

  FairGeoMedium    * Iron  = geoMedia->getMedium("iron");
  if ( ! Iron ) Fatal("Main", "FairMedium iron not found");
  geoBuild->createMedium(Iron);
  TGeoMedium* iron = gGeoManager->GetMedium("iron");
  if ( ! iron ) Fatal("Main", "Medium iron not found");

  FairGeoMedium    * Aluminum  = geoMedia->getMedium("aluminium");
  if ( ! Aluminum ) Fatal("Main", "FairMedium Aluminum not found");
  geoBuild->createMedium(Aluminum);
  TGeoMedium* aluminum = gGeoManager->GetMedium("aluminium");
  if ( ! aluminum ) Fatal("Main", "Medium aluminum not found");


  TGeoVolume* vMed = gGeoManager->MakeTube("vMed", iron, 0., 10., 0.5*25.); // name, medium, rmin, rmax, dz/2

  TGeoVolumeAssembly* vDetContainer = new TGeoVolumeAssembly("vDetContainer");

  // Volume hierarchy -----------------------------------------------------------------
  vDetContainer->AddNode(vMed, 1);

  top->AddNode(vDetContainer, 1);

  // ---------------   Finish   -----------------------------------------------
  gGeoManager->SetTopVolume(top);
  gGeoManager->CloseGeometry();
  gGeoManager->CheckOverlaps(0.001);
  gGeoManager->PrintOverlaps();
  gGeoManager->CheckGeometry();

  //gGeoManager->GetTopVolume()->Draw("ogl");

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
  // --------------------------------------------------------------------------
}
