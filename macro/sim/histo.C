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

    Int_t binNumb = 2500;
    Double_t minBin = 0.;
    Double_t maxBin = 2500.;
    Double_t step = (maxBin - minBin) / (Double_t)binNumb;
    TH1F* histo = new TH1F("histo", "histo", binNumb, minBin, maxBin);
    Double_t minTime = 100.0e10;
    Double_t maxTime = 0.;
    for (UInt_t i = 0; i <= nEvents; i++)
    {
        cout << "i: " << i << endl;
        Br->GetEntry(i);
        OLVPoint* Point;
        TIter Iter(Arr);
        Int_t neutronsNum = 0;
        // Loop over points 
        while ((Point = (OLVPoint*)Iter.Next())) 
        {
        	neutronsNum++;
            Double_t curTimeIn = Point->GetTimeIn();
            Double_t curTimeOut = Point->GetTimeOut();
          
            for (Int_t j = (Int_t)curTimeIn; j <= (Int_t)curTimeOut; j+=1)
            {
            	histo->Fill(j);
            }
            cout << "timeIn: " << setw(8) << curTimeIn << ", " << "timeOut: " << curTimeOut << endl;
            if (curTimeIn < minTime)
                minTime = curTimeIn;
            if (curTimeOut > maxTime)
                maxTime = curTimeOut;
        } // loop over points end
        cout << "num: " << neutronsNum << endl;
    }
    histo->Draw();
    histo->SetLineWidth(2);
    //TLegend* leg = histo->GetLegend();
    //leg->SetLineWidth(5);
    TAxis* xAx = (TAxis*)histo->GetXaxis();
    TAxis* yAx = (TAxis*)histo->GetYaxis();
    xAx->SetTitle("time, ns");
    yAx->SetTitle("neutrons number");
    yAx->SetTitleSize(0.05);
    gPad->SetFrameLineWidth(5);
    cout << "min: " << minTime << ", max: " << maxTime << endl;
}
