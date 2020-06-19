// ------------------------------------------------------------------------
//  ===== STANDARD ION TABLE (Z, A) =====
//
//      H  :  1,  1       Si : 14, 28
//      He :  2,  4       P  : 15, 31
//      Li :  3,  7       S  : 16, 32
//      Be :  4,  9       Cl : 17, 35
//      B  :  5, 11       Ar : 18, 40
//      C  :  6, 12       K  : 19, 39
//      N  :  7, 14       Ca : 20, 40
//      O  :  8, 16       Sc : 21, 45
//      F  :  9, 19       Ti : 22, 48
//      Ne : 10, 20        V : 23, 51
//      Na : 11, 23       Cr : 24, 52
//      Mg : 12, 24       Mn : 25, 55
//      Al : 13, 27       Fe : 26, 56
// ------------------------------------------------------------------------

int  GetPdgCode(const int Z, const int A);
void AddIon(const int pdg);                    //For PDG ion beam

void sim(Int_t nEvents = 487, Int_t index = 0)
{
  gRandom->SetSeed(index);

  //---------------------Files-----------------------------------------------
  TString outFile;
  outFile.Form("sim.root");
  TString parFile;
  parFile.Form("par.root");
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

  TString geoFileName = "door.root";

  HERODetector* detector = new HERODetector("HEROdetector", kTRUE);
  detector->SetGeometryFileName(geoFileName);
  detector->AddSensetive("vDoor");

  run->AddModule(detector);

// -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  HERONeutronsGenerator* hGen = new HERONeutronsGenerator("gamma.txt");
  hGen->SetPosZ(-500.);

  primGen->AddGenerator(hGen);
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

