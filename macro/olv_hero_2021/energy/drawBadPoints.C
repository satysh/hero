void drawBadPoints()
{
  Double_t x[] = {3.3, 3.49, 3.69, 4.12, 4.59};
  Double_t y[] = {1227.96, 1146.25, 998.65, 866.14, 748.18};
  TGraph* graph = new TGraph(5, x, y);
  graph->SetLineWidth(4);
  graph->SetMarkerSize(2);
  TF1* func = new TF1("func", "[0]*x+[1]", 3.3, 4.59);
  func->SetLineColor(kBlue);
  graph->Fit("func");

  ifstream fin("input/flux.txt");
  if (!fin.is_open()) {
    cerr << "Can't read input/flux.txt" << endl;
    return;
  }

  Int_t nPoints = 70; // number of the points in flux.txt
  Double_t *inputX = new Double_t[nPoints];
  Double_t *inputY = new Double_t[nPoints];
  for (Int_t i=0; i<nPoints; i++) {
    fin >> inputX[i] >> inputY[i];
  }

  TGraph *inputGraph = new TGraph(nPoints, inputX, inputY);
  inputGraph->SetLineColor(kRed);
  inputGraph->SetLineWidth(4);

  Double_t new2pX[] = {3.69, 4.12};
  Double_t new2pY[] = {1052.43829, 892.62492};
  TGraph *new2Points = new TGraph(2, new2pX, new2pY);
  new2Points->SetMarkerStyle(20);
  new2Points->SetLineColor(kYellow);

  TMultiGraph *mg  = new TMultiGraph();
  //mg->Add(graph, "PL*");
  mg->Add(inputGraph, "PL.");
  //mg->Add(new2Points, "P");
  mg->Draw("AP");
  mg->GetXaxis()->SetTitle("Energy [GeV]");
  mg->GetYaxis()->SetTitle("flux");
  mg->SetTitle("flux");

  TLegend *lgnd = new TLegend();
  //lgnd->AddEntry(graph, "5 points: bad region", "p");
  lgnd->AddEntry(inputGraph, "input flux points", "l");
  /*lgnd->AddEntry(func, "5 points fiting function", "l");
  lgnd->AddEntry(new2Points, "new 2 points", "p");*/
  lgnd->Draw();
  gPad->SetGrid(2,2);
}
