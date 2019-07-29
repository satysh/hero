void checker(TString inputdir = "output")
{
    TString readFileName = inputdir + "/sim.root";
    TFile* file = new TFile(readFileName, "READ");
    if (file->IsZombie()) {
        cerr << readFileName << " can't be opened!" << endl;
        return;
    }

    TTree* tree = (TTree*)file->Get("OLV");
    if (!tree) {
        cerr << "Tree OLV is not found!" << endl;
        return;
    }

    TBranch* br[3];
    for (Int_t i = 0; i < 3; i++) {
        TString branchName;
        branchName.Form("OLVdetectorbox%dPoint", i);
        br[i] = tree->GetBranch(branchName);
        if (!br[i]) {
            cerr << "Branch " << branchName << " read error!" << endl;
            return;
        }
    }


    TClonesArray* Arr[3];
    for (Int_t i = 0; i < 3; i++) {
        Arr[i] = new TClonesArray("OLVPoint");
        br[i]->SetAddress(&Arr[i]);
    }

    UInt_t nEvents = tree->GetEntries();
    cout << "Number of events is: " << nEvents << endl;

    for (Int_t i = 0; i < nEvents; i++) {
        cout << "Event : " << i << endl;
        Double_t timePrev = 0.;
        for (Int_t j = 0; j < 3; j++) {
            cout << " toF: " << j << endl;
            br[j]->GetEntry(i);
            OLVPoint* Point = NULL;
            TIter Iter(Arr[j]);
            while ((Point = (OLVPoint*)Iter.Next())) {
                Double_t timeIn = Point->GetTimeIn();
                Double_t timeOut = Point->GetTimeOut();
                cout << "  TimeIn: " << timeIn << ", TimeOut: " << timeOut << ", dT = " << fabs(timeIn - timePrev) << endl;
                timePrev = timeOut;
            }
        }
    }
}
