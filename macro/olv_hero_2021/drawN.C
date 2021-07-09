void drawN(TString inputDir = "output")
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

  Int_t binNumb = 100;
  Double_t minBin = 0;
  Double_t maxBin = 410;
  Double_t binStep = (maxBin - minBin)/Double_t(binNumb);
  cout << "binStep: " << binStep << endl;
  TString histName = "N";
  TH1F* histo = new TH1F(histName, histName, binNumb, minBin, maxBin);

  Int_t maxdn = 0;
  Int_t mindn = 1000000;
  //nEvents = 100; //TODO debug
  for (UInt_t i = 0; i < nEvents; i++) {
    cout << "Event: " << i << endl;
    Br->GetEntry(i);
    HEROPoint* Point;
    TIter Iter(Arr);
    Int_t nPoints = 0;
    Bool_t flag = kFALSE;
    // Loop over points
    while ((Point = (HEROPoint*)Iter.Next())) {
      // 2112 neutron 1000020040 alpha
      if (Point->GetPID() == 2112) {
        flag = kTRUE;
        nPoints++;
      }
    } // loop over points end
    if (flag) {
      maxdn = max(maxdn, nPoints);
      mindn = min(mindn, nPoints);
      histo->Fill(nPoints);
    }
  }
  cout << mindn << ", " << maxdn << endl;

  TH1F* histToDraw = histo;
  TCanvas* canv = new TCanvas("canv", "canv");
  //canv->SetLogy();
  histToDraw->Draw();
  histToDraw->SetLineWidth(3);
  TAxis* xax = histToDraw->GetXaxis();
  TAxis* yax = histToDraw->GetYaxis();
  xax->SetTitle("number of neutrons []");
  yax->SetTitle("counts");
  histToDraw->SetTitle("neutrons num distribution");
}
