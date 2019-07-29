void eventDisplay(TString inputDir = "output")
{
  TString parFileName = inputDir + "/par_1.root";
  TString simFileName = inputDir + "/sim_1.root";
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
  FairMCPointDraw* Points = new FairMCPointDraw ("OLVdetectorbox1Point", kRed, kFullSquare);
  FairMCPointDraw* Points2 = new FairMCPointDraw ("OLVdetectorbox2Point", kRed, kFullSquare);

  fMan->AddTask(Track);
  fMan->AddTask(Points);
  fMan->AddTask(Points2);

  fMan->Init();
}
