#include <cmath>

using namespace std; 

int main() {
  /*
  TH1F *pedRMS1 = new TH1F("mux1pedrms","mux1pedrms",512,0,512);
  TH1F *pedRMS2 = new TH1F("mux2pedrms","mux2pedrms",512,0,512);
  TH1F *pedRMS3 = new TH1F("mux3pedrms","mux3pedrms",512,0,512);
  TH1F *pedRMS4 = new TH1F("mux4pedrms","mux4pedrms",512,0,512);
  TFile *filePedRMS1 = TFile::Open("pedfile","READ");
  TFile *filePedRMS2 = TFile::Open("pedfile","READ");
  TFile *filePedRMS3 = TFile::Open("pedfile","READ");
  TFile *filePedRMS4 = TFile::Open("pedfile","READ");
  pedRMS1 = (TH1F*) filePedRMS1->Get("pedRMS");
  pedRMS2 = (TH1F*) filePedRMS2->Get("pedRMS");
  pedRMS3 = (TH1F*) filePedRMS3->Get("pedRMS");
  pedRMS4 = (TH1F*) filePedRMS4->Get("pedRMS");
  */
  float sig[4] = {354.78, 456.232, 750.611, 607.121};
  float noise[4];
  float sigE[4] = {0.86859,1.08608,1.6165,1.34};
  float noiseE[4];
  float SN[4];
  const float mux[4] = {1,2,3,4};
  const int n = 4;
  int strip = 409;
  char fileName[256];
 
  float quadE[4];
  float zero[4];
  for (int i = 0; i < 4; ++i) {

    if (i ==0) filePedRMS = TFile::Open("../Peds/PedRMSRun24.root","READ");
    if (i ==1) filePedRMS = TFile::Open("../Peds/PedRMSRun25.root","READ");
    if (i ==2) filePedRMS = TFile::Open("../Peds/PedRMSRun29.root","READ");
    if (i ==3) filePedRMS = TFile::Open("../Peds/PedRMSRun35.root","READ");
    TH1F *pedRMS = (TH1F*) filePedRMS->Get("pedRMS");
    noise[i] = pedRMS->GetBinContent(360);
    noiseE[i] = pedRMS->GetBinError(360);
    SN[i] = sig[i]/noise[i];
    zero[i] = 0;
    quadE[i] = TMath::Sqrt(pow(noiseE[i]/noise[i],2)+pow(sigE[i]/sig[i],2));
  }


  TGraphErrors *g1 = new TGraphErrors(n,mux,SN,zero,quadE);
  TCanvas *c2 = new TCanvas("c2","c2");
  g1->SetTitle("Signal/Noise for Mux 1-4; MUXGAIN; Mean ADC / Pedestal Width");
  char g1Name[256];
  g1->Draw("AP");
  g1->SetMarkerStyle(31);
  //g1->SetMarkerColor(4);
  g1->SetMarkerSize(2);
  sprintf(g1Name,"../IntMux1/SNCalibration.root");
  c2->SaveAs(g1Name); 
  delete c2; 
  return 0;
}
