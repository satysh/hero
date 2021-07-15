void rw(TString inputDir = "output")
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
  TString histName = "alpha";
  TH1F* histo = new TH1F(histName, histName, binNumb, minBin, maxBin);

  Double_t minr = 1000.;
  Double_t maxr = 0.;
  for (UInt_t i = 0; i < nEvents; i++) {
    cout << "Event: " << i << endl;
    Br->GetEntry(i);
    HEROPoint* Point;
    TIter Iter(Arr);
    // Loop over points
    while ((Point = (HEROPoint*)Iter.Next())) {
      // 2112 neutron 1000020040 alpha
      if (Point->GetPID() == 1000020040) {
        Double_t curX = Point->GetXIn();
        Double_t curY = Point->GetYIn();
        Double_t curZ = Point->GetZIn();
        Double_t curr = TMath::Sqrt(curX*curX + curY*curY + curZ*curZ);
        minr = min(minr, curr);
        maxr = max(maxr, curr);
        histo->Fill(curr);
      }
    } // loop over points end
  }
  cout << "minr=" << minr << ", maxr=" << maxr << endl;

  for (Int_t i=0; i<histo->GetNbinsX(); i++) {
    Double_t oldBinCon = histo->GetBinContent(i);
    Double_t binCen = histo->GetXaxis()->GetBinCenter(i);
    if (binCen <= 125. && binCen > 0.) {
      Double_t newBinCon = 1./(binCen*binCen);
      newBinCon *= oldBinCon;
      histo->SetBinContent(i, newBinCon);
    }

    cout << i << ": " << histo->GetBinContent(i) << ", " << binCen << endl;
  }
  //TH1F* histo2 = new TH1F("rw", histo->GetTitle(), binNumb, minBin, maxBin);

  TCanvas* canv = new TCanvas("canv", "canv");
  histo->Draw();
  histo->GetXaxis()->SetTitle("r [cm]");
  histo->GetYaxis()->SetTitle("counts*(1/r^2)");
  histo->SetTitle("for alpha with weight (dits = -5000 cm)");
}
