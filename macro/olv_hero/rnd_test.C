void rnd_test() {
  TH1F* h = new TH1F("h", "h", 1000, 900., 1100.);

  Int_t n = 1000000;
  for (Int_t i = 0; i < n; i++) {
    Double_t rnd = gRandom->Gaus(1000., 10.);
    h->Fill(rnd);
  }
  h->Draw();
}
