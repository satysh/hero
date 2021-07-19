void point(TString inputDir = "output")
{
  Double_t pMass = 0.93827208816; // GeV
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

  //nEvents = 100; //TODO debug
  Int_t counter = 0;
  for (UInt_t i = 0; i < nEvents; i++) {
    cout << "Event: " << i << endl;
    Br->GetEntry(i);
    HEROPoint* Point;
    TIter Iter(Arr);
    while ((Point = (HEROPoint*)Iter.Next())) {
      if (Point->GetTrackID() == 0) {
        Double_t pPabsIn = Point->GetPIn();
        Double_t pEkinIn = TMath::Sqrt(pPabsIn*pPabsIn + pMass*pMass) - pMass;
        cout << "GetPdgCode = " << Point->GetPID() << ", ";
        cout << "In x,y,z, Ekin = " << Point->GetXIn() << ", " << Point->GetYIn() << ", " << Point->GetZIn() << ", "
                                    << pEkinIn << endl;
        Double_t pPabsOut = Point->GetPOut();
        Double_t pEkinOut = TMath::Sqrt(pPabsOut*pPabsOut + pMass*pMass) - pMass;
        cout << "Out x,y,z, Ekin = " << Point->GetXOut() << ", " << Point->GetYOut() << ", " << Point->GetZOut() << ", "
                                     << pEkinOut << endl;
        counter++;
      }
    } // loop over points end
  }
  cout << "Particle's n with Point id = 0 is " << counter << endl;
}
