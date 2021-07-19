void track(TString inputDir = "output")
{
  TString fileName;
  fileName.Form("%s/sim.root", inputDir.Data());
  TFile* file = new TFile(fileName, "READ");
  if (file->IsZombie()) {
      cerr << "File read error" << endl;
      return;
  }


  TTree* tree = (TTree*)file->Get("HERO");
  if (!tree) {
    cerr << "tree read error" << endl;
    return;
  }
  Int_t nEvents = tree->GetEntries();
  cout << "nevents in tree is " << nEvents << endl;

  TBranch* Br;
  Br = tree->GetBranch("MCTrack");
  if (!Br) {
    cerr << " Branche in tree read error." << endl;
  }
  cout << "nevents in branch is " << Br->GetEntries() << endl;

  // Form and set adress to data arrays
  TClonesArray* Arr;
  Arr = new TClonesArray("HEROMCTrack");
  Br->SetAddress(&Arr);

  //nEvents = 100; //TODO debug
  Int_t counter = 0;
  for (UInt_t i = 0; i < nEvents; i++) {
    cout << "Event: " << i << endl;
    Br->GetEntry(i);
    HEROMCTrack* Track;
    TIter Iter(Arr);
    while ((Track = (HEROMCTrack*)Iter.Next())) {
      if (Track->GetId() == 0) {
        cout << "GetPdgCode = " << Track->GetPdgCode() << ", ";
        cout << "Start x,y,z,t, Ekin = " << Track->GetStartX() << ", " << Track->GetStartY() << ", " << Track->GetStartZ()
                                   << ", " << Track->GetStartT() << ", " << Track->GetEkin() << endl;
        counter++;
      }
    } // loop over points end
  }
  cout << "Particle's n with track id = 0 is " << counter << endl;
}
