void prototyp_test()
{

//  cout << "\n\n\n<NuTarget::ConstructGeometry> " << endl;
//     cout << fNLayers << endl;
//     cout << fNXYPads  << endl;
//     cout << fLayerSizeXY << endl;
//     cout << fLayer1SizeZ  << endl;
//     cout << fLayer2SizeZ  << endl;


    Double_t lzsize = fLayer1SizeZ + fLayer2SizeZ;
    Double_t tzsize = fNLayers * lzsize;

    NuTargetGeo* ngeo = new NuTargetGeo();

    Int_t nmed;

    TGeoMedium* Air = gGeoManager->GetMedium("air");
    if (!Air) {
        nmed = ngeo->CreateMedium("air");
        Air = gGeoManager->GetMedium(nmed);
        if (!Air) Air = DefaultMedium();
    }

    TGeoMedium* Lead = gGeoManager->GetMedium("FscScintVB");
    if (!Lead) {
        nmed = ngeo->CreateMedium("FscScintVB");
        Lead = gGeoManager->GetMedium(nmed);
        if (!Lead) Lead = DefaultMedium();
    }

    TGeoMedium* Scint = gGeoManager->GetMedium("FscScintVB");
    if (!Scint) {
        nmed = ngeo->CreateMedium("FscScintVB");
        Scint = gGeoManager->GetMedium(nmed);
        if (!Scint) Scint = DefaultMedium();
    }

//     if (Air)   cout << Air->GetName() << endl;
//     if (Lead)  cout << Lead->GetName() << endl;
//     if (Scint) cout << Scint->GetName() << endl;

    TGeoVolume* TopVol = gGeoManager->GetTopVolume();

    TString volname;

    TGeoVolume* vol;
    Double_t zz;

    // create target box


    TGeoVolume* target = gGeoManager->MakeBox("TARGET",Lead,
                                              0.5*fLayerSizeXY, 0.5*fLayerSizeXY, 0.5*tzsize);
    target->SetFillColor(kCyan);
    target->SetLineColor(kCyan);
    target->SetTransparency(80);

    // create layer box

    TGeoVolume* layer = gGeoManager->MakeBox("layer",Air,
                                              0.5*fLayerSizeXY, 0.5*fLayerSizeXY, 0.5*lzsize);
    layer->SetFillColor(kGreen);
    layer->SetLineColor(kGreen);
    layer->SetTransparency(70);

    // create double-layer (lead+scint) box

    TGeoVolume* blead = gGeoManager->MakeBox("leadbox",Lead,
                                              0.5*fLayerSizeXY, 0.5*fLayerSizeXY, 0.5*fLayer1SizeZ);
    blead->SetFillColor(kBlue);
    blead->SetLineColor(kBlue);
    blead->SetTransparency(60);

    TGeoVolume* bscin = gGeoManager->MakeBox("scinbox",Scint,
                                              0.5*fLayerSizeXY, 0.5*fLayerSizeXY, 0.5*fLayer2SizeZ);
    bscin->SetFillColor(kPink);
    bscin->SetLineColor(kPink);
    bscin->SetTransparency(50);

    // slice scin plane

    Double_t xypadsize =  fLayerSizeXY/fNXYPads;

    TGeoVolume* scinpad = gGeoManager->MakeBox("scinpad",Scint,
                                               0.5*xypadsize, 0.5*xypadsize, 0.5*fLayer2SizeZ);
    scinpad->SetFillColor(kPink);
    scinpad->SetLineColor(kPink);
    scinpad->SetTransparency(50);

    //ATTENTION!!!ATTENTION!!!ATTENTION
    AddSensitiveVolume(scinpad);
    //ATTENTION!!!ATTENTION!!!ATTENTION

    Int_t n(0);
    Double_t yy;

    Double_t xx = -0.5 * ( fLayerSizeXY + xypadsize);
    for (Int_t i(0); i<fNXYPads; i++) {
         xx += xypadsize;
         yy = -0.5 * ( fLayerSizeXY + xypadsize);
         for (Int_t j(0); j<fNXYPads; j++) {
              n++;
              yy += xypadsize;
              bscin->AddNode(scinpad,n,new TGeoTranslation(xx,yy,0));
         }
    }

    // add sublayers to the main layer

    zz = -0.5*lzsize + 0.5*fLayer1SizeZ;
    layer->AddNode(blead,1,new TGeoTranslation(0,0,zz));

    zz =  0.5*lzsize - 0.5*fLayer2SizeZ;
    layer->AddNode(bscin,1,new TGeoTranslation(0,0,zz));

    //ATTENTION!!!ATTENTION!!!ATTENTION
    AddSensitiveVolume(bscin);
    AddSensitiveVolume(blead);
    //ATTENTION!!!ATTENTION!!!ATTENTION

    // add layers to the target

    zz = -0.5*(tzsize + lzsize);

    for (Int_t i(0); i<fNLayers; i++) {
         zz += lzsize;
         target->AddNode(layer,i+1,new TGeoTranslation(0,0,zz));
    }

    // add target to TOP volume

    TopVol->AddNode(target,1,new TGeoTranslation(0,0,0));

    delete ngeo;
}
