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

void sim(Int_t nEvents = 1, Int_t index = 0, TString outDir = "output", Int_t IonIndex = 1)
{

  // -----   Particle  --------------------------------------------------------
  Int_t pdgId = 2212; // proton 2212 // electron 11
  Double32_t momentum = 13.; // GeV
  Int_t curZ, curA;
  switch(IonIndex) {
    case 1 : curZ = 1; curA = 1; break;
    case 2 : curZ = 2; curA = 4; break;
    case 3 : curZ = 6; curA = 12; break;
    case 4 : curZ = 26; curA = 56; break;
    default : cerr << "index error" << endl; return ;
  }
  pdgId = GetPdgCode(curZ,curA);      // Set nuclear pdg for Ion

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
  detector->SetGeometryFileName("OLV_Prototype.root");
  detector->AddSensetive("vPlate_B10_xyz_u_f");
  detector->AddSensetive("vPlate_B10_xyz_u_b");
  detector->AddSensetive("vPlate_B10_xyz_d_f");
  detector->AddSensetive("vPlate_B10_xyz_d_b");
  detector->AddSensetive("vPlate_B10_yxz_r_f");
  detector->AddSensetive("vPlate_B10_yxz_r_b");
  detector->AddSensetive("vPlate_B10_yxz_l_f");
  detector->AddSensetive("vPlate_B10_yxz_l_b");
  detector->AddSensetive("vPlate_B10_xzy_f");
  run->AddModule(detector);

// -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
  //boxGen->SetPRange(momentum, momentum);
  boxGen->SetEkinRange(momentum, momentum);
  boxGen->SetThetaRange(0., 0.); // 0-90
  boxGen->SetPhiRange(0., 0.); // 0-360
  boxGen->SetBoxXYZ(0., 0., 0., 0., -500.); // xmin, ymin, xmax, ymax, z
  primGen->AddGenerator(boxGen);

  // ------------------------------------------------------------------------
  AddIon(pdgId);                         //Add ion in FairRunSim
  // ------------------------------------------------------------------------

  primGen->AddGenerator(boxGen);
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
int GetPdgCode(const int Z, const int A)             //For PDG ion beam
{
  if (Z == 1 && A == 1) return 2212;
  return 1000000000 + Z*10000 + A*10;
}
void AddIon(const int pdg)
{
  if (pdg < 1000000000) return;

  FairRunSim* run = FairRunSim::Instance();
  if (!run) return;

  int Z = (pdg-1000000000)/10000;
  int A = (pdg-1000000000-10000*Z)/10;

  FairIon* ion = new FairIon();
  ion->SetParams(Form("ION_%03d_%03d",Z,A), Z, A, Z);

  run->AddNewIon(ion);
}

