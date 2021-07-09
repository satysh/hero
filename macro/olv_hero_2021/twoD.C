void twoD(TString inputDir = "output")
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

  std::vector<Double_t> vntout;
  std::vector<Double_t> vatin;
  Double_t nmaxt = 0.;
  Double_t nmint = 10000.;
  Double_t amaxt = 0.;
  Double_t amint = 10000.;
  //nEvents = 100; // DEBUG
  for (UInt_t i = 0; i < nEvents; i++) {
    cout << "Event: " << i << endl;
    Br->GetEntry(i);
    HEROPoint* Point;
    TIter Iter(Arr);
    Int_t nneutrons = 0;
    Int_t nalpha = 0;
    Double_t meannt = 0.;
    Double_t meanat = 0.;
    Double_t maxtn = 0.;
    Double_t maxta = 0.;
    Bool_t flag1 = kFALSE;
    Bool_t flag2 = kFALSE;
    // Loop over points
    while ((Point = (HEROPoint*)Iter.Next())) {
      // 2112 neutron 1000020040 alpha
      if (Point->GetPID() == 2112) {
        flag1 = kTRUE;
        nneutrons++;
        Double_t timeout = Point->GetTimeOut()*1e-3;
        nmaxt = max(nmaxt, timeout);
        nmint = min(nmint, timeout);
        //vntout.push_back(timeout);
        meannt += timeout;
        maxtn = max(maxtn, timeout);
      }
      else if (Point->GetPID() == 1000020040) {
        flag2 = kTRUE;
        nalpha++;
        Double_t timein = Point->GetTimeIn()*1e-3;
        amaxt = max(amaxt, timein);
        amint = min(amint, timein);
        meanat += timein;
        maxta = max(maxta, timein);
        //vatin.push_back(timein);
      }
    } // loop over points end
    if (flag1 && flag2) {
      vntout.push_back(maxtn);
      vatin.push_back(maxta);
    }
  }
  cout << "n: min = " << nmint << ", max=" << nmaxt << endl;
  cout << "a: min=" << amint << ", max=" << amaxt << endl;
  cout << "vntout size is " << vntout.size() << endl;
  cout << "vatin size is " << vatin.size() << endl;
  TString title = "";
  Int_t nx = 1000;
  Int_t ny = 1000;

  TH2D* histo = new TH2D("histo", title.Data(), nx, 0., 30., ny, 0., 30.);
  for (Int_t i=0; i<vatin.size(); i++) {
    histo->Fill(vntout.at(i), vatin.at(i));
  }

  histo->GetXaxis()->SetTitle("neutron time out [usec]");
  histo->GetYaxis()->SetTitle("alpha time in [usec]");
  histo->Draw("COLZ");
  histo->SetStats(kFALSE);
  gPad->SetGrid(2, 2);
}
