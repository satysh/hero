void drawmax(TString inputDir = "output")
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

  Int_t binNumb = 100000;
  Double_t minBin = 0.;
  Double_t maxBin = 20.;
  Double_t binStep = (maxBin - minBin)/Double_t(binNumb);
  cout << "binStep: " << binStep << endl;
  TString histName = "dt";
  TH1F* histo = new TH1F(histName, histName, binNumb, minBin, maxBin);
  histName = "meanhist";
  TH1F* meanhist = new TH1F(histName, histName, binNumb, minBin, maxBin);

  Double_t maxdtmean = 0.;
  Double_t mindtmean = 1000000.;
  Double_t maxdt = 0.;
  Double_t mindt = 1000000.;
  //nEvents = 100; //TODO debug
  for (UInt_t i = 0; i < nEvents; i++) {
    maxdt = 0.;
    cout << "Event: " << i << endl;
    Br->GetEntry(i);
    HEROPoint* Point;
    TIter Iter(Arr);
    Double_t dtmean = 0.;
    Int_t nPoints = 0;
    Bool_t flag = kFALSE;
    // Loop over points
    while ((Point = (HEROPoint*)Iter.Next())) {
      // 2112 neutron 1000020040 alpha
      if (Point->GetPID() == 1000020040) {
        flag = kTRUE;
        Double_t curTimeIn = Point->GetTimeIn()*1e-3;
        Double_t curTimeOut = Point->GetTimeOut()*1e-3;

        Double_t dTime = curTimeOut;
        maxdt = max(maxdt, dTime);
        mindt = min(mindt, dTime);
        histo->Fill(dTime);

        dtmean += dTime;
        nPoints++;
      }
    } // loop over points end
    if (flag) {
      dtmean /= Double_t(nPoints);
      maxdtmean = max(maxdtmean, dtmean);
      mindtmean = min(mindtmean, dtmean);
      meanhist->Fill(maxdt);
    }
  }
  cout << mindtmean << ", " << maxdtmean << endl;
  cout << mindt << ", " << maxdt << endl;

  TH1F* histToDraw = meanhist;
  TCanvas* canv = new TCanvas("canv", "canv");
  //canv->SetLogy();
  histToDraw->Draw();
  histToDraw->SetLineWidth(3);
  TAxis* xax = histToDraw->GetXaxis();
  TAxis* yax = histToDraw->GetYaxis();
  xax->SetTitle("t [usec]");
  yax->SetTitle("counts");
  histToDraw->SetTitle("");
}
