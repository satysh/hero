// ------------------------------------------------------------------------
//  ===== STANDARD ION TABLE (Z, A) =====
//
//      H  :  1,  1       Si : 14, 28
//      He :  2,  4       P  : 15, 31
//      Li :  3,  7       S  : 16, 32
//      Be :  4,  9       Cl : 17, 35
//      B  :  5, 11       Ar : 18, 40
//      C  :  6, 12       K  : 19, 39
//      N  :  7, 14       Ca : 20, 40
//      O  :  8, 16       Sc : 21, 45
//      F  :  9, 19       Ti : 22, 48
//      Ne : 10, 20        V : 23, 51
//      Na : 11, 23       Cr : 24, 52
//      Mg : 12, 24       Mn : 25, 55
//      Al : 13, 27       Fe : 26, 56
// ------------------------------------------------------------------------
int  GetPdgCode(const int Z, const int A);
// ------------------------------------------------------------------------
TTree* proccesse(TString ion = "H");
TGraph* makegraph(TTree* tree);
void drawT()
{
  TLegend* leg = new TLegend(1., 1., 0.80, 0.80);
  TMultiGraph* mg = new TMultiGraph("mg", "Target is Iron(diameter = 20 [cm], Thick = 25 [cm])");
  for (Int_t i=0; i<4; i++) {
    TString ionname = "";
    TTree* tree;
    TGraph* gr;
    if (i==0) {
      ionname = "H";
    }
    else if (i==1) {
      ionname = "C12";
    }
    else if (i==2) {
      ionname = "Fe56";
    }
    else if (i==3) {
      ionname = "Kr84";
    }
    tree = proccesse(ionname); if (!tree) {return;};
    gr = makegraph(tree);
    gr->SetLineWidth(3);
    gr->SetLineColor(i+1);
    gr->SetMarkerColor(i+1);
    gr->SetMarkerSize(5);
    mg->Add(gr);
    leg->AddEntry(gr, ionname+"+Fe_15 GeV/a", "l");
  }
  mg->Draw("ALP");
  leg->Draw();
  gPad->SetGrid(2,2);
  mg->GetXaxis()->SetTitle("neutrons T [MeV]");
  mg->GetYaxis()->SetTitle("log10(n_neutrons / nEvents)");
}

TTree* proccesse(TString ion) {
  cout << "proccesse(" << ion << ")" << endl;
  TString fileName = "archive/"+ion+"/sim.root";
  TFile* file = new TFile(fileName, "READ");
  if (file->IsZombie()) {
      cerr << "File read error" << endl;
      return 0;
  }

  TTree* tree = (TTree*)file->Get("HERO");
  if (!tree) {
    cerr << "Get Tree error!" << endl;
    return 0;
  }
  UInt_t nEvents = tree->GetEntries();
  cout << "Number of events is: " << nEvents << endl;

  TBranch* branch = tree->GetBranch("rogovDetectorvMedPoint");
  if (!branch) {
    cerr << "Get Branch error!" << endl;
    return 0;
  }

  TClonesArray* Arr = new TClonesArray("HEROPoint");
  branch->SetAddress(&Arr);


  Double_t ekin;
  TFile* outfile;
  if (ion == "H") outfile = new TFile("out.root", "RECREATE");
  else            outfile = new TFile("out.root", "UPDATE");
  if (outfile->IsZombie()) {
    cerr << "Can't open out.root" << endl;
    return 0;
  }
  TTree* outtree = new TTree(ion, ion);
  outtree->Branch("ekin", &ekin);
  UInt_t nneu = 0;
  Int_t chEvent = 0;
  for (UInt_t i = 0; i < nEvents; i++) {
    if (chEvent == i | i == nEvents-1) {
      cout << "Event: " << i << endl;
      chEvent += 1000;
    }
    branch->GetEntry(i);
    HEROPoint* Point;
    TIter Iter(Arr);
    while ((Point = (HEROPoint*)Iter.Next())) {
      if (Point->GetPID() == 2112) {
        ekin = Point->GetEkinIn()*1000.; // to MeV
        outtree->Fill();
        nneu++;
      }
    }
  }
  delete Arr;
  file->Close();
  outtree->Write();
  //delete outfile;
  return outtree;
}

TGraph* makegraph(TTree* tree) {
  cout << "makegraph(" << tree->GetTitle() << ")" << endl;
  Int_t nEntries = tree->GetEntries();
  cout << "nEntries: " << nEntries << "\n\n" << endl;
  Int_t nBins = 200;
  Double_t binMax = 1000.; //MeV
  TH1F* histo = new TH1F("histo", "histo", nBins, 0, binMax);
  tree->Draw("ekin >> histo");
  TVectorD vEkin(nBins-1);
  TVectorD vCurrent(nBins-1);
  Int_t integral = 0;
  for (Int_t i=1; i<nBins; i++) {
    vCurrent(i-1) = log10((Double_t)histo->GetBinContent(i)/10000.);
    vEkin   (i-1) = histo->GetBinCenter(i);
    integral += histo->GetBinContent(i);
  }

  TGraph* graph = new TGraph(vEkin, vCurrent);
  delete histo;
  delete tree;
  return graph;
}

int GetPdgCode(const int Z, const int A) {
  if (Z == 1 && A == 1) return 2212;
  return 1000000000 + Z*10000 + A*10;
}

