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
  Float_t FeCompEDS[4] = {0.3,0.4,0.7,0.5};
  Float_t errorsFeEDS[4] = {0.025,0.025,0.035,0.03};
  Float_t CuCompEDS[4] = {0.2,0.3,0.3,0.3};
  Float_t errorsCuEDS[4] = {0.055,0.055,0.055,0.055};
  Float_t SeCompEDS[4] = {3.8,3.3,3.5,3.2};
  Float_t errorsSeEDS[4] = {0.015,0.01,0.01,0.03};
  Float_t MoCompEDS[4] = {0.7,0.7,0.7,0.3};
  Float_t errorsMoEDS[4] = {0.03,0.015,0.035,0.02};
  Float_t WCompEDS[4] = {95.0,95.3,94.8,95.7};
  Float_t errorsWEDS[4] = {0.005,0.01,0.01,0.01};

  Float_t FeCompXRF[4] = {0.143,0.296,0.233,0.059};
  Float_t errorsFeXRF[4] = {0.0024,0.0017,0.0027,0.002};
  Float_t CrCompXRF[4] = {0.043,0.076,0.062,0.005};
  Float_t errorsCrXRF[4] = {0.002,0.00233,0.00233,0.002};
  Float_t SeCompXRF[4] = {6.717,8.242,8.077,8.412};
  Float_t errorsSeXRF[4] = {0.08,0.08,0.08,0.08};
  Float_t MoCompXRF[4] = {0.006,0.008,0.011,0.006};
  Float_t errorsMoXRF[4] = {0.00133,0.00133,0.00133,0.00133};
  Float_t WCompXRF[4] = {93.090,91.378,91.618,91.519};
  Float_t errorsWXRF[4] = {0.06,0.06,0.06,0.06};

  Float_t NiCompICP[4] = {0.403,0.44,0.37,0.038};
  Float_t errorsNiICP[4] = {0.0024,0.0017,0.0027,0.002};
  Float_t CrCompICP[4] = {0.376,0.416,0.351,0.318};
  Float_t errorsCrICP[4] = {0.002,0.00233,0.00233,0.002};
  Float_t WCompICP[4] = {93.82,95.34,95.33,92.14};
  Float_t errorsWICP[4] = {0.06,0.06,0.06,0.06};
  /*
  Float_t SeCompICP[4] = {6.717,8.242,8.077,8.412};
  Float_t errorsSeICP[4] = {0.08,0.08,0.08,0.08};
  Float_t MoCompICP[4] = {0.006,0.008,0.011,0.006};
  Float_t errorsMoICP[4] = {0.00133,0.00133,0.00133,0.00133};
  */

  //  TString SampleName[4] = {"UIUC 1A                 ", "UIUC 1B         ", " UIUC 2", "           UIUC 3", "                        BNL"};
    TString SampleName[4] = {"UIUC 1                                       ", "UIUC 2             ", "              UIUC 3", "                                            BNL"};

  TString SampleName2[4] = {"UIUC 1                                       ", "UIUC 2             ", "              UIUC 3", "                                            BNL"};
  
  
  Float_t XAxis[4] = {0,1,2,3};
    Float_t XAxis2[4] = {0,1,2,3};
  
    Float_t noErrors[4] = {0.0,0.0,0.0,0.0};
    Float_t noErrors2[4] = {0.0,0.0,0.0,0.0};

  TCanvas *canvasX = new TCanvas("cX","cX", 15,49,950,500);
  
  TMultiGraph *mgW = new TMultiGraph();
  TGraphErrors *FeEDS = new TGraphErrors(4,  XAxis, FeCompEDS, noErrors, errorsFeEDS);
  TGraphErrors *CuEDS = new TGraphErrors(4,  XAxis, CuCompEDS, noErrors,  errorsCuEDS);
  TGraphErrors *SeEDS = new TGraphErrors(4,  XAxis, SeCompEDS, noErrors, errorsSeEDS);
  TGraphErrors *MoEDS = new TGraphErrors(4,  XAxis, MoCompEDS, noErrors,  errorsMoEDS);
  TGraphErrors *WEDS = new TGraphErrors(4,  XAxis, WCompEDS, noErrors, errorsWEDS);

    TGraphErrors *NiICP = new TGraphErrors(4,  XAxis2, NiCompICP, noErrors2, errorsNiICP);
  TGraphErrors *CrICP = new TGraphErrors(4,  XAxis2, CrCompICP, noErrors2,  errorsCrICP);
  TGraphErrors *WICP = new TGraphErrors(4,  XAxis2, WCompICP, noErrors2, errorsWICP);

  
  TGraphErrors *FeXRF = new TGraphErrors(4,  XAxis2, FeCompXRF, noErrors2, errorsFeXRF);
  TGraphErrors *CrXRF = new TGraphErrors(4,  XAxis2, CrCompXRF, noErrors2,  errorsCrXRF);
  TGraphErrors *SeXRF = new TGraphErrors(4,  XAxis2, SeCompXRF, noErrors2, errorsSeXRF);
  TGraphErrors *MoXRF = new TGraphErrors(4,  XAxis2, MoCompXRF, noErrors2,  errorsMoXRF);
  TGraphErrors *WXRF = new TGraphErrors(4,  XAxis2, WCompXRF, noErrors2, errorsWXRF);
  
  FeXRF->SetMarkerStyle(22);
  CrXRF->SetMarkerStyle(21);
  SeXRF->SetMarkerStyle(20);
  MoXRF->SetMarkerStyle(23);
  WXRF->SetMarkerStyle(20);
  FeXRF->SetMarkerSize(1.6);
  CrXRF->SetMarkerSize(1.6);
  SeXRF->SetMarkerSize(1.6);
  MoXRF->SetMarkerSize(1.6);
  WXRF->SetMarkerSize(1.6);
  SeXRF->SetMarkerColor(4);
  FeXRF->SetMarkerColor(2);
  MoXRF->SetMarkerColor(8);
  CrXRF->SetMarkerColor(13);

  
  
  NiICP->SetMarkerStyle(22);
  CrICP->SetMarkerStyle(21);
  WICP->SetMarkerStyle(24);
  NiICP->SetMarkerSize(1.6);
  CrICP->SetMarkerSize(1.6);
  WICP->SetMarkerSize(1.6);
  NiICP->SetMarkerColor(2);
  CrICP->SetMarkerColor(13);



  FeEDS->SetMarkerStyle(22);
  CuEDS->SetMarkerStyle(21);
  SeEDS->SetMarkerStyle(20);
  MoEDS->SetMarkerStyle(23);
  WEDS->SetMarkerStyle(23);
  
  FeEDS->SetMarkerSize(1.6);
  CuEDS->SetMarkerSize(1.6);
  SeEDS->SetMarkerSize(1.6);
  MoEDS->SetMarkerSize(1.6);
  WEDS->SetMarkerSize(1.6);
  SeEDS->SetMarkerColor(4);
  FeEDS->SetMarkerColor(2);
  MoEDS->SetMarkerColor(8);
  CuEDS->SetMarkerColor(13);


  
  TPaveText *det1 = new TPaveText(0.5, 0, .5, 0.8);
  TPaveText *det2 = new TPaveText(.36, 0, .39, .08);
  TPaveText *det3 = new TPaveText(.75, 0, .79, .08);
  TPaveText *det4 = new TPaveText(.85, 0, .89, .08);

  det1->SetShadowColor(0);
  det2->SetShadowColor(0);
  det3->SetShadowColor(0);
  det4->SetShadowColor(0);
  det1->SetFillColor(0);
  det2->SetFillColor(0);
  det3->SetFillColor(0);
  det4->SetFillColor(0);
  det1->SetTextFont(12);
  det1->SetTextSize(.03);
  det2->SetTextFont(12);
  det2->SetTextSize(.03);
  det3->SetTextFont(12);
  det3->SetTextSize(.03);
  det4->SetTextFont(12);
  det4->SetTextSize(.03);
  det1->SetLineColor(0);
  det2->SetLineColor(0);
  det3->SetLineColor(0);
  det4->SetLineColor(0);

  det1->Draw();
  det2->Draw();
  det3->Draw();
  det4->Draw();


  mgW->SetTitle("; Sample; Weighted Composition (%)"); 

  mgW->Add(WEDS);
  mgW->Add(WXRF);
  mgW->Add(WICP);


  mgW->Draw("ap");

  mgW->GetXaxis()->CenterTitle();
  //  mgW->GetXaxis()->SetLabelSize(0.045);
  mgW->GetYaxis()->CenterTitle();
  //  mgW->GetXaxis()->SetTitleOffset(1.2);


    TAxis *ax = mgW->GetHistogram()->GetXaxis();
  Double_t x1 = ax->GetBinLowEdge(1);
  Double_t x2 = ax->GetBinUpEdge(ax->GetNbins());
  mgW->GetHistogram()->GetXaxis()->Set(4,x1,x2);
  
  for (int i=0; i < 4; ++i) {
    mgW->GetHistogram()->GetXaxis()->SetBinLabel(i+1,SampleName[i].Data());
  }
  
  TLegend *legendX = new TLegend(0.46,0.60,0.70,0.90);
  legendX->SetTextFont(72);
  legendX->SetTextSize(0.04);
  legendX->SetFillColor(0);
  legendX->AddEntry(WEDS, "SEM-EDS", "pe");
  legendX->AddEntry(WXRF, "EDXRF", "pe");
  legendX->AddEntry(WICP, "ICP-MS", "pe");

  legendX->Draw();
  //  canvasX->SaveAs("WCompResults.root");

  TCanvas *canvasY = new TCanvas("cY","cY", 15,49,950,500);
  TMultiGraph *mgICP = new TMultiGraph();
  
  det1->Draw();
  det2->Draw();
  det3->Draw();
  det4->Draw();
  mgICP->SetTitle("; Sample; Weighted Composition (%)"); 

    mgICP->Add(NiICP);
  mgICP->Add(CrICP);

  mgICP->Draw("ap");


  mgICP->GetXaxis()->CenterTitle();
  mgICP->GetYaxis()->CenterTitle();
  //  mgICP->GetXaxis()->SetTitleOffset(1.2);


  
  TAxis *axXRF = mgICP->GetHistogram()->GetXaxis();
  Double_t x1XRF = axXRF->GetBinLowEdge(1);
  Double_t x2XRF = axXRF->GetBinUpEdge(axXRF->GetNbins());
  mgICP->GetHistogram()->GetXaxis()->Set(4,x1XRF,x2XRF);
  
  for (int i=0; i < 4; ++i) {
    mgICP->GetHistogram()->GetXaxis()->SetBinLabel(i+1,SampleName2[i].Data());
  }

  
  TLegend *legendY = new TLegend(0.46,0.6,0.70,0.90);
  legendY->SetTextFont(72);
  legendY->SetTextSize(0.04);
  legendY->SetFillColor(0);
  
    legendY->AddEntry(NiICP, "Ni", "pe");
  legendY->AddEntry(CrICP, "Cr", "pe");
  legendY->Draw();
  canvasY->SaveAs("ICPCompResults.root");

  return 0;
}
