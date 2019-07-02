void histo_paralell(TString inputDir = "output_paralell", Int_t NTHR = 3)
{
  //========== Histogram form =========================================
  Int_t binNumb = 1000000;
  Double_t minBin = 0.;
  Double_t maxBin = 16.;
  Double_t binStep = (maxBin - minBin)/Double_t(binNumb);
  cout << "binStep: " << binStep << endl;
  TString fileName;
  TH1F* histo[9], *sr_histo[9],*out_histo[9];
  for (Int_t i = 0; i < 9; i++)
  {
    TString histName;
    histName.Form("Detector_%d", i);
    TString sr_histName;
    sr_histName.Form("sr_hsito_%d", i);
    TString out_histName;
    out_histName.Form("out_histo%d", i);
    histo[i] = new TH1F(histName, histName, binNumb, minBin, maxBin);
    sr_histo[i] = new TH1F(sr_histName, sr_histName, 100000, 0, 16);
    out_histo[i] = new TH1F(out_histName, out_histName, 1000000, 0, 16);
  }
  //===================================================================
  Int_t chekerNumberOfHistosMemersByPlates[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

  // Loop over THR start
  for (Int_t THR = 1; THR <= NTHR; THR++)
  {
    cout << "============ THR: " << THR << " ============" << endl;
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
    UInt_t THRNeutrons = 0;
    UInt_t THRAlphas = 0;

    // Loop over events start
    for (UInt_t i = 0; i < nEvents; i++)
    {
      cout << "Event: " << i << endl;

      // Loop over plates start
      for (Int_t j = 0; j < 9; j++)
      {
        cout << " Plate: " << j << endl;
        Br[j]->GetEntry(i);
        OLVPoint* Point;
        TIter Iter(Arr[j]);
        Int_t neutronsNum = 0;
        Int_t alphaNum = 0;

        // Loop over points
        while ((Point = (OLVPoint*)Iter.Next()))
        {
	        if (Point->GetPID() == 1000020040)
            alphaNum++;
          else
            neutronsNum++;

          // == - It writes neutrons, != - It writes alphas
          if (Point->GetPID() == 1000020040)
            continue;

          Double_t curTimeIn = Point->GetTimeIn()*1e-3;
          Double_t curTimeOut = Point->GetTimeOut()*1e-3;
          Double_t dTime = curTimeOut - curTimeIn;
          //if (dTime < 1e-3) continue;
          Double_t ii = curTimeIn;
          // Loop fill hostograms
//           sr_histo[j]->StatOverflows(kTRUE); 
          sr_histo[j]->Fill(dTime);
//           out_histo[j]->StatOverflows(kTRUE); 
          out_histo[j]->Fill(curTimeOut);
          while (ii <= curTimeOut)
          {
            //chekerNumberOfHistosMemersByPlates[j]++;
//               histo[j]->StatOverflows(kTRUE);
            histo[j]->Fill((Float_t)ii);
            ii += binStep;
          }
          chekerNumberOfHistosMemersByPlates[j]++;
        } // loop over points end
        
        
        cout << " neutronsNum: " << neutronsNum << endl;
        cout << " alphaNum: " << alphaNum << endl;
        THRNeutrons += neutronsNum;
        THRAlphas += alphaNum;
      } // loop over plates end
    } // loop over Events end
    cout << "THRNeutrons: " << THRNeutrons << endl;
    cout << "THRAlphas: " << THRAlphas << endl;
    cout << "----------------------------------------" << endl;

    // Close and clear
    file->Close();
    for (Int_t i = 0; i < 9; i++)
    {
        Br[i] = NULL;
        Arr[i]->Clear();
    }
  } // loop over THR end

  for (Int_t i = 0; i < 9; i++)
  {
    cout << "Plate: " << i << ", Enries: " << chekerNumberOfHistosMemersByPlates[i] << endl;
  }
  TCanvas* canv[9];
  for (Int_t i = 0; i < 9; i++)
  {
/*    TString sr_canvName;
    sr_canvName.Form("sr_canv_%d", i);
    sr_canv[i] = new TCanvas(sr_canvName, sr_canvName, 800, 1000);
    sr_canv[i]->cd();
    sr_histo->Draw(); */ 
    
    TString canvName;
    canvName.Form("canv_%d", i);
    canv[i] = new TCanvas(canvName, canvName, 800, 1000);

    canv[i]->cd();
    histo[i]->Draw();
    histo[i]->SetLineWidth(2);
    TAxis* xAx = (TAxis*)histo[i]->GetXaxis();
    TAxis* yAx = (TAxis*)histo[i]->GetYaxis();
    xAx->SetTitle("time, us");
    yAx->SetTitle("neutrons");
    yAx->SetTitleSize(0.06);
    gPad->SetFrameLineWidth(5);
    gPad->SetGrid(2, 2);
    
  }

  TString outFileName = inputDir + "/" + "histo_out.root";

  TFile* outFile = new TFile(outFileName, "RECREATE");
  for (Int_t i = 0; i < 9; i++)
  {
      cout<<"MEAN"<<"   "<<i<<"     "<<out_histo[i]->GetMean()<<"    "<<histo[i]->GetMean()<<endl;  
    histo[i]->Write();
    sr_histo[i]->Write();
    out_histo[i]->Write();
  }
}
