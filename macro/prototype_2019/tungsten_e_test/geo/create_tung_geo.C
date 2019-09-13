void create_tung_geo()
{
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
  TString geoFileName = geoPath + "/geometry/geo_e_t.root";
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
  FairGeoMedium    * vacM  = geoMedia->getMedium("vacuum2");
  if ( ! vacM ) Fatal("Main", "FairMedium vacuum2 not found");
  geoBuild->createMedium(vacM);
  TGeoMedium* VacM = gGeoManager->GetMedium("vacuum2");
  if ( ! VacM ) Fatal("Main", "Medium vacuum2 not found");

  //------------------  Material for calorimetr   --------------
  FairGeoMedium    * tungM  = geoMedia->getMedium("tungsten");
  if ( ! tungM  ) Fatal("Main", "FairMedium tungsten not found");
  geoBuild->createMedium(tungM);
  TGeoMedium* TungM = gGeoManager->GetMedium("tungsten");
  if ( !  TungM ) Fatal("Main", "Medium  TungM not found");

  Double_t Dim = 50.;
  Double_t dZ = 10.;

  //------------------  Material for SIZ  --------------
  TGeoVolume* tungstenVol = gGeoManager->MakeTube("tungstenVol", TungM, 0., 0.5*Dim, 0.5*dZ);


  // Container
  TGeoVolume* DetectorContainer = gGeoManager->MakeBox("DetectorContainer", VacM, 50., 50., 100.);
  /*
  DetectorContainer->SetFillColor(0);
  DetectorContainer->SetLineColor(kYellow);
  DetectorContainer->SetTransparency(60);*/

  // Volumes hierarchical

  DetectorContainer->AddNode(tungstenVol, 1/*, new TGeoTranslation(0., 0., -(Calor_Size+10.1+ chmp.dz)*0.5)*/);

  top->AddNode(DetectorContainer, 1);

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















