void Prototape_ECAL_whith_Boron_film()
{
    typedef struct{
        Double_t dx = 12.;
        Double_t dy = 12.;
        Double_t Scint_z = 0.15;
        Double_t Lead_z=0.08;
	    Double_t Boron_film_z=6*10e-4;
        Double_t  NLayer=26./(Lead_z+Scint_z+Boron_film_z);
    } Calorimetr;
    typedef struct{
        Double_t dx = 10.;
        Double_t dy = 0.5;
        Double_t dz = 12.;
    } Plate_B10;
    typedef struct{
        Double_t dx = 2.;
        Double_t dy = 2.;
        Double_t dz = 1.;
    } SIZ;
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
    TString geoFileName = geoPath + "/geometry/OLV_Prototyp_ECAL_whith_Boron_film.root";
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
    FairGeoMedium    * SCint  = geoMedia->getMedium("FscScintVB");
    if ( ! SCint ) Fatal("Main", "FairMedium FscScintVB not found");
    geoBuild->createMedium(SCint);
    TGeoMedium* Scint = gGeoManager->GetMedium("FscScintVB");
    if ( ! Scint ) Fatal("Main", "Medium FscScintVB not found");

    //------------------  Material for calorimetr   --------------
     FairGeoMedium    * air  = geoMedia->getMedium("air");
    if ( ! air  ) Fatal("Main", "FairMedium air   not found");
    geoBuild->createMedium(air);
    TGeoMedium* Air = gGeoManager->GetMedium("air");
    if ( !  Air ) Fatal("Main", "Medium  Air not found");

      FairGeoMedium    * LEad  = geoMedia->getMedium("lead");
    if ( ! LEad ) Fatal("Main", "FairMedium LEad not found");
    geoBuild->createMedium(LEad);
    TGeoMedium* Lead = gGeoManager->GetMedium("lead");
    if ( ! Lead ) Fatal("Main", "Medium Lead not found");

         FairGeoMedium    * POlysterol  = geoMedia->getMedium("FscScint");
    if ( ! POlysterol ) Fatal("Main", "FairMedium POlysterol not found");
    geoBuild->createMedium(POlysterol);
    TGeoMedium* Polysterol = gGeoManager->GetMedium("FscScint");
    if ( ! Polysterol ) Fatal("Main", "Medium FscScint not found");

      FairGeoMedium    * BOron  = geoMedia->getMedium("HYPboron");
    if ( ! BOron ) Fatal("Main", "FairMedium BOron not found");
    geoBuild->createMedium(BOron);
    TGeoMedium* boron = gGeoManager->GetMedium("HYPboron");
    if ( ! boron ) Fatal("Main", "Medium boron not found");

    //------------------  Material for SIZ  --------------
    Calorimetr calor;
    Plate_B10 plate;
    SIZ  chmp;
    Double_t Layer_Size = calor.Scint_z + calor.Lead_z+calor.Boron_film_z; // 0.15 + 0.08 + 6*10^(-4) = 0.23
    Double_t Calor_Size = calor.NLayer * Layer_Size; // 26. cm

    //-------------------  Logical Volumes  ----------------
    // Calorimetr
    TGeoVolume* vCalorimetr = gGeoManager->MakeBox("vCalorimetr", Air, 0.5*calor.dx, 0.5*calor.dy, 0.5*Calor_Size);
    vCalorimetr->SetFillColor(kGreen);
    vCalorimetr->SetLineColor(kGreen);
    vCalorimetr->SetTransparency(60);


    TGeoVolume* layer = gGeoManager->MakeBox("layer", Air, 0.5*calor.dx, 0.5*calor.dy, 0.5*(Layer_Size));

    // Calorimetr scitilator
    TGeoVolume* vCalorimetrScint = gGeoManager->MakeBox("vCalorimetrScint", Polysterol, 0.5*calor.dx, 0.5*calor.dy, 0.5*calor.Scint_z);
    vCalorimetrScint->SetFillColor(kRed);
    vCalorimetrScint->SetLineColor(kRed);
    vCalorimetrScint->SetTransparency(60);

    // Calorimetr lead
    TGeoVolume* vCalorimetrLead = gGeoManager->MakeBox("vCalorimetrLead", Lead, 0.5*calor.dx, 0.5*calor.dy, 0.5*calor.Lead_z);
    vCalorimetrLead->SetFillColor(kBlue);
    vCalorimetrLead->SetLineColor(kBlue);
    vCalorimetrLead->SetTransparency(60);

    // Calorimetr Boron film
    TGeoVolume* vCalorimetrFilm = gGeoManager->MakeBox("vCalorimetrFilm", boron, 0.5*calor.dx, 0.5*calor.dy, 0.5*calor.Boron_film_z);
    vCalorimetrFilm->SetFillColor(kRed);
    vCalorimetrFilm->SetLineColor(kRed);
    vCalorimetrFilm->SetTransparency(60);

    // u is up, d is down, f is front, b is back in the code below
    // --- 10B Plates XYZ -------------------------------
    TGeoVolume* vPlate_B10_xyz_u_f = gGeoManager->MakeBox("vPlate_B10_xyz_u_f", Scint, 0.5*plate.dx, 0.5*plate.dy, 0.5*plate.dz);
    vPlate_B10_xyz_u_f->SetFillColor(kGreen);
    vPlate_B10_xyz_u_f->SetLineColor(kGreen);
    vPlate_B10_xyz_u_f->SetTransparency(60);

    TGeoVolume* vPlate_B10_xyz_u_b = gGeoManager->MakeBox("vPlate_B10_xyz_u_b", Scint, 0.5*plate.dx, 0.5*plate.dy, 0.5*plate.dz);
    vPlate_B10_xyz_u_b->SetFillColor(kGreen);
    vPlate_B10_xyz_u_b->SetLineColor(kGreen);
    vPlate_B10_xyz_u_b->SetTransparency(60);

    TGeoVolume* vPlate_B10_xyz_d_f = gGeoManager->MakeBox("vPlate_B10_xyz_d_f", Scint, 0.5*plate.dx, 0.5*plate.dy, 0.5*plate.dz);
    vPlate_B10_xyz_d_f->SetFillColor(kGreen);
    vPlate_B10_xyz_d_f->SetLineColor(kGreen);
    vPlate_B10_xyz_d_f->SetTransparency(60);

    TGeoVolume* vPlate_B10_xyz_d_b = gGeoManager->MakeBox("vPlate_B10_xyz_d_b", Scint, 0.5*plate.dx, 0.5*plate.dy, 0.5*plate.dz);
    vPlate_B10_xyz_d_b->SetFillColor(kGreen);
    vPlate_B10_xyz_d_b->SetLineColor(kGreen);
    vPlate_B10_xyz_d_b->SetTransparency(60);
    //-----------------------------------------------------

    // l - left, r - right in the code below
    // --- 10B Plates YXZ -------------------------------
    TGeoVolume* vPlate_B10_yxz_r_f = gGeoManager->MakeBox("vPlate_B10_yxz_r_f", Scint, 0.5*plate.dy, 0.5*plate.dx, 0.5*plate.dz);
    vPlate_B10_yxz_r_f->SetFillColor(kGreen);
    vPlate_B10_yxz_r_f->SetLineColor(kGreen);
    vPlate_B10_yxz_r_f->SetTransparency(60);

    TGeoVolume* vPlate_B10_yxz_r_b = gGeoManager->MakeBox("vPlate_B10_yxz_r_b", Scint, 0.5*plate.dy, 0.5*plate.dx, 0.5*plate.dz);
    vPlate_B10_yxz_r_b->SetFillColor(kGreen);
    vPlate_B10_yxz_r_b->SetLineColor(kGreen);
    vPlate_B10_yxz_r_b->SetTransparency(60);


    TGeoVolume* vPlate_B10_yxz_l_f = gGeoManager->MakeBox("vPlate_B10_yxz_l_f", Scint, 0.5*plate.dy, 0.5*plate.dx, 0.5*plate.dz);
    vPlate_B10_yxz_l_f->SetFillColor(kGreen);
    vPlate_B10_yxz_l_f->SetLineColor(kGreen);
    vPlate_B10_yxz_l_f->SetTransparency(60);


    TGeoVolume* vPlate_B10_yxz_l_b = gGeoManager->MakeBox("vPlate_B10_yxz_l_b", Scint, 0.5*plate.dy, 0.5*plate.dx, 0.5*plate.dz);
    vPlate_B10_yxz_l_b->SetFillColor(kGreen);
    vPlate_B10_yxz_l_b->SetLineColor(kGreen);
    vPlate_B10_yxz_l_b->SetTransparency(60);
    //-----------------------------------------------------

    // --- 10B Plates XZY -------------------------------
    TGeoVolume* vPlate_B10_xzy_f = gGeoManager->MakeBox("vPlate_B10_xzy_f", Scint, 0.5*plate.dx, 0.5*plate.dz, 0.5*plate.dy);
    vPlate_B10_xzy_f->SetFillColor(kGreen);
    vPlate_B10_xzy_f->SetLineColor(kGreen);
    vPlate_B10_xzy_f->SetTransparency(60);
    //-----------------------------------------------------

    // Air layer
    TGeoVolume* BOX_Air = gGeoManager->MakeBox("BOX_Air", Air, 8., 8., 0.5*(Calor_Size+4.));
    BOX_Air->SetFillColor(0);
    BOX_Air->SetLineColor(kYellow);
    BOX_Air->SetTransparency(60);

    // Lead box
    TGeoVolume* BOX_Lead = gGeoManager->MakeBox("BOX_Lead", Lead, 8.1, 8.1, 0.5*(Calor_Size+4.2));
    BOX_Lead->SetFillColor(kBlue);
    BOX_Lead->SetLineColor(kBlue);
    BOX_Lead->SetTransparency(60);


    // Charge particle detection system
    TGeoVolume* ChMP = gGeoManager->MakeBox("ChMP", Air, chmp.dx, chmp.dy, chmp.dz);
    ChMP->SetFillColor(kBlack);
    ChMP->SetLineColor(kBlack);
    ChMP->SetTransparency(60);

    // Container
    TGeoVolume* Detector = gGeoManager->MakeBox("Detector", Air, 50., 50., 100.);
    Detector->SetFillColor(0);
    Detector->SetLineColor(kYellow);
    Detector->SetTransparency(60);

    // Volumes hierarchical
    Double_t zz = -0.5*Layer_Size + 0.5*calor.Scint_z;
    layer->AddNode(vCalorimetrScint, 1, new TGeoTranslation(0., 0., zz));

    zz =  -0.5*Layer_Size + 0.5*calor.Scint_z + 0.5*calor.Boron_film_z;
    layer->AddNode(vCalorimetrFilm, 1, new TGeoTranslation(0., 0., zz));

    zz =  0.5*Layer_Size - 0.5*calor.Lead_z;
    layer->AddNode(vCalorimetrLead, 1, new TGeoTranslation(0., 0., zz));

    zz = -0.5*(Calor_Size+Layer_Size);

    for (Int_t i = 0; i < (Int_t)calor.NLayer; i++) {
         zz += Layer_Size;
         vCalorimetr->AddNode(layer, i+1, new TGeoTranslation(0., 0., zz));
    }
    cout << "calor.NLayer "<<calor.NLayer <<endl;

    BOX_Air->AddNode(vCalorimetr, 1, new TGeoTranslation(0., 0., 0.));

    BOX_Air->AddNode(vPlate_B10_xyz_u_f, 1, new TGeoTranslation(0., 0.5*(calor.dy+plate.dy), -0.5*plate.dz));
    BOX_Air->AddNode(vPlate_B10_xyz_u_b, 1, new TGeoTranslation(0., 0.5*(calor.dy+plate.dy), 0.5*plate.dz));
    BOX_Air->AddNode(vPlate_B10_xyz_d_f, 1, new TGeoTranslation(0., -0.5*(calor.dy+plate.dy), -0.5*plate.dz));
    BOX_Air->AddNode(vPlate_B10_xyz_d_b, 1, new TGeoTranslation(0., -0.5*(calor.dy+plate.dy), 0.5*plate.dz));

    BOX_Air->AddNode(vPlate_B10_yxz_r_f, 1, new TGeoTranslation(0.5*(calor.dy+plate.dy), 0., -0.5*plate.dz));
    BOX_Air->AddNode(vPlate_B10_yxz_r_b, 1, new TGeoTranslation(0.5*(calor.dy+plate.dy), 0., 0.5*plate.dz));
    BOX_Air->AddNode(vPlate_B10_yxz_l_f, 1, new TGeoTranslation(-0.5*(calor.dy+plate.dy), 0., -0.5*plate.dz));
    BOX_Air->AddNode(vPlate_B10_yxz_l_b, 1, new TGeoTranslation(-0.5*(calor.dy+plate.dy), 0., 0.5*plate.dz));

    BOX_Air->AddNode(vPlate_B10_xzy_f, 1, new TGeoTranslation(0., 0., -0.5*(plate.dy+Calor_Size) ));

    BOX_Lead->AddNode(BOX_Air, 1, new TGeoTranslation(0.,0., 0.));

    Detector->AddNode(BOX_Lead, 1, new TGeoTranslation(0., 0., 0.));

    Detector->AddNode(ChMP, 1, new TGeoTranslation(0., 0., -(Calor_Size+10.1+ chmp.dz)*0.5));

    top->AddNode(Detector, 1);


    // ---------------   Finish   -----------------------------------------------
    gGeoManager->SetTopVolume(top);
    gGeoManager->CloseGeometry();
    gGeoManager->CheckOverlaps(0.001);
    gGeoManager->PrintOverlaps();
    //gGeoManager->CheckGeometry();
    //gGeoManager->CheckGeometryFull();

    //gGeoManager->GetTopVolume()->Draw("ogl");

    TFile* geoFile = new TFile(geoFileName, "RECREATE");
    top->Write();
    geoFile->Close();
}







