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
Double_t proccesse(TString ion = "H");
void drawInteg()
{
//  TLegend* leg = new TLegend(1., 1., 0.80, 0.80);
  Int_t nIons = 3;
  TVectorD vZ(nIons);
  TVectorD vNeut(nIons);
  for (Int_t i=0; i<nIons; i++) {
    TString ionname = "";
    Double_t curZ;
    if (i==0) {
      ionname = "H";
      curZ = 1;

    }
    else if (i==1) {
      ionname = "C12";
      curZ = 6;
    }
    else if (i==2) {
      ionname = "Fe56";
      curZ = 26;
    }
    else if (i==3) {
      ionname = "Kr84";
      curZ = 36;
    }
    vZ(i) = curZ;
    vNeut(i) = proccesse(ionname);

    //leg->AddEntry(gr, ionname+"+Fe_15 GeV/a", "l");
  }
  TGraph* gr = new TGraph(vZ, vNeut);
  gr->Draw("ACP");
  gr->SetMarkerStyle(8);
  gr->SetMarkerSize(2);
  gr->SetMarkerColor(2);
  gr->SetTitle("Target is Iron(diameter = 20 [cm], Thick = 5 [cm])");
  //leg->Draw();
  gr->GetXaxis()->SetTitle("Z");
  gr->GetYaxis()->SetTitle("n_neutrons_Integral / nEvents");

  TLatex *latexH = new TLatex(gr->GetX()[0], gr->GetY()[0],"H");
  latexH->SetTextSize(0.05);
  latexH->SetTextColor(kBlack);
  gr->GetListOfFunctions()->Add(latexH);
  TLatex *latexC12 = new TLatex(gr->GetX()[1], gr->GetY()[1],"C^{12}");
  latexC12->SetTextSize(0.05);
  latexC12->SetTextColor(kBlack);
  gr->GetListOfFunctions()->Add(latexC12);
  TLatex *latexFe56 = new TLatex(gr->GetX()[2], gr->GetY()[2],"Fe^{56}");
  latexFe56->SetTextSize(0.05);
  latexFe56->SetTextColor(kBlack);
  gr->GetListOfFunctions()->Add(latexFe56);
  /*TLatex *latexKr84 = new TLatex(gr->GetX()[3], gr->GetY()[3],"Kr^{84}");
  latexFe56->SetTextSize(0.05);
  latexFe56->SetTextColor(kBlack);
  gr->GetListOfFunctions()->Add(latexKr84);*/
  gPad->SetGrid(2,2);
}

Double_t proccesse(TString ion) {
  cout << "proccesse(" << ion << ")" << endl;
  TString fileName = "archive/5cmthicktarget/"+ion+"/sim.root";
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
        nneu++;
      }
    }
  }
  file->Close();
  return (Double_t)nneu/10000.;
}
