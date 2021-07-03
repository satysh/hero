void geo()
{
    Double_t Scint_dz = 0.9;
    Double_t Metal_dz = 0.1;
    Int_t Num_Layers = 125.;

    // -------   Load media from media file   -----------------------------------
    FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
    FairGeoInterface* geoFace = geoLoad->getGeoInterface();
    TString geoPath = gSystem->Getenv("VMCWORKDIR");
    TString medFile = geoPath + "/geometry/media.geo";
    geoFace->setMediaFile(medFile);
    geoFace->readMedia();
    // --------------------------------------------------------------------------

    // -------   Geometry file name (output)   ----------------------------------
    TString geoFileName = geoPath + "/geometry/HERO_detector_Sphear.geo.root";
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

    FairGeoMedium    * air  = geoMedia->getMedium("air");
    if ( ! air  ) Fatal("Main", "FairMedium air   not found");
    geoBuild->createMedium(air);
    TGeoMedium* Air = gGeoManager->GetMedium("air");
    if ( !  Air ) Fatal("Main", "Medium  Air not found");

    FairGeoMedium    * SCint  = geoMedia->getMedium("FscScintVB");
    if ( ! SCint ) Fatal("Main", "FairMedium FscScintVB not found");
    geoBuild->createMedium(SCint);
    TGeoMedium* Scint = gGeoManager->GetMedium("FscScintVB");
    if ( ! Scint ) Fatal("Main", "Medium FscScintVB not found");

//     FairGeoMedium    * Aluminum  = geoMedia->getMedium("aluminium");
//     if ( ! Aluminum ) Fatal("Main", "FairMedium Aluminum not found");
//     geoBuild->createMedium(Aluminum);
//     TGeoMedium* aluminum = gGeoManager->GetMedium("aluminium");
//     if ( ! aluminum ) Fatal("Main", "Medium aluminum not found");
//
//       FairGeoMedium    * LEad  = geoMedia->getMedium("lead");
//   if ( ! LEad ) Fatal("Main", "FairMedium LEad not found");
//   geoBuild->createMedium(LEad);
//   TGeoMedium* Lead = gGeoManager->GetMedium("lead");
//   if ( ! Lead ) Fatal("Main", "Medium Lead not found");


    TGeoVolume* vAir = gGeoManager->MakeSphere("vAir", Air, 0, Num_Layers, 0.,180.,0.,0.);
    vAir ->SetFillColor(kGreen);
    vAir ->SetLineColor(kGreen);
    vAir ->SetTransparency(60);

    TGeoVolume* vAir_Sphere = gGeoManager->MakeSphere("vAir_Sphere", Scint, 0, Num_Layers, 0.,180.,0.,0.);
    vAir_Sphere ->SetFillColor(kGreen);
    vAir_Sphere ->SetLineColor(kGreen);
    vAir_Sphere ->SetTransparency(60);
    vAir->AddNode(vAir_Sphere, 1);

//     TGeoVolume* vScint_Sphere_[Num_Layers], *vMetal_Sphere_[Num_Layers];
//     // Shapes and volumes ---------------------------------------------------------------
//     for(int i=1 ; i< Num_Layers ; i++){
// //         int j=i-1;
// //         vScint_Sphere_[i] = gGeoManager->MakeSphere(Form("vScint_%i", i), Scint, 126-i*(Scint_dz+Metal_dz)-Scint_dz, 126-i*(Scint_dz+Metal_dz), 0.,180.,0.,0.);
// //         vScint_Sphere_[i] ->SetFillColor(kGreen);
// //         vScint_Sphere_[i] ->SetLineColor(kGreen);
// //         vScint_Sphere_[i] ->SetTransparency(60);
// //         vAir_Sphere ->AddNode(vScint_Sphere_[i],1);
//
//         vMetal_Sphere_[i] = gGeoManager->MakeSphere(Form("vMetal_%i", i), Lead ,Num_Layers-i*(Scint_dz + Metal_dz), Num_Layers+1-i*(Scint_dz+Metal_dz)-Scint_dz, 0.,180.,0.,0.);
//         vMetal_Sphere_[i] ->SetFillColor(kRed);
//         vMetal_Sphere_[i] ->SetLineColor(kRed);
//         vMetal_Sphere_[i] ->SetTransparency(60);
//         vAir_Sphere ->AddNode(vMetal_Sphere_[i],1);
//
//     }

//     TGeoVolume*  vScint_Sphere = gGeoManager->MakeSphere("vScint", Scint, 0, 125, 0.,180.,0.,0.);

//     vAir_Sphere ->AddNode(vScint_Sphere,1);
    // Container for 1 detector
    TGeoVolumeAssembly* vDetContainer = new TGeoVolumeAssembly("vDetContainer");

    // Volume hierarchy -----------------------------------------------------------------
    vDetContainer->AddNode(vAir, 1);

    top->AddNode(vDetContainer, 1);

    // ---------------   Finish   -----------------------------------------------
    gGeoManager->SetTopVolume(top);
    gGeoManager->CloseGeometry();
    gGeoManager->CheckOverlaps(0.001);
    gGeoManager->PrintOverlaps();
    gGeoManager->CheckGeometry();
//     gGeoManager->CheckGeometryFull();

    gGeoManager->GetTopVolume()->Draw("ogl");

    TFile* geoFile = new TFile(geoFileName, "RECREATE");
    top->Write();
    geoFile->Close();
    // --------------------------------------------------------------------------
}
