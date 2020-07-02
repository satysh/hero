void ReadSim(TString inputDir = "output_paralell/New_Data_Foil/Data/10", Int_t NTHR = 7) //New_Data_Foil/Data/baseGeometry10BGranularity
{
  //========== Histogram form =========================================
  Int_t  Nreaction=0;
  Int_t binNumb = 10000;
  Double_t minBin = 0.;
  Double_t maxBin = 3.;
  Double_t binStep = (maxBin - minBin)/binNumb;
  cout << "binStep: " << binStep << endl;
  TString fileName;
  //Namber Branches NUM = 18, old geometry NUM = 9
    Int_t NUM = 4;
  
    TH1F* histo[NUM], *length[NUM] , *Aenerg[NUM];
  
    for (Int_t i = 0; i < NUM; i++) {
    TString histName,lenghthistoName, Aenergy;
    histName.Form("hsito_%i", i);
    lenghthistoName.Form("length_%i", i);
    Aenergy.Form("Aenergy_%i", i);
    histo[i] = new TH1F(histName, histName, binNumb, minBin, maxBin);
    length[i] = new TH1F(lenghthistoName, lenghthistoName, 100, 0, 20.);
    Aenerg[i] = new TH1F(Aenergy, Aenergy, 1000, 0, 10.);
  }
  //===================================================================
  Int_t chekerNumberOfHistosMemersByPlates[4] = {0, 0, 0, 0 };
    
  
    // Loop over THR start
  for (Int_t THR = 1; THR <= NTHR; THR++) {
    cout << "============ THR: " << THR << " ============" << endl;
    // Reding root file open
    fileName.Form("%s/sim_%d.root", inputDir.Data(), THR);
    TFile* file = new TFile(fileName, "READ");
    if (file->IsZombie()) {
      cerr << "File read error" << endl;
      return;
    }
    // Finding the tree in root file
    TTree* tree = (TTree*)file->Get("HERO");
    if (!tree) {
      cerr << "Tree read error" << endl;
      return;
    }
    // Branches reding
    TBranch* Br[NUM],*Br_Track;
    Br[0] = tree->GetBranch("HEROdetectorvPlate_B10_xyz_u_fPoint");
    Br[1] = tree->GetBranch("HEROdetectorvPlate_B10_xyz_u_bPoint");
//     Br[2] = tree->GetBranch("HEROdetectorvPlate_B10_xyz_d_fPoint");
//     Br[3] = tree->GetBranch("HEROdetectorvPlate_B10_xyz_d_bPoint");
//     Br[4] = tree->GetBranch("HEROdetectorvPlate_B10_yxz_l_fPoint");
//     Br[5] = tree->GetBranch("HEROdetectorvPlate_B10_yxz_l_bPoint");
//     Br[6] = tree->GetBranch("HEROdetectorvPlate_B10_yxz_r_fPoint");
//     Br[7] = tree->GetBranch("HEROdetectorvPlate_B10_yxz_r_bPoint");
//     Br[8] = tree->GetBranch("HEROdetectorvPlate_B10_xzy_fPoint");
    
  Br[2] = tree->GetBranch("HEROdetectorvBoron_B10_xyz_u_fPoint");
  Br[3] = tree->GetBranch("HEROdetectorvBoron_B10_xyz_u_bPoint");
//   Br[2] = tree->GetBranch("HEROdetectorvBoron_B10_xyz_d_fPoint");
//   Br[3] = tree->GetBranch("HEROdetectorvBoron_B10_xyz_d_bPoint");
//   Br[4] = tree->GetBranch("HEROdetectorvBoron_B10_yxz_r_fPoint");
//   Br[5] = tree->GetBranch("HEROdetectorvBoron_B10_yxz_r_bPoint");
//   Br[6] = tree->GetBranch("HEROdetectorvBoron_B10_yxz_l_fPoint");
//   Br[7] = tree->GetBranch("HEROdetectorvBoron_B10_yxz_l_bPoint");
//   Br[8] = tree->GetBranch("HEROdetectorvBoron_B10_xzy_fPoint");
//     
    
    
    for (Int_t i = 0; i < NUM; i++)
      if (!Br[i]) {
        cerr << "i:" << i << " Branche read error." << endl;
      }
    Br_Track= tree->GetBranch("MCTrack");
        if (!Br_Track) {
        cerr  << "Br_Track Branche read error." << endl;
      }
    // Form and set adress to data arrays
    TClonesArray* Arr[NUM],*Arr_Track;
    for (Int_t i = 0; i < NUM; i++) {
      Arr[i] = new TClonesArray("HEROPoint");
      Br[i]->SetAddress(&Arr[i]);
    }
    Arr_Track=new TClonesArray("HEROMCTrack"); 
    Br_Track->SetAddress(&Arr_Track);
    
    UInt_t nEvents = tree->GetEntries();
    cout << "Number of events is: " << nEvents << endl;
    UInt_t THRNeutrons = 0;
    UInt_t THRAlphas = 0;
//     HEROMCTrack* Track;
//     TIter Titer(Arr_Track);

    // Loop over events start
    for (UInt_t i = 0; i < nEvents; i++) {
//       cout << "Event: " << i << endl;
//       Br_Track->GetEntry(i);
    HEROMCTrack* Track, *ETrack;
    TIter Titer(Arr_Track);
     Br_Track->GetEntry(i);
      // Loop over plates start
      for (Int_t j = 0; j < NUM; j++) {
//         cout << " Plate: " << j << endl;
        Br[j]->GetEntry(i);
        HEROPoint* Point;
        TIter Iter(Arr[j]);
        Int_t neutronsNum = 0;
        Int_t alphaNum = 0;

        // Loop over points
        while ((Point = (HEROPoint*)Iter.Next())) {
          if (Point->GetPID() == 1000020040)
            alphaNum++;
         
         if (Point->GetPID() == 2112)    neutronsNum++;

          // == - It writes neutrons, != - It writes alphas
//           if (Point->GetPID() == 1000020040){
//             continue;
          if (Point->GetPID() == 1000020040){
          Double_t curTimeIn = Point->GetTimeIn();
          Double_t curTimeOut = Point->GetTimeOut();
          Double_t dTime = curTimeOut - curTimeIn;
          //if (dTime < 1e-3) continue;
          Double_t ii = curTimeIn;
//           cout<<curTimeIn<<endl;
          // Loop fill hostograms
//           while (ii <= curTimeOut) {
//             //chekerNumberOfHistosMemersByPlates[j]++;
//             histo[j]->Fill((Float_t)ii);
//             ii += binStep;
//           }
//           histo[j]->Fill(curTimeOut); //This is factual lifetime of neuttrons, regarding the whole prototype!!!
          
            Int_t track_id= Point->GetTrackID();
//             cout <<Point->GetPID()<<endl;
//             cout<<track_id<<endl;
             Track =(HEROMCTrack*)Arr_Track->At(track_id);
            Int_t moth_Id =Track->GetMotherId(); //беру номер трэка родителя нейтрона
            if(moth_Id<0) continue;
            Int_t tpdg =Track->GetPdgCode(); //беру номер трэка родителя нейтрона
            Int_t moth_pdg = ((HEROMCTrack*)Arr_Track->At(moth_Id))->GetPdgCode();
            
//             cout<<tpdg<<"  "<<moth_pdg<<endl;
            Double_t En,Mn,Ea,Ma;
          if(moth_pdg==2112){ Nreaction++; 
             ETrack  = ((HEROMCTrack*)Arr_Track->At(moth_Id));
             En = ETrack->GetEnergy();
             Mn = ETrack->GetMass();
             histo[j]->Fill(En-Mn);
             ETrack  = ((HEROMCTrack*)Arr_Track->At(track_id));
//              Ea = ETrack->GetEnergy();
             Ma = ETrack->GetMass();
//              cout<<Ea<<endl;
             
          Double_t curPIn = Point-> GetPIn();
          Double_t Ea = sqrt(curPIn*curPIn + Ma*Ma);
                       
             Aenerg[j]->Fill(1000*(Ea-Ma));
//              cout<<E<<endl;
            }
            /*if(math_pdg ==2112 )*/
          Double_t X_in = Point->GetXIn();                
          Double_t Y_in = Point->GetYIn();
          Double_t Z_in = Point->GetZIn();
          Double_t X_out = Point->GetXOut();
          Double_t Y_out = Point->GetYOut();
          Double_t Z_out = Point->GetZOut();//For length histo !!!
//           Double_t Energy = Track->GetEnergy();
          if (moth_pdg==2112) length[j]->Fill(sqrt((X_out-X_in)*(X_out-X_in)+(Y_out-Y_in)*(Y_out-Y_in)+(Z_out-Z_in)*(Z_out-Z_in))*1e+4);
//             length[j]->Fill(Energy);
          chekerNumberOfHistosMemersByPlates[j]++;
          }
        } // loop over points end
//         cout << " neutronsNum: " << neutronsNum << endl;
//         cout << " alphaNum: " << alphaNum << endl;
        THRNeutrons += neutronsNum;
        THRAlphas += alphaNum;
      } // loop over plates end
    } // loop over Events end
    cout << "THRNeutrons: " << THRNeutrons << endl;
    cout << "THRAlphas: " << THRAlphas << endl;
    cout << "----------------------------------------" << endl;
cout<<Nreaction<<"  "<<"Nreaction"<<endl;
    // Close and clear
    file->Close();
    for (Int_t i = 0; i < NUM; i++) {
        Br[i] = NULL;
        Arr[i]->Clear();
    }
  } // loop over THR end

  for (Int_t i = 0; i < NUM; i++) {
    cout << "Plate: " << i << ", Enries: " << chekerNumberOfHistosMemersByPlates[i] << endl;
  }
  TCanvas* canv[NUM],* hcnv[NUM],*hcnvA[NUM] ;
  for (Int_t i = 0; i < NUM; i++) {
    TString canvName;
    canvName.Form("canv_%d", i);
    canv[i] = new TCanvas(canvName, canvName, 800, 1000);

    canv[i]->cd();
    length[i]->Draw();
    length[i]->SetLineWidth(2);
    TAxis* xAx = (TAxis*)length[i]->GetXaxis();
    TAxis* yAx = (TAxis*)length[i]->GetYaxis();
    xAx->SetTitle("lentgth, mkm");
    yAx->SetTitle("alpha number");
    yAx->SetTitleSize(0.05);
    gPad->SetFrameLineWidth(5);
    gPad->SetGrid(2, 2);
   
  }
  for (Int_t i = 0; i < NUM; i++) {
    TString hcnvN;
    hcnvN = Form("hcnvN%i",i); 
    hcnv[i] = new TCanvas(hcnvN,hcnvN,800, 1000);
    hcnv[i]->cd();
    histo[i]->Draw();
  }
  for (Int_t i = 0; i < NUM; i++) {
    TString hcnvAe;
    hcnvAe = Form("hcnvAe%i",i); 
    hcnvA[i] = new TCanvas(hcnvAe,hcnvAe,800, 1000);
    hcnvA[i]->cd();
    Aenerg[i]->Draw();
  }

  TString outFileName = inputDir + "/" + "histo_out.root";

  TFile* outFile = new TFile(outFileName, "RECREATE");
  for (Int_t i = 0; i < NUM; i++) {
    histo[i]->Write();
    length[i]->Write();
     Aenerg[i]->Write();
  }
}
