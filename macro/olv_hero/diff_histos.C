TH1F* histo_form(TString indir);

void diff_histos(TString indir1 = "output", TString indir2 = "archive") {
  TH1F* h1 = histo_form(indir1);  if (! h1 ) return;
  TH1F* h2 = histo_form(indir2);  if (! h2 ) return;
  TCanvas* canv = new TCanvas("c1", "c1", 800, 1000);
  canv->cd();
  h1->Draw();
  h1->SetLineWidth(2);
  h1->SetLineColor(2);
  h1->DrawPanel();
  TAxis* xAx = (TAxis*)h1->GetXaxis();
  TAxis* yAx = (TAxis*)h1->GetYaxis();
  xAx->SetTitle("time, mks");
  yAx->SetTitle("neutrons number");
  yAx->SetTitleSize(0.05);

  h2->Draw("same");
  gPad->Update();
  gPad->SetFrameLineWidth(5);
}

TH1F* histo_form(TString inputDir) {
  Int_t binNumb = 10000;
  Double_t minBin = -10.;
  Double_t maxBin = 60.;
  TString fileName;
  fileName.Form("%s/sim.root", inputDir.Data());
  TFile* file = new TFile(fileName, "READ");
  if (file->IsZombie()) {
    cerr << fileName << " File read error" << endl;
    return NULL;
  }

  TTree* tree = (TTree*)file->Get("OLV");
  if (!tree) {
    cerr << "Tree read error in " << fileName << endl;
    return NULL;
  }

  TBranch* Br = tree->GetBranch("OLVdetectorvCubPoint");
  if (!Br) {
    cerr << "Branche read error in " << fileName << endl;
    return NULL;
  }

  // Form and set adress to data arrays
  TClonesArray* Arr = new TClonesArray("OLVPoint");
  Br->SetAddress(&Arr);

  UInt_t nEvents = tree->GetEntries();
  cout << "Number of events in " << fileName << " is: " << nEvents << endl;

  Double_t binStep = (maxBin - minBin)/Double_t(binNumb);
  //cout << "binStep: " << binStep << endl;
  TH1F* histo = new TH1F("histo", "histo", binNumb, minBin, maxBin);
  Int_t numOfParticles = 0;
  for (UInt_t i = 0; i < nEvents; i++) {
    //cout << "Event: " << i << endl;
    Br->GetEntry(i);
    OLVPoint* Point;
    TIter Iter(Arr);
    // Loop over points
    while ((Point = (OLVPoint*)Iter.Next())) {
      if (Point->GetPID() == 2112) {
        //cout << Point->GetPID() << endl;
        continue;
      }
/*
      Double_t curTimeIn = Point->GetTimeIn()*1e-3;
      Double_t curTimeOut = Point->GetTimeOut()*1e-3;
      Double_t j = curTimeIn;
      while (j <= curTimeOut) {
        histo->Fill((Float_t)j);
        j += binStep;
        //cout << " j: " << j << endl;
      }
*/
      Double_t curTimeOut = Point->GetTimeOut()*1e-3;
      histo->Fill((Float_t)curTimeOut);
      numOfParticles++;
    } // loop over points end
  }
  cout << "number of particles in " << fileName << " is: " << numOfParticles << endl;
  return histo;
}
