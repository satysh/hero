void sim(Int_t nEvents=100, Int_t index=1)
{
  // -----   Particle  --------------------------------------------------------
  Int_t pdgId = 2212; // proton 2212

  // ------------------------------------------------------------------------
  gRandom->SetSeed(index);

  //---------------------Files-----------------------------------------------
  TString outFile = "output/sim.root";
  TString parFile = "output/par.root";
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();

  FairRunSim* run = new FairRunSim();
  run->SetName("TGeant4");              // Transport engine
  run->SetOutputFile(outFile.Data());          // Output file
  run->SetMaterials("media.geo");       // Materials

  FairModule* cave= new FairCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);

  HERODetector* detector = new HERODetector("HEROdetector", kTRUE);
  detector->SetGeometryFileName("HERO_detector_Sphear.geo.root");
  detector->AddSensetive("vAir_Sphere");
  run->AddModule(detector);

// -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  HEROSphereGenerator* shpGen = new HEROSphereGenerator(pdgId, 1);
  shpGen->SetDistance(-500.);
  Double_t dtheta = fabs(atan(125./500.))*TMath::RadToDeg();
  shpGen->SetThetaRange(-dtheta, dtheta);
  primGen->AddGenerator(shpGen);
  run->SetGenerator(primGen);

  run->SetStoreTraj(kFALSE); // kFALSE

  //-------Set LOG verbosity  -----------------------------------------------
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");

  run->Init();

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();

  // -----   Run simulation  ------------------------------------------------
  run->Run(nEvents);

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is sim.root" << endl;
  cout << "Parameter file is par.root" << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime
                  << "s" << endl << endl;
}

