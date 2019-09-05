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

    TTree* tree = (TTree*)file->Get("HERO");
    if (!tree)
    {
        cerr << "Tree read error" << endl;
        return;
    }

    TBranch* Br = tree->GetBranch("HEROdetectorvCubPoint");
    if (!Br)
    {
        cerr << "Branche read error." << endl;
    }

    // Form and set adress to data arrays
    TClonesArray* Arr = new TClonesArray("HEROPoint");
    Br->SetAddress(&Arr);

    UInt_t nEvents = tree->GetEntries();
    cout << "Number of events is: " << nEvents << endl;
    UInt_t allNeutrons = 0;
    UInt_t allMksecNeutrons = 0;

    Int_t binNumb = 1000;
    Double_t minBin = 0.;
    Double_t maxBin = 60.;
    Double_t binStep = (maxBin - minBin)/Double_t(binNumb);
    cout << "binStep: " << binStep << endl;
    TH1F* histo = new TH1F("histo", "histo", binNumb, minBin, maxBin);
    TH1F* EHist = new TH1F("EHist", "EHist", 100000, 1E-6, 1000.);
    for (UInt_t i = 0; i < nEvents; i++)
    {
        cout << "Event: " << i << endl;
        Double_t minTimeIn = 100.0e10;
        Double_t maxTimeIn = 0.;
        Double_t minTimeOut = 100.0e10;
        Double_t maxTimeOut = 0.;
        Double_t minLiveNutr[4] = {100.0e10, 100.0e10, 100.0e10, 0.};
        Double_t maxLiveNutr[4] = {0., 0., 0., 0.};
        Br->GetEntry(i);
        HEROPoint* Point;
        TIter Iter(Arr);
        Int_t neutronsNum = 0;
        Int_t mksecNutrNum = 0;

        // Loop over points
        while ((Point = (HEROPoint*)Iter.Next()))
        {
            if (Point->GetPID() != 2112)
            {
                //cout << Point->GetPID() << endl;
                continue;
            }
        	neutronsNum++;
            Double_t curTimeIn = Point->GetTimeIn()*1e-3;
            Double_t curTimeOut = Point->GetTimeOut()*1e-3;
            Double_t curPIn = Point->GetPIn();
            Double_t curPOut = Point->GetPOut();
            Double_t curP = Point->GetP(0.01);
            Double_t curEIn = curPIn*curPIn/(2.*0.939);
            Double_t curEOut = curEOut*curEOut/(2.*0.939);
            Double_t curE = curP*curP/(2.*0.939);
            EHist->Fill(1E+3*curEIn);

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
                histo->Fill(maxBin);
                continue;
            }
            Double_t j = curTimeIn;
            while (j <= curTimeOut)
            {
            	histo->Fill((Float_t)j);
                j += binStep;
            	//cout << " j: " << j << endl;
            }
        } // loop over points end
        cout << "All neutrons number: " << neutronsNum << endl;
        cout << "minTimeIn: " << minTimeIn << ", maxTimeIn: " << maxTimeIn << " (mksec)" << endl;
        cout << "minTimeOut: " << minTimeOut << ", maxTimeOut: " << maxTimeOut << " (mksec)" << endl;
        cout << "mksec neutrons number: " << mksecNutrNum << endl;
        cout << "INFO: TrackID: " << minLiveNutr[3] << ", neutron with min life time: " << minLiveNutr[0] << ", timeIn: " << minLiveNutr[1] << ", timeOut: " << minLiveNutr[2] << " (mksec)" << endl;
        cout << "INFO: TrackID: " << maxLiveNutr[3] << ", neutron with max life time: " << maxLiveNutr[0] << ", timeIn: " << maxLiveNutr[1] << ", timeOut: " << maxLiveNutr[2] << " (mksec)" << endl;
        cout << "///////////////////////////////////" << endl;
        allNeutrons += neutronsNum;
        allMksecNeutrons += mksecNutrNum;
    }
    cout << "=======================================" << endl;
    cout << "allNeutrons: " << allNeutrons << endl;
    cout << "allMksecNeutrons: " << allMksecNeutrons << endl;
    TCanvas* c1 = new TCanvas("c1", "c1", 800, 1000);
    c1->cd();
    histo->Draw();
    histo->SetLineWidth(2);
    //TLegend* leg = histo->GetLegend();
    //leg->SetLineWidth(5);
    TAxis* xAx = (TAxis*)histo->GetXaxis();
    TAxis* yAx = (TAxis*)histo->GetYaxis();
    xAx->SetTitle("time, mks");
    yAx->SetTitle("neutrons number");
    yAx->SetTitleSize(0.05);
    gPad->SetFrameLineWidth(5);
    gPad->SetGrid(2, 2);

    TCanvas* c2 = new TCanvas("c2", "c2", 800, 1000);
    c2->cd();
    EHist->Draw();  
}
