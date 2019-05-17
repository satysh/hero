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

    TBranch* Br = tree->GetBranch("OLVdetectorvCubPoint");
    if (!Br)
    {
        cerr << "Branche read error." << endl;
    }

    // Form and set adress to data arrays
    TClonesArray* Arr = new TClonesArray("OLVPoint");
    Br->SetAddress(&Arr);

    UInt_t nEvents = tree->GetEntries();
    cout << "Number of events is: " << nEvents << endl;

    Int_t binNumb = 10000;
    Double_t minBin = 0.;
    Double_t maxBin = 60.;
    Double_t binStep = (maxBin - minBin)/Double_t(binNumb);
    cout << "binStep: " << binStep << endl;
    TH1F* histo = new TH1F("histo", "histo", binNumb, minBin, maxBin);
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
        OLVPoint* Point;
        TIter Iter(Arr);
        Int_t neutronsNum = 0;
        Int_t msecNutrNum = 0;
        // Loop over points
        while ((Point = (OLVPoint*)Iter.Next()))
        {
            if (Point->GetPID() != 2112)
            {
                //cout << Point->GetPID() << endl;
                continue;
            }
        	neutronsNum++;
            Double_t curTimeIn = Point->GetTimeIn()*1e-3;
            Double_t curTimeOut = Point->GetTimeOut()*1e-3;
            Double_t dTime = curTimeOut - curTimeIn;
            if ((dTime) < minLiveNutr[0])
            {
                minLiveNutr[0] = (dTime);
                minLiveNutr[1] = curTimeIn;
                minLiveNutr[2] = curTimeOut;
                minLiveNutr[3] = Point->GetTrackID();
            }
            if ((dTime) > maxLiveNutr[0])
            {
                maxLiveNutr[0] = (dTime);
                maxLiveNutr[1] = curTimeIn;
                maxLiveNutr[2] = curTimeOut;
                maxLiveNutr[3] = Point->GetTrackID();
            }
            if (curTimeIn < minTimeIn) { minTimeIn = curTimeIn; }
            if (curTimeIn > maxTimeIn) { maxTimeIn = curTimeIn; }
            if (curTimeOut < minTimeOut) { minTimeOut = curTimeOut; }
            if (curTimeOut > maxTimeOut) { maxTimeOut = curTimeOut; }
            if ((dTime) >= 1.) { /*cout << (dTime) << endl;*/ msecNutrNum++; }
            else continue;
            //cout << "timeIn: " << setw(8) << curTimeIn << ", " << "timeOut: " << curTimeOut << endl;z
            Double_t j = curTimeIn;
            while (j <= curTimeOut)
            {
            	histo->Fill((Float_t)j);
                j += binStep;
            	//cout << " j: " << j << endl;
            }
        } // loop over points end
        cout << "All neutrons number: " << neutronsNum << endl;
        cout << "minTimeIn: " << minTimeIn << ", maxTimeIn: " << maxTimeIn << " (msec)" << endl;
        cout << "minTimeOut: " << minTimeOut << ", maxTimeOut: " << maxTimeOut << " (msec)" << endl;
        cout << "msec neutrons number: " << msecNutrNum << endl;
        cout << "INFO: TrackID: " << minLiveNutr[3] << ", neutron with min life time: " << minLiveNutr[0] << ", timeIn: " << minLiveNutr[1] << ", timeOut: " << minLiveNutr[2] << " (msec)" << endl;
        cout << "INFO: TrackID: " << maxLiveNutr[3] << ", neutron with max life time: " << maxLiveNutr[0] << ", timeIn: " << maxLiveNutr[1] << ", timeOut: " << maxLiveNutr[2] << " (msec)" << endl;
        cout << "///////////////////////////////////" << endl;
    }

    histo->Draw();
    histo->SetLineWidth(2);
    //TLegend* leg = histo->GetLegend();
    //leg->SetLineWidth(5);
    TAxis* xAx = (TAxis*)histo->GetXaxis();
    TAxis* yAx = (TAxis*)histo->GetYaxis();
    xAx->SetTitle("time, ms");
    yAx->SetTitle("neutrons number");
    yAx->SetTitleSize(0.05);
    gPad->SetFrameLineWidth(5);
    gPad->SetGrid(2, 2);
}
