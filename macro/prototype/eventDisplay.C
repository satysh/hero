void eventDisplay(TString inputDir = "output")
{
  TString parFileName = inputDir + "/par.root";
  TString simFileName = inputDir + "/sim.root";
  TString outFileName = inputDir + "/eventDisplay.root";

  FairRunAna* fRun = new FairRunAna();
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open(parFileName);
  rtdb->setFirstInput(parIo1);
  //rtdb->print();

  fRun->SetInputFile(simFileName);
  fRun->SetOutputFile(outFileName);

  FairEventManager* fMan = new FairEventManager();
  FairMCTracks* Track = new FairMCTracks ("Monte-Carlo Tracks");
  FairMCPointDraw* Points = new FairMCPointDraw ("OLVdetectorvPlate_B10_xyz_u_f", kRed, kFullSquare);

  fMan->AddTask(Track);
  fMan->AddTask(Points);

  fMan->Init();
}
