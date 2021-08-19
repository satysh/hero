void fit5Points()
{
  Double_t x[] = {3.3, 3.49, 3.69, 4.12, 4.59};
  Double_t y[] = {1227.96, 1146.25, 998.65, 866.14, 748.18};
  TGraph* graph = new TGraph(5, x, y);
  TF1* func = new TF1("func", "[0]*x+[1]", 3.3, 4.59);
  graph->Fit("func");
  graph->SetLineWidth(4);
  graph->SetMarkerSize(2);
  graph->Draw("ALP*");
}
