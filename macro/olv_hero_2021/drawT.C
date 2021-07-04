void drawT(TString inputDir = "output")
{
   // 2112 neutron 1000020040 alphas
  Int_t pid = 1000020040;
  Double_t PDGMass = 3.72737924; // GeV
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
  Double_t maxBin = 600.;
  Double_t binStep = (maxBin - minBin)/Double_t(binNumb);
  cout << "binStep: " << binStep << endl;
  TString histName = "dR";
  TH1F* histo = new TH1F(histName, histName, binNumb, minBin, maxBin);
  histName = "meanhist";
  TH1F* meanhist = new TH1F(histName, histName, binNumb, minBin, maxBin);


  Double_t maxT = 0.;
  Double_t minT = 1000000.;
  Double_t maxTmean = 0.;
  Double_t minTmean = 1000000.;
  //nEvents = 100; //TODO debug
  for (UInt_t i = 0; i < nEvents; i++) {
    cout << "Event: " << i << endl;
    Br->GetEntry(i);
    HEROPoint* Point;
    TIter Iter(Arr);
    Double_t Tmean = 0.;
    Int_t nPoints = 0;
    // Loop over points
    while ((Point = (HEROPoint*)Iter.Next())) {
      if (Point->GetPID() == pid) {
        Double_t curPx = Point->GetPxIn();
        Double_t curPy = Point->GetPyIn();
        Double_t curPz = Point->GetPzIn();
        Double_t pabs = TMath::Sqrt(curPx*curPx + curPy*curPy + curPz*curPz);
        Double_t ekin = TMath::Sqrt(pabs*pabs+PDGMass*PDGMass) - PDGMass;
        ekin *= 1000.; // GeV toMeV
        maxT = max(maxT, ekin);
        minT = min(minT, ekin);
        histo->Fill(ekin);

        Tmean += ekin;
        nPoints++;
      }
    } // loop over points end
    Tmean /= Double_t(nPoints);
    maxTmean = max(maxTmean, Tmean);
    minTmean = min(minTmean, Tmean);
    meanhist->Fill(Tmean);
  }
  cout << minTmean << ", " << maxTmean << endl;
  cout << minT << ", " << maxT << endl;

  TH1F* histToDraw = histo;
  TCanvas* canv = new TCanvas("canv", "canv");
  //canv->SetLogy();
  histToDraw->Draw();
  histToDraw->SetLineWidth(3);
  TAxis* xax = histToDraw->GetXaxis();
  TAxis* yax = histToDraw->GetYaxis();
  xax->SetTitle("T [MeV]");
  yax->SetTitle("counts");
  histToDraw->SetTitle("Alpha ekin distribution");
}
