#include <iostream>
#include "TStyle.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "TAxis.h"

int Comp() {
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);
  //gStyle->SetTickLength(0.02, "xyz");


  Float_t NiCompICP[5] = {0.403,0.44,0.37,0.038,0};
  Float_t errorsNiICP[5] = {0.0024,0.0017,0.0027,0.002,0.0};
  Float_t CoCompICP[5] = {0.0,0.0,0.0,0.0,0.08};
  Float_t errorsCoICP[5] = {0.000,0.000,0.000,0.000,0.0};
  //Float_t NaCompICP[4] = {0.0,0.0,0.0,0.0,0.006};
  //Float_t errorsNaICP[4] = {0.0024,0.0017,0.0027,0.002};
  Float_t SiCompICP[5] = {0.0,0.0,0.0,0.0,0.016};
  Float_t errorsSiICP[5] = {0.00,0.00,0.00,0.0,0.0};
  Float_t CrCompICP[5] = {0.376,0.416,0.351,0.318, 0.23};
  Float_t errorsCrICP[5] = {0.002,0.00233,0.00233,0.002,0.002};
  Float_t WCompICP[5] = {93.82,95.34,95.33,92.14, 99.09};
  Float_t errorsWICP[5] = {0.06,0.06,0.06,0.06,0.06};
  /*
  Float_t SeCompICP[4] = {6.717,8.242,8.077,8.412};
  Float_t errorsSeICP[4] = {0.08,0.08,0.08,0.08};
  Float_t MoCompICP[4] = {0.006,0.008,0.011,0.006};
  Float_t errorsMoICP[4] = {0.00133,0.00133,0.00133,0.00133};
  */

  //  TString SampleName[4] = {"UIUC 1A                 ", "UIUC 1B         ", " UIUC 2", "           UIUC 3", "                        BNL"};
  TString SampleName[5] = {"THP 1 (UIUC)", "THP 2 (UIUC)", "THP 3 (UIUC)", "THP 4 (BNL)","Buffalo Tungsten"};

    //  TString SampleName2[5] = {"UIUC 1                                       ", "UIUC 2             ", "              UIUC 3", "                                            BNL"};
  
  
  Float_t XAxis[5] = {0,1,2,3,4};
  Float_t XAxis2[5] = {0,1,2,3,4};
  
    Float_t noErrors[4] = {0.0,0.0,0.0,0.0};
    Float_t noErrors2[4] = {0.0,0.0,0.0,0.0};

  TCanvas *canvasX = new TCanvas("cX","cX", 15,49,950,500);
  
  TMultiGraph *mgW = new TMultiGraph();

    TGraphErrors *NiICP = new TGraphErrors(5,  XAxis2, NiCompICP, noErrors2, errorsNiICP);
  TGraphErrors *CrICP = new TGraphErrors(5,  XAxis2, CrCompICP, noErrors2,  errorsCrICP);
      TGraphErrors *SiICP = new TGraphErrors(5,  XAxis2, SiCompICP, noErrors2, errorsSiICP);
  TGraphErrors *CoICP = new TGraphErrors(5,  XAxis2, CoCompICP, noErrors2,  errorsCoICP);
  TGraphErrors *WICP = new TGraphErrors(5,  XAxis2, WCompICP, noErrors2, errorsWICP);

  
  
  
  NiICP->SetMarkerStyle(22);
  CrICP->SetMarkerStyle(21);
  WICP->SetMarkerStyle(20);
  SiICP->SetMarkerStyle(24);
  CoICP->SetMarkerStyle(25);

  NiICP->SetMarkerSize(1.6);
  CrICP->SetMarkerSize(1.6);
  SiICP->SetMarkerSize(1.6);
  CoICP->SetMarkerSize(1.6);
  WICP->SetMarkerSize(1.6);
  NiICP->SetMarkerColor(2);
  CrICP->SetMarkerColor(13);
  SiICP->SetMarkerColor(1);
  CoICP->SetMarkerColor(8);


  
  TPaveText *det1 = new TPaveText(0.2, 0, .25, 0.08,"brNDC");
  TPaveText *det2 = new TPaveText(.36, 0, .39, .08,"brNDC");
  TPaveText *det3 = new TPaveText(.45, 0, .50, .08,"brNDC");
  TPaveText *det4 = new TPaveText(.6, 0, .65, .08,"brNDC");
  TPaveText *det5 = new TPaveText(.8, 0, .84, .08,"brNDC");


  det1->AddText(SampleName[0]);
  det2->AddText(SampleName[1]);
  det3->AddText(SampleName[2]);
  det4->AddText(SampleName[3]);
  det5->AddText(SampleName[4]);
  det1->SetShadowColor(0);
  det2->SetShadowColor(0);
  det3->SetShadowColor(0);
  det4->SetShadowColor(0);
    det5->SetShadowColor(0);
  det1->SetFillColor(0);
  det2->SetFillColor(0);
  det3->SetFillColor(0);
  det4->SetFillColor(0);
    det5->SetFillColor(0);
  det1->SetTextFont(12);
  det1->SetTextSize(.03);
  det2->SetTextFont(12);
  det2->SetTextSize(.03);
  det3->SetTextFont(12);
  det3->SetTextSize(.03);
  det4->SetTextFont(12);
  det4->SetTextSize(.03);
    det5->SetTextFont(12);
  det5->SetTextSize(.03);
  det2->SetTextFont(12);
  det1->SetLineColor(0);
  det2->SetLineColor(0);
  det3->SetLineColor(0);
  det4->SetLineColor(0);
    det5->SetLineColor(0);

  


  mgW->SetTitle("; ; Weighted Composition (%)"); 

  mgW->Add(WICP);
  mgW->Draw("ap");
