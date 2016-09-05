#include <iostream>

using namespace std;


int main() {

  TFile f("/home/mphipps/rcdaqAnalysis/ExtMux4/TreeScanMux4EXTRun275.root");
  TCanvas *c1 = new TCanvas("c1","c1");
  Hit->Draw("MaxTimeBin","PadNo ==19");
  c1->SaveAs("../ExtMux4/Mux4Pad19Fe55.root");
  c1->SaveAs("../ExtMux4/Mux4Pad19Fe55.png");
  delete c1;
  TCanvas *c1 = new TCanvas("c1","c1");
  Hit->Draw("MaxCharge","PadNo > 360");
  c1->SaveAs("SNPlots/Mux1AllPadsFe55.root");
  c1->SaveAs("SNPlots/Mux1AllPadsFe55.png");
  delete c1;

  TFile f2("../Trees/Trees/TreeScanMux2Run38.root");
  TCanvas *c1 = new TCanvas("c1","c1");
  Hit->Draw("MaxCharge","PadNo ==464");
  c1->SaveAs("SNPlots/Mux2Pad464Fe55.root");
  c1->SaveAs("SNPlots/Mux2Pad464Fe55.png");
  delete c1;
  TCanvas *c1 = new TCanvas("c1","c1");
  Hit->Draw("MaxCharge","PadNo > 360");
  c1->SaveAs("SNPlots/Mux2AllPadsFe55.root");
  c1->SaveAs("SNPlots/Mux2AllPadsFe55.png");
  delete c1;


  TFile f3("../Trees/Trees/TreeScanMux3Run44.root");
  TCanvas *c1 = new TCanvas("c1","c1");
  Hit->Draw("MaxCharge","PadNo ==464");
  c1->SaveAs("SNPlots/Mux3Pad464Fe55.root");
  c1->SaveAs("SNPlots/Mux3Pad464Fe55.png");
  delete c1;
  TCanvas *c1 = new TCanvas("c1","c1");
  Hit->Draw("MaxCharge","PadNo > 360");
  c1->SaveAs("SNPlots/Mux3AllPadsFe55.root");
  c1->SaveAs("SNPlots/Mux3AllPadsFe55.png");
  delete c1;


  TFile f4("../Trees/Trees/TreeScanMux4Run36.root");
  TCanvas *c1 = new TCanvas("c1","c1");
  Hit->Draw("MaxTimeBin","MaxCharge > 1900 && MaxCharge < 2200");
  c1->SaveAs("SNPlots/Mux4MaxTimeBin.root");
  c1->SaveAs("SNPlots/Mux4MaxTimeBin.png");
  delete c1;
  TCanvas *c1 = new TCanvas("c1","c1");
  Hit->Draw("MaxCharge","PadNo > 360");
  c1->SaveAs("SNPlots/Mux4AllPadsFe55.root");
  c1->SaveAs("SNPlots/Mux4AllPadsFe55.png");
  delete c1;
  return 0;
}
