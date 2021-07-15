void b(TString inputDir = "output")
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
  Br = tree->GetBranch("HEROdetectorvAir_SpherePoint");
  if (!Br) {
    cerr << " Branche in tree read error." << endl;
  }
  cout << "nevents in branch is " << Br->GetEntries() << endl;

  // Form and set adress to data arrays
  TClonesArray* Arr;
  Arr = new TClonesArray("HEROPoint");
  Br->SetAddress(&Arr);

  Int_t binNumb = 1000;
  Double_t minBin = 0.;
  Double_t maxBin = 130.;
  Double_t binStep = (maxBin - minBin)/Double_t(binNumb);
  cout << "binStep: " << binStep << endl;
  TString histName = "b";
  TH1F* histo = new TH1F(histName, histName, binNumb, minBin, maxBin);

  Double_t minb = 1000.;
  Double_t maxb = 0.;
  for (UInt_t i = 0; i < nEvents; i++) {
    cout << "Event: " << i << endl;
    Br->GetEntry(i);
    HEROPoint* Point;
    TIter Iter(Arr);
    // Loop over points
    while ((Point = (HEROPoint*)Iter.Next())) {
      // 2112 neutron 1000020040 alpha
      if (Point->GetPID() == 2112) {
        Double_t curX = Point->GetXOut();
        Double_t curY = Point->GetYOut();
        Double_t curb = TMath::Sqrt(curX*curX + curY*curY);
        minb = min(minb, curb);
        maxb = max(maxb, curb);
        histo->Fill(curb);
      }
    } // loop over points end
  }
  cout << "minb=" << minb << ", maxb=" << maxb << endl;
  TCanvas* canv = new TCanvas("canv", "canv");
  histo->Draw();
  histo->GetXaxis()->SetTitle("b [cm]");
  histo->GetYaxis()->SetTitle("counts");
  histo->SetTitle("for neutrons (dits = -500 cm)");
}
