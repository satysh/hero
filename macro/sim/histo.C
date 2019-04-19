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

    Int_t binNumb = 100;
    Double_t minBin = 0.;
    Double_t maxBin = 100.;
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
        Double_t minLiveNutr[3] = {100.0e10, 100.0e10, 100.0e10};
        Double_t maxLiveNutr[3] = {0., 0., 0.};
        Br->GetEntry(i);
        OLVPoint* Point;
        TIter Iter(Arr);
        Int_t neutronsNum = 0;
        Int_t msecNutrNum = 0;
        // Loop over points
        while ((Point = (OLVPoint*)Iter.Next()))
        {
        	neutronsNum++;
            Double_t curTimeIn = Point->GetTimeIn();
            Double_t curTimeOut = Point->GetTimeOut();
            if ((curTimeOut - curTimeIn) < minLiveNutr[0])
            {
                minLiveNutr[0] = (curTimeOut - curTimeIn);
                minLiveNutr[1] = curTimeIn*1e-3;
                minLiveNutr[2] = curTimeOut*1e-3;
            }
            if ((curTimeOut - curTimeIn) > maxLiveNutr[0])
            {
                maxLiveNutr[0] = (curTimeOut - curTimeIn);
                maxLiveNutr[1] = curTimeIn*1e-3;
                maxLiveNutr[2] = curTimeOut*1e-3;
            }
            if (curTimeIn < minTimeIn) { minTimeIn = curTimeIn; }
            if (curTimeIn > maxTimeIn) { maxTimeIn = curTimeIn; }
            if (curTimeOut < minTimeOut) { minTimeOut = curTimeOut; }
            if (curTimeOut > maxTimeOut) { maxTimeOut = curTimeOut; }
            if ((curTimeOut - curTimeIn)*1e-3 >= 1.) { /*cout << (curTimeOut - curTimeIn) << endl;*/ msecNutrNum++; }
            //else continue;
            //cout << "timeIn: " << setw(8) << curTimeIn << ", " << "timeOut: " << curTimeOut << endl;
            for (Int_t j = (Int_t)curTimeIn*1e-3; j <= (Int_t)curTimeOut*1e-3; j+=(Int_t)binStep)
            {
            	histo->Fill(j);
            	//cout << " j: " << j << endl;
            }
        } // loop over points end
        cout << "All nutrons number: " << neutronsNum << endl;
        cout << "minTimeIn: " << minTimeIn*1e-3 << ", maxTimeIn: " << maxTimeIn*1e-3 << " (msec)" << endl;
        cout << "minTimeOut: " << minTimeOut*1e-3 << ", maxTimeOut: " << maxTimeOut*1e-3 << " (msec)" << endl;
        cout << "msec nutrons number: " << msecNutrNum << endl;
        cout << "INFO: nutron with min life time: " << minLiveNutr[0]*1e-3 << ", timeIn: " << minLiveNutr[1] << ", timeOut: " << minLiveNutr[2] << " (msec)" << endl;
        cout << "INFO: nutron with max life time: " << maxLiveNutr[0]*1e-3 << ", timeIn: " << maxLiveNutr[1] << ", timeOut: " << maxLiveNutr[2] << " (msec)" << endl;
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
