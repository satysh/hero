void proton(TString inputDir = "output")
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

  Int_t binNumb = 100;
  Double_t minBin = 0.;
  Double_t maxBin = 21.;
  Double_t binStep = (maxBin - minBin)/Double_t(binNumb);
  cout << "binStep: " << binStep << endl;
  TString histName = "histo";
  TH1F* histo = new TH1F(histName, histName, binNumb, minBin, maxBin);

  //nEvents = 100; //TODO debug
  Int_t counter = 0;
  for (UInt_t i = 0; i < nEvents; i++) {
    Int_t nneutrons = 0;
    Double_t ekin = 0.;
    cout << "Event: " << i << endl;
    Br->GetEntry(i);
    HEROMCTrack* Track;
    TIter Iter(Arr);
    while ((Track = (HEROMCTrack*)Iter.Next())) {
      if (Track->GetId() == 0) {
        ekin = Track->GetEkin();
        cout << "GetPdgCode = " << Track->GetPdgCode() << ", ";
        cout << "Start x,y,z,t, Ekin = " << Track->GetStartX() << ", " << Track->GetStartY() << ", " << Track->GetStartZ()
                                   << ", " << Track->GetStartT() << ", " << Track->GetEkin() << endl;
        counter++;
      }
      if (Track->GetPdgCode() == 2112) {
        nneutrons++;
      }
    } // loop over points end
    cout << "proton ekin = " << ekin << endl;
    cout << "nneutrons = " << nneutrons << endl;
    for (Int_t i=0; i<nneutrons; i++) {
      histo->Fill(ekin);
    }
    //histo->Fill(ekin, nneutrons);
  }
  cout << "Particle's n with track id = 0 is " << counter << endl;

  // Formate histo


  TCanvas* canv = new TCanvas("canv", "canv");
  histo->Draw();
  histo->GetXaxis()->SetTitle("Ekin [GeV]");
  histo->GetYaxis()->SetTitle("nneutrons");
  histo->SetTitle("dist = -500 cm");
}
