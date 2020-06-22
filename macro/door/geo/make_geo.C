void make_geo()
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
  TString geoFileName = geoPath + "/geometry/door.root";
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
  if ( ! SCint ) Fatal("Main", "FairMedium FscScintVB not found");
  geoBuild->createMedium(SCint);
  TGeoMedium* Scint = gGeoManager->GetMedium("FscScintVB");
  if ( ! Scint ) Fatal("Main", "Medium FscScintVB not found");

  //------------------  Material for calorimetr   --------------
  FairGeoMedium* air  = geoMedia->getMedium("air");
  if ( ! air  ) Fatal("Main", "FairMedium air   not found");
  geoBuild->createMedium(air);
  TGeoMedium* Air = gGeoManager->GetMedium("air");
  if ( !  Air ) Fatal("Main", "Medium  Air not found");


  //-------------------  Logical Volumes  ----------------
  // Calorimetr
  TGeoVolume* vDoor = gGeoManager->MakeBox("vDoor", Scint, 0.5*100., 0.5*200., 0.5*20.);
  vDoor->SetFillColor(kGreen);
  vDoor->SetLineColor(kGreen);

  TGeoVolume* vAir = gGeoManager->MakeBox("vAir", Air, 0.5*(100.+1.), 0.5*(200.+1.), 0.5*(20.+ 1.));


  // Volumes hierarchical
  vAir->AddNode(vDoor, 1);

  top->AddNode(vAir, 1);

  // ---------------   Finish   -----------------------------------------------
  gGeoManager->SetTopVolume(top);
  gGeoManager->CloseGeometry();
  gGeoManager->CheckOverlaps(0.001);
  //gGeoManager->PrintOverlaps();
  //gGeoManager->CheckGeometry();
  //gGeoManager->CheckGeometryFull();

  //gGeoManager->GetTopVolume()->Draw("ogl");

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
}















