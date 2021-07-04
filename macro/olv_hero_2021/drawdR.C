void drawdR(TString inputDir = "output")
{
  TString fileName;
  fileName.Form("%s/sim.root", inputDir.Data());
  TFile* file = new TFile(fileName, "READ");
  if (file->IsZombie()) {
      cerr << "File read error" << endl;
      return;
  }


  TTree* tree = (TTree*)file->Get("HERO;34");
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
  Double_t maxBin = 7.;
  Double_t binStep = (maxBin - minBin)/Double_t(binNumb);
  cout << "binStep: " << binStep << endl;
  TString histName = "dR";
  TH1F* histo = new TH1F(histName, histName, binNumb, minBin, maxBin);
  histName = "meanhist";
  TH1F* meanhist = new TH1F(histName, histName, binNumb, minBin, maxBin);


  Double_t maxdr = 0.;
  Double_t mindr = 1000000.;
  Double_t maxdrmean = 0.;
  Double_t mindrmean = 1000000.;
  //nEvents = 100; //TODO debug
  for (UInt_t i = 0; i < nEvents; i++) {
    cout << "Event: " << i << endl;
    Br->GetEntry(i);
    HEROPoint* Point;
    TIter Iter(Arr);
    Double_t drmean = 0.;
    Int_t nPoints = 0;
    // Loop over points
    while ((Point = (HEROPoint*)Iter.Next())) {
      // 2112 neutron 1000020040 alpha
      if (Point->GetPID() == 1000020040) {
        Double_t curxin = Point->GetXIn();
        Double_t curyin = Point->GetYIn();
        Double_t curzin = Point->GetZIn();
        Double_t curxout = Point->GetXOut();
        Double_t curyout = Point->GetYOut();
        Double_t curzout = Point->GetZOut();

        Double_t outXmInX = curxout-curxin;
        Double_t outYmInY = curyout-curyin;
        Double_t outZmInZ = curzout-curzin;
        Double_t dR = TMath::Sqrt(outXmInX*outXmInX + outYmInY*outYmInY + outZmInZ*outZmInZ);
        maxdr = max(maxdr, dR);
        mindr = min(mindr, dR);
        histo->Fill(dR);

        drmean += dR;
        nPoints++;
      }
    } // loop over points end
    drmean /= Double_t(nPoints);
    maxdrmean = max(maxdrmean, drmean);
    mindrmean = min(mindrmean, drmean);
    meanhist->Fill(drmean);
  }
  cout << mindrmean << ", " << maxdrmean << endl;
  cout << mindr << ", " << maxdr << endl;

  TH1F* histToDraw = histo;
  TCanvas* canv = new TCanvas("canv", "canv");
  //canv->SetLogy();
  histToDraw->Draw();
  histToDraw->SetLineWidth(3);
  TAxis* xax = histToDraw->GetXaxis();
  TAxis* yax = histToDraw->GetYaxis();
  xax->SetTitle("dR [cm]");
  yax->SetTitle("counts");
  histToDraw->SetTitle("Alpha lenght distribution");
}
