void histo(TString inputDir = "output")
{
    TString fileName;
    fileName.Form("%s/sim.root", inputDir.Data());
    TFile* file = new TFile(fileName, "READ");
    if (file->IsZombie())
    {
        cerr << "File read error" << endl;
        return;
    }

    TTree* tree = (TTree*)file->Get("OLV");
    if (!tree)
    {
        cerr << "Tree read error" << endl;
        return;
    }

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
    UInt_t allNeutrons = 0;
    UInt_t allMksecNeutrons = 0;

    Int_t binNumb = 1000;
    Double_t minBin = 0.;
    Double_t maxBin = 60.;
    Double_t binStep = (maxBin - minBin)/Double_t(binNumb);
    cout << "binStep: " << binStep << endl;
    TH1F* histo[9];
    for (Int_t i = 0; i < 9; i++)
    {	
    	TString histName;
    	histName.Form("hsito_%d", i);
    	histo[i] = new TH1F(histName, histName, binNumb, minBin, maxBin);
    }
    for (UInt_t i = 0; i < nEvents; i++)
    {
        cout << "Event: " << i << endl;
        for (Int_t j = 0; j < 9; j++)
        {   
            cout << "Plate: " << j << endl;
        	Double_t minTimeIn = 100.0e10;
        	Double_t maxTimeIn = 0.;
        	Double_t minTimeOut = 100.0e10;
        	Double_t maxTimeOut = 0.;
        	Double_t minLiveNutr[4] = {100.0e10, 100.0e10, 100.0e10, 0.};
        	Double_t maxLiveNutr[4] = {0., 0., 0., 0.};
        	Br[j]->GetEntry(i);
        	OLVPoint* Point;
        	TIter Iter(Arr[j]);
        	Int_t neutronsNum = 0;
        	Int_t mksecNutrNum = 0;
            Int_t alphaNum = 0;
        	// Loop over points
        	while ((Point = (OLVPoint*)Iter.Next()))
        	{
	            if (Point->GetPID() != 1000020040)
            	{
	                //cout << Point->GetPID() << endl;
                    alphaNum++;
                    continue;
        	    }
                else
                    neutronsNum++;

        		neutronsNum++;
            	Double_t curTimeIn = Point->GetTimeIn()*1e-3;
            	Double_t curTimeOut = Point->GetTimeOut()*1e-3;

            	Double_t dTime = curTimeOut - curTimeIn;
            	if ((dTime) < minLiveNutr[0])
            	{
                	minLiveNutr[0] = dTime;
                	minLiveNutr[1] = curTimeIn;
                	minLiveNutr[2] = curTimeOut;
                	minLiveNutr[3] = Point->GetTrackID();
            	}
            	if ((dTime) > maxLiveNutr[0])
            	{
                	maxLiveNutr[0] = dTime;
                	maxLiveNutr[1] = curTimeIn;
                	maxLiveNutr[2] = curTimeOut;
                	maxLiveNutr[3] = Point->GetTrackID();
            	}
            	if (curTimeIn < minTimeIn) { minTimeIn = curTimeIn; }
            	if (curTimeIn > maxTimeIn) { maxTimeIn = curTimeIn; }
            	if (curTimeOut < minTimeOut) { minTimeOut = curTimeOut; }
            	if (curTimeOut > maxTimeOut) { maxTimeOut = curTimeOut; }
            	if ((dTime) >= 2.) { /*cout << (dTime) << endl;*/ mksecNutrNum++; }
            	//else continue;
            	//cout << "timeIn: " << setw(8) << curTimeIn << ", " << "timeOut: " << curTimeOut << endl;z
            	if (dTime > maxBin)
            	{
	                histo[j]->Fill(maxBin);
    	            continue;
        	    }
            	Double_t ii = curTimeIn;
            	while (ii <= curTimeOut)
            	{	
            		histo[j]->Fill((Float_t)ii);
                	ii += binStep;
            	}
        	} // loop over points end
            
        	cout << "All neutrons number: " << neutronsNum << endl;
            cout << "Alpha number is: " << alphaNum << endl;
        	cout << "minTimeIn: " << minTimeIn << ", maxTimeIn: " << maxTimeIn << " (mksec)" << endl;
        	cout << "minTimeOut: " << minTimeOut << ", maxTimeOut: " << maxTimeOut << " (mksec)" << endl;
        	cout << "mksec neutrons number: " << mksecNutrNum << endl;
        	cout << "INFO: TrackID: " << minLiveNutr[3] << ", neutron with min life time: " << minLiveNutr[0] << ", timeIn: " << minLiveNutr[1] << ", timeOut: " << minLiveNutr[2] << " (mksec)" << endl;
        	cout << "INFO: TrackID: " << maxLiveNutr[3] << ", neutron with max life time: " << maxLiveNutr[0] << ", timeIn: " << maxLiveNutr[1] << ", timeOut: " << maxLiveNutr[2] << " (mksec)" << endl;
        	cout << "///////////////////////////////////" << endl;
        	allNeutrons += neutronsNum;
        	allMksecNeutrons += mksecNutrNum;
    	}
    }
    cout << "=======================================" << endl;
    cout << "allNeutrons: " << allNeutrons << endl;
    cout << "allMksecNeutrons: " << allMksecNeutrons << endl;
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
    	xAx->SetTitle("time, mks");
    	yAx->SetTitle("neutrons number");
    	yAx->SetTitleSize(0.05);
    	gPad->SetFrameLineWidth(5);
    	gPad->SetGrid(2, 2);	
    } 
	
	TFile* outFile = new TFile("histo_out.root", "RECREATE");
	for (Int_t i = 0; i < 9; i++)
	{
		histo[i]->Write();
	}
}
