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
void draw()
{
  TString fileName;
  fileName.Form("output/sim.root", inputDir.Data());
  TFile* file = new TFile(fileName, "READ");
  if (file->IsZombie()) {
      cerr << "File read error" << endl;
      return;
  }

  TTree* tree = (TTree*)file->Get("HERO");
  if (!tree) {
    cerr << "Get Tree error!" << endl;
    return;
  }
  UInt_t nEvents = tree->GetEntries();
  cout << "Number of events is: " << nEvents << endl;

  TBranch* branch = tree->GetBranch("rogovDetectorvMedPoint");
  if (!branch) {
    cerr << "Get Branch error!" << endl;
    return;
  }

  TClonesArray* Arr = new TClonesArray("HEROPoint");
  branch->SetAddress(Arr);

  for (UInt_t i = 0; i < nEvents; i++) {
    cout << "Event: " << i << endl;
    branch->GetEntry(i);
    HEROPoint* Point;
    TIter Iter(Arr);
    while ((Point = (HEROPoint*)Iter.Next())) {
      if (Point->GetPID() == GetPdgCode(26, 56)) {
        cout << "Fe" << endl;
      }
      else if (Point->GetPID() == 2112) {
        cout << "neutron" << endl;
      }
    }

  }
}

int GetPdgCode(const int Z, const int A) {
  if (Z == 1 && A == 1) return 2212;
  return 1000000000 + Z*10000 + A*10;
}