det1->Draw();
  det2->Draw();
  det3->Draw();
  det4->Draw();
    det5->Draw();
  
    //  mgW->GetXaxis()->CenterTitle();
  //  mgW->GetXaxis()->SetLabelSize(0.045);
    //  mgW->GetYaxis()->CenterTitle();
  //  mgW->GetXaxis()->SetTitleOffset(1.2);


    TAxis *ax = mgW->GetHistogram()->GetXaxis();
  Double_t x1 = ax->GetBinLowEdge(1);
  Double_t x2 = ax->GetBinUpEdge(ax->GetNbins());
  mgW->GetHistogram()->GetXaxis()->Set(5,x1,x2);
  
  for (int i=0; i < 5; ++i) {
    mgW->GetHistogram()->GetXaxis()->SetBinLabel(i+1,"");
  }
  
  TLegend *legendX = new TLegend(0.46,0.60,0.70,0.90);
  legendX->SetTextFont(72);
  legendX->SetTextSize(0.04);
  legendX->SetFillColor(0);
  legendX->AddEntry(WICP, "ICP-MS", "pe");

  legendX->Draw();
  //  canvasX->SaveAs("WCompResults.root");

  TCanvas *canvasY = new TCanvas("cY","cY", 15,49,950,500);
  TMultiGraph *mgICP = new TMultiGraph();
  

  
  mgICP->SetTitle("; ; Weighted Composition (%)"); 

    mgICP->Add(NiICP);
  mgICP->Add(CrICP);
  mgICP->Add(CoICP);
  mgICP->Add(SiICP);
  mgICP->Draw("ap");

  det1->Draw();
  det2->Draw();
  det3->Draw();
  det4->Draw();
  det5->Draw();
  
  //mgICP->GetXaxis()->CenterTitle();
  //  mgICP->GetYaxis()->CenterTitle();
  //  mgICP->GetXaxis()->SetTitleOffset(1.2);


  
  TAxis *axXRF = mgICP->GetHistogram()->GetXaxis();
  Double_t x1XRF = axXRF->GetBinLowEdge(1);
  Double_t x2XRF = axXRF->GetBinUpEdge(axXRF->GetNbins());
  mgICP->GetHistogram()->GetXaxis()->Set(5,x1XRF,x2XRF);
  
  for (int i=0; i < 5; ++i) {
    mgICP->GetHistogram()->GetXaxis()->SetBinLabel(i+1,"");
  }

  
  TLegend *legendY = new TLegend(0.46,0.6,0.70,0.90);
  legendY->SetTextFont(72);
  legendY->SetTextSize(0.04);
  legendY->SetFillColor(0);
  

      legendY->AddEntry(CoICP, "Co", "pe");
  legendY->AddEntry(CrICP, "Cr", "pe");
      legendY->AddEntry(NiICP, "Ni", "pe");
      legendY->AddEntry(SiICP, "Si", "pe");

  legendY->Draw();
  canvasY->SaveAs("ICPCompResults.root");

  return 0;
}
