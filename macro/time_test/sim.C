void sim(Int_t nEvents = 1, Int_t index = 0, TString outDir = "output")
{

  // -----   Particle  --------------------------------------------------------
  Int_t pdgId = 2212; // proton 2212 // electron 11
  Double32_t eKin = 300.; // GeV
  // ------------------------------------------------------------------------

  gRandom->SetSeed(index);

  //---------------------Files-----------------------------------------------
  TString outFile;
  outFile.Form("%s/sim_%d.root", outDir.Data(), index);
  TString parFile;
  parFile.Form("%s/par_%d.root", outDir.Data(), index);
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

  OLVDetector* detector = new OLVDetector("OLVdetector", kTRUE);
  detector->SetGeometryFileName("time_test_geo.root");
  detector->AddSensetive("box1");
  detector->AddSensetive("box2");
  run->AddModule(detector);

// -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
  boxGen->SetEkinRange(eKin, eKin);
  boxGen->SetThetaRange(0., 0.); // 0-90
  boxGen->SetPhiRange(0., 0.); // 0-360
  boxGen->SetBoxXYZ(0., 0., 0., 0., 0.); // xmin, ymin, xmax, ymax, z
  primGen->AddGenerator(boxGen);

  // ------------------------------------------------------------------------

  primGen->AddGenerator(boxGen);
  run->SetGenerator(primGen);

  run->SetStoreTraj(kTRUE); // kFALSE

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
