#include <iostream>

using namespace std;

int main() {

  float scale[4];
  float muxMean[4] = {381.809,491.917,817.39,660.16};
  TH1F* h[4];
  for (int i=0; i < 4; ++i) {
    scale[i] = muxMean[2]/muxMean[i];
    
    TFile *file;
    if (i ==0) file = TFile::Open("SNPlots/Mux1AllPadsFe55.root","READ");
    if (i ==1) file = TFile::Open("SNPlots/Mux2AllPadsFe55.root","READ");
    if (i ==2) file = TFile::Open("SNPlots/Mux3AllPadsFe55.root","READ");
    if (i ==3) file = TFile::Open("SNPlots/Mux4AllPadsFe55.root","READ");
    TCanvas *c1 = (TCanvas*) file->Get("c1;1");    
    h[i] = (TH1F*) c1->GetPrimitive("htemp");
    
    TAxis *a = h[i]->GetXaxis();
    a->Set(a->GetNbins(),a->GetXmin()*scale[i],a->GetXmax()*scale[i]);
    
  }
   
  TCanvas *c = new TCanvas("c","c");
  h[0]->SetTitle("Muxgain Dynamic Range: Scaled Charge Spectrums; ADC; Counts");
  h[0]->Draw();
  c->Update();

  h[1]->SetLineStyle(2);
  h[1]->SetLineColor(2);
  h[1]->Draw("same");
  h[2]->SetLineStyle(6);
  h[2]->SetLineColor(4);
  h[2]->Draw("same");
  h[3]->SetLineStyle(9);
  h[3]->SetLineColor(8);
  h[3]->Draw("same");

  TLegend *leg = new TLegend(0.46,0.7,0.7,0.9);
  leg->SetTextFont(72);
  leg->SetTextSize(0.04);
  leg->SetFillColor(0);
  leg->AddEntry(h[0],"Muxgain 1","ple");
  leg->AddEntry(h[1],"Muxgain 2","ple");
  leg->AddEntry(h[2],"Muxgain 3","ple");
  leg->AddEntry(h[3],"Muxgain 4","ple");
  leg->Draw();

  char Name[256];
  sprintf(Name,"SNPlots/dynamicRange.root");
  c->SaveAs(Name); 
  return 0;
}
