void Prototyp_test()
{
    typedef struct{ 
    Double_t dx = 12.;
    Double_t dy = 12.;
    Double_t Scint_z = 0.15;
    Double_t Lead_z=0.08;
    Double_t NLayer=26./(Lead_z+Scint_z);
    }Calorimetr;
     typedef struct{ 
    Double_t dx = 10.;
    Double_t dy = 0.5;
    Double_t dz = 12.;
    } Plate_B10;
     typedef struct{ 
    Double_t dx = 2.;
    Double_t dy = 2.;
    Double_t dz = 1.;
    } SIZ ;
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
    TString geoFileName = geoPath + "/geometry/OLV_Prototyp.root";
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
    
    //------------------  Material for SIZ  --------------
    Calorimetr calor;
    Plate_B10 plate;
    SIZ  chmp;
    Double_t Layer_Size = calor.Scint_z + calor.Lead_z;
    Double_t Calor_Size = calor.NLayer * Layer_Size;
    TGeoVolume* vCalorimetr = gGeoManager->MakeBox("vCalorimetr",Air, 0.5*calor.dx, 0.5*calor.dy,0.5*Calor_Size);
    vCalorimetr->SetFillColor(kGreen);
    vCalorimetr->SetLineColor(kGreen);
    vCalorimetr->SetTransparency(60);
    TGeoVolume* layer = gGeoManager->MakeBox("layer",Air,0.5*calor.dx,0.5*calor.dy,0.5*Layer_Size); 
    
    TGeoVolume* vCalorimetrLead = gGeoManager->MakeBox("vCalorimetrLead", Lead,0.5*calor.dx, 0.5*calor.dy, 0.5*calor.Lead_z);
    vCalorimetrLead->SetFillColor(kBlue);
    vCalorimetrLead->SetLineColor(kBlue);
    vCalorimetrLead->SetTransparency(60);
    TGeoVolume* vCalorimetrScint = gGeoManager->MakeBox("vCalorimetrScint", Polysterol, 0.5*calor.dx, 0.5*calor.dy, 0.5*calor.Scint_z);
    vCalorimetrScint->SetFillColor(kRed);
    vCalorimetrScint->SetLineColor(kRed);
    vCalorimetrScint->SetTransparency(60);
TGeoVolume* vPlate_B10 = gGeoManager->MakeBox("vPlate_B10", Scint, 0.5*plate.dx, 0.5*plate.dy, 0.5*plate.dz);
    vPlate_B10->SetFillColor(kGreen);
    vPlate_B10->SetLineColor(kGreen);
    vPlate_B10->SetTransparency(60);
TGeoVolume* vPlate_B10_Roter = gGeoManager->MakeBox("vPlate_B10_Roter", Scint, 0.5*plate.dy, 0.5*plate.dx, 0.5*plate.dz);
    vPlate_B10_Roter->SetFillColor(kGreen);
    vPlate_B10_Roter->SetLineColor(kGreen);
    vPlate_B10_Roter->SetTransparency(60);
TGeoVolume* vPlate_B10_front = gGeoManager->MakeBox("vPlate_B10_Roter", Scint, 0.5*plate.dx, 0.5*plate.dz, 0.5*plate.dy);
    vPlate_B10_front->SetFillColor(kGreen);
    vPlate_B10_front->SetLineColor(kGreen);
    vPlate_B10_front->SetTransparency(60);
    
    TGeoVolume* ChMP = gGeoManager->MakeBox("ChMP", Air, chmp.dx, chmp.dy, chmp.dz);
    ChMP->SetFillColor(kBlack);
    ChMP->SetLineColor(kBlack);
    ChMP->SetTransparency(60);
    
     TGeoVolume* BOX_Lead = gGeoManager->MakeBox("BOX_Lead",Lead, 8.1,8.1, 0.5*(Calor_Size+10.1));
    BOX_Lead->SetFillColor(kBlue);
    BOX_Lead->SetLineColor(kBlue);
    BOX_Lead->SetTransparency(60);
     TGeoVolume* BOX_Air = gGeoManager->MakeBox("BOX_Air",Air, 8, 8, 0.5*(Calor_Size+10.));
    BOX_Air->SetFillColor(0);
    BOX_Air->SetLineColor(kYellow);
    BOX_Air->SetTransparency(60);
     
    TGeoVolume* Detector = gGeoManager->MakeBox("Detector",Air, 50., 50., 100.);
    Detector->SetFillColor(0);
    Detector->SetLineColor(kYellow);
    Detector->SetTransparency(60);

    
   Double_t zz = -0.5*Layer_Size + 0.5*calor.Scint_z;
    layer->AddNode(vCalorimetrScint,1,new TGeoTranslation(0,0,zz));
        
    zz =  0.5*Layer_Size - 0.5*calor.Lead_z;
    layer->AddNode(vCalorimetrLead,1,new TGeoTranslation(0,0,zz));
    
        
    zz = -0.5*(Calor_Size+Layer_Size);
    
    for (Int_t i(0); i<calor.NLayer; i++) {
         zz += Layer_Size;
         vCalorimetr->AddNode(layer,i+1,new TGeoTranslation(0,0,zz)); 
    }
    cout << "calor.NLayer "<<calor.NLayer <<endl;
    
//     BOX_Air->AddNode(vCalorimetr,1,new TGeoTranslation(0,0,0)); 
//    
//     BOX_Air->AddNode(vPlate_B10,1,new TGeoTranslation(0,0.5*(calor.dy+plate.dy),0.5*plate.dz));
//     BOX_Air->AddNode(vPlate_B10,1,new TGeoTranslation(0,0.5*(calor.dy+plate.dy),-0.5*plate.dz));
//     BOX_Air->AddNode(vPlate_B10,1,new TGeoTranslation(0,-0.5*(calor.dy+plate.dy),0.5*plate.dz));
//     BOX_Air->AddNode(vPlate_B10,1,new TGeoTranslation(0,-0.5*(calor.dy+plate.dy),-0.5*plate.dz));
//     
//     BOX_Air->AddNode(vPlate_B10_Roter,1,new TGeoTranslation(0.5*(calor.dy+plate.dy),0,0.5*plate.dz));
//     BOX_Air->AddNode(vPlate_B10_Roter,1,new TGeoTranslation(0.5*(calor.dy+plate.dy),0,-0.5*plate.dz));
//     BOX_Air->AddNode(vPlate_B10_Roter,1,new TGeoTranslation(-0.5*(calor.dy+plate.dy),0,0.5*plate.dz));
//     BOX_Air->AddNode(vPlate_B10_Roter,1,new TGeoTranslation(-0.5*(calor.dy+plate.dy),0,-0.5*plate.dz));
//     
//     BOX_Air->AddNode(vPlate_B10_front,1,new TGeoTranslation(0,0, -0.5*(plate.dy+Calor_Size) ));

     BOX_Lead->AddNode(BOX_Air,1,new TGeoTranslation(0,0,0));
    
    Detector->AddNode(BOX_Lead,1,new TGeoTranslation(0,0,0)); 
    Detector->AddNode(vCalorimetr,1,new TGeoTranslation(0,0,0)); 
   
    Detector->AddNode(vPlate_B10,1,new TGeoTranslation(0,0.5*(calor.dy+plate.dy),0.5*plate.dz));
    Detector->AddNode(vPlate_B10,1,new TGeoTranslation(0,0.5*(calor.dy+plate.dy),-0.5*plate.dz));
    Detector->AddNode(vPlate_B10,1,new TGeoTranslation(0,-0.5*(calor.dy+plate.dy),0.5*plate.dz));
    Detector->AddNode(vPlate_B10,1,new TGeoTranslation(0,-0.5*(calor.dy+plate.dy),-0.5*plate.dz));
    
    Detector->AddNode(vPlate_B10_Roter,1,new TGeoTranslation(0.5*(calor.dy+plate.dy),0,0.5*plate.dz));
    Detector->AddNode(vPlate_B10_Roter,1,new TGeoTranslation(0.5*(calor.dy+plate.dy),0,-0.5*plate.dz));
    Detector->AddNode(vPlate_B10_Roter,1,new TGeoTranslation(-0.5*(calor.dy+plate.dy),0,0.5*plate.dz));
    Detector->AddNode(vPlate_B10_Roter,1,new TGeoTranslation(-0.5*(calor.dy+plate.dy),0,-0.5*plate.dz));
    
    Detector->AddNode(vPlate_B10_front,1,new TGeoTranslation(0,0, -0.5*(plate.dy+Calor_Size) ));

       
      
     Detector->AddNode(ChMP,1,new TGeoTranslation(0,0,-(Calor_Size+10.1+ chmp.dz)*0.5)); 
    
    
    top->AddNode(Detector, 1);


    // ---------------   Finish   -----------------------------------------------
    gGeoManager->SetTopVolume(top);
    gGeoManager->CloseGeometry();
    gGeoManager->CheckOverlaps(0.001);
    gGeoManager->PrintOverlaps();
    gGeoManager->CheckGeometry();
    gGeoManager->CheckGeometryFull();

    gGeoManager->GetTopVolume()->Draw("ogl");

    TFile* geoFile = new TFile(geoFileName, "RECREATE");
    top->Write();
    geoFile->Close();
    
}
    
     
    
    
    

    
    
    
    
    
    
    
    
    
