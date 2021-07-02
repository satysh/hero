TGraph* energyCDFGr = NULL;
TGraph* energyInvCDFGr = NULL;

Double_t EnergyCDF(Double_t *x, Double_t *par) {
  return energyCDFGr->Eval(x[0]);
}

Double_t EnergyInvCDF(Double_t *x, Double_t *par) {
  return energyInvCDFGr->Eval(x[0]);
}

void energy()
{
  TString path = "input/flux.txt";
  std::ifstream f;
  f.open(path.Data());
  if (!f.is_open()) {
    cerr << "Can't open file " << path << endl;
    return;
  }

  Int_t nPoints = std::count(std::istreambuf_iterator<char>(f),
                               std::istreambuf_iterator<char>(), '\n');
  f.seekg(0, std::ios::beg);
  TVectorD energy(nPoints);
  TVectorD flux(nPoints);

   cout << "nPoints = " << nPoints << endl;

  Int_t i = 0;
  while (!f.eof()) {
    if (i == nPoints) break;
      f >> energy(i) >> flux(i);
      //cout << i << ": " << energy(i) << " " << flux(i) << endl;
      i++;
  }

  energyCDFGr = new TGraph(energy, flux);
  energyInvCDFGr = new TGraph(flux, energy);

  TF1* energyCDF = new TF1("thetaCDF", EnergyCDF, 0.082, 20.76, 0);
  TF1* energyInvCDF = new TF1("thetaInvCDF", EnergyInvCDF, 80.73, 7808.99, 0);
  Double_t param = 6600.23;
  Double_t res = energyInvCDF->Eval(param);
  cout << res << endl;
  cout << energyCDF->Eval(res) << endl;


  TH1F* hist = new TH1F("hist", "hist", 10000, 0., 21.);
  TRandom rnd;
  for (Int_t i=0; i<1000000; i++) {
    Double_t rndflux = rnd.Uniform(80.73, 7808.99);
    Double_t val = energyInvCDF->Eval(rndflux);
    hist->Fill(val);

  }

  TCanvas* canv = new TCanvas("canv", "canv");
  canv->Divide(2, 1);
  canv->cd(1);
  energyCDF->Draw();
  //energyInvCDF->Draw();
  //energyCDFGr->Draw();
  //energyInvCDFGr->Draw();
  canv->cd(2);
  hist->Draw();
}
