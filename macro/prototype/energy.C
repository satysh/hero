void energy(TString inputDir = "output_paralell", Int_t NTHR = 16)
{
  //========== Histogram form =========================================
  Int_t binNumb = 1000;
  Double_t minBin = 0.;
  Double_t maxBin = 10;
  Double_t binStep = (maxBin - minBin)/Double_t(binNumb);
  cout << "binStep: " << binStep << endl;
  TString fileName;
  TH1F* histo[9];
  for (Int_t i = 0; i < 9; i++)
  {
    TString histName;
    histName.Form("hsito_%d", i);
    histo[i] = new TH1F(histName, histName, binNumb, minBin, maxBin);
  }
  //===================================================================
  TString partName = "neutron";
  TParticlePDG*   partPDG;
  partPDG = TDatabasePDG::Instance()->GetParticle(partName);
  if (!partPDG )
  {
    cerr << partName << " doesn't exit!" << endl;
    return;
  }

  Double_t pMass = partPDG->Mass();
  cout << "Particle mass is " << pMass << endl;
  pMass = 3.72737924; // alpha mass
  // Loop over THR start
  for (Int_t THR = 1; THR <= NTHR; THR++)
  {
    // Reding root file open
    fileName.Form("%s/sim_%d.root", inputDir.Data(), THR);
    TFile* file = new TFile(fileName, "READ");
    if (file->IsZombie())
    {
      cerr << "File read error" << endl;
      return;
    }
    // Finding the tree in root file
    TTree* tree = (TTree*)file->Get("OLV");
    if (!tree)
    {
      cerr << "Tree read error" << endl;
      return;
    }
    // Branches reding
    TBranch* Br[9];
    Br[0] = tree->GetBranch("OLVdetectorvPlate_B10_xyz_u_fPoint");
    Br[1] = tree->GetBranch("OLVdetectorvPlate_B10_xyz_u_bPoint");
    Br[2] = tree->GetBranch("OLVdetectorvPlate_B10_xyz_d_fPoint");
    Br[3] = tree->GetBranch("OLVdetectorvPlate_B10_xyz_d_bPoint");
    Br[4] = tree->GetBranch("OLVdetectorvPlate_B10_yxz_l_fPoint");
    Br[5] = tree->GetBranch("OLVdetectorvPlate_B10_yxz_l_bPoint");
    Br[6] = tree->GetBranch("OLVdetectorvPlate_B10_yxz_r_fPoint");
    Br[7] = tree->GetBranch("OLVdetectorvPlate_B10_yxz_r_bPoint");
    Br[8] = tree->GetBranch("OLVdetectorvPlate_B10_xzy_fPoint");
    for (Int_t i = 0; i < 9; i++)
    if (!Br[i])
    {
      cerr << "i:" << i << " Branche read error." << endl;
    }

    // Form and set adress to data arrays
    TClonesArray* Arr[9];
    for (Int_t i = 0; i < 9; i++)
    {
      Arr[i] = new TClonesArray("OLVPoint");
      Br[i]->SetAddress(&Arr[i]);
    }
    UInt_t nEvents = tree->GetEntries();
    cout << "Number of events is: " << nEvents << endl;

    // Loop over events start
    for (UInt_t i = 0; i < nEvents; i++)
    {
      // Loop over plates start
      for (Int_t j = 0; j < 9; j++)
      {
        Br[j]->GetEntry(i);
        OLVPoint* Point;
        TIter Iter(Arr[j]);
        Int_t neutronsNum = 0;
        Int_t alphaNum = 0;

        // Loop over points
        while ((Point = (OLVPoint*)Iter.Next()))
        {
          // == - It writes neutrons, != - It writes alphas
          if (Point->GetPID() != 1000020040)
            continue;

          Double_t curPIn = Point->GetPIn();
          Double_t curPIn2 = curPIn*curPIn;
          Double_t inEnergy = sqrt(curPIn2 + pMass*pMass);
          inEnergy -= pMass;
          histo[j]->Fill(1000.*inEnergy);

        } // loop over points end
      } // loop over plates end
    } // loop over Events end

    // Close and clear
    file->Close();
    for (Int_t i = 0; i < 9; i++)
    {
        Br[i] = NULL;
        Arr[i]->Clear();
    }
  } // loop over THR end

  TCanvas* canv[9];
  for (Int_t i = 0; i < 9; i++)
  {
    TString canvName;
    canvName.Form("canv_%d", i);
    canv[i] = new TCanvas(canvName, canvName, 800, 1000);

    canv[i]->cd();
    histo[i]->Draw();
    histo[i]->SetLineWidth(2);
    TAxis* xAx = (TAxis*)histo[i]->GetXaxis();
    TAxis* yAx = (TAxis*)histo[i]->GetYaxis();
    xAx->SetTitle("born Energy, MeV");
    yAx->SetTitle("number");
    yAx->SetTitleSize(0.05);
    gPad->SetFrameLineWidth(5);
    gPad->SetGrid(2, 2);
  }

  TString outFileName = inputDir + "/" + "energy_out.root";

  TFile* outFile = new TFile(outFileName, "RECREATE");
  for (Int_t i = 0; i < 9; i++)
  {
    histo[i]->Write();
  }
}
