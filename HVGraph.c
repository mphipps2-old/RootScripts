#include <iostream>
void main() {
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);
  //gStyle->SetTickLength(0.02, "xyz");
  Float_t exclusiveX[8] = {140,140,129,118,105,97,97,99};
  Float_t errorsExX[8] = {3,3,3,2,2,1,1,1};
  Float_t inclusiveX[8] = {112,108,103,94,84,77,76,78};
  Float_t errorsInX[8] = {2,2,2,1,1,1,1,1};
  Float_t geoMeanX[8] = {125,123,116,105,94,87,86,87};
  Float_t errorsGeoX[8] = {4,4,3,2,2,1,1,1};
  TString planeNameX[8] = {"REF2X", "REF3X", "UVA3X", "REF1X"}; 
  Float_t XAxis[8] = {2900,2950,3000,3050,3100,3200,3250,3300};

  Float_t noErrors[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

  TCanvas *canvasX = new TCanvas("cX","cX", 15,49,950,500);
  
  TMultiGraph *mgX = new TMultiGraph();
  TGraphErrors *exclXResiduals = new TGraphErrors(8,  XAxis, exclusiveX, noErrors, errorsExX);
  TGraphErrors *inclXResiduals = new TGraphErrors(8,  XAxis, inclusiveX, noErrors,  errorsInX);
  TGraphErrors *geoMeanResidualsX = new TGraphErrors(8,  XAxis, geoMeanX, noErrors, errorsGeoX);
/*
  TAxis *ax = exclXResiduals->GetHistogram()->GetXaxis();
  Double_t x1 = exclXResiduals->GetBinLowEdge(1);
  Double_t x2 = exclXResiduals->GetBinUpEdge(ax->GetNbins());
  exclXResiduals->GetHistogram()->GetXaxis()->Set(4,x1,x2);
  inclXResiduals->GetHistogram()->GetXaxis()->Set(4,x1,x2);
  geoMeanResidualsX->GetHistogram()->GetXaxis()->Set(4,x1,x2);
*/


  exclXResiduals->SetMarkerStyle(22);
  inclXResiduals->SetMarkerStyle(21);
  geoMeanResidualsX->SetMarkerStyle(20);
  exclXResiduals->SetMarkerSize(1);
  inclXResiduals->SetMarkerSize(1);
  geoMeanResidualsX->SetMarkerSize(1);
  geoMeanResidualsX->SetMarkerColor(4);

  TPaveText *det1 = new TPaveText(.12, 0, .16, .08);
  TPaveText *det2 = new TPaveText(.36, 0, .39, .08);
  TPaveText *det3 = new TPaveText(.75, 0, .79, .08);
  TPaveText *det4 = new TPaveText(.85, 0, .89, .08);
 // det1->AddText("Tracker 1");
 // det2->AddText("Tracker 2");
 // det3->AddText("Tracker 3");
 // det4->AddText("Tracker 4");

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


  mgX->SetTitle("Resolution vs. HV; HV (V); Sigma of Residual Distribution (#murad)"); 

  mgX->Add(exclXResiduals);
  mgX->Add(inclXResiduals);
  mgX->Add(geoMeanResidualsX);



  mgX->Draw("apl");
/*
  int binNb = 0;
  for (int k = 0; k <4 ; ++k) {
    if (k == 0) binNb = 5;
    else if (k==1) binNb = 35;
    else if (k==2) binNb = 65;
    else binNb = 96;
    mgX->GetHistogram()->GetXaxis()->SetBinLabel(binNb, planeNameX[k].Data());
  }
*/


  mgX->GetXaxis()->CenterTitle();
  mgX->GetYaxis()->CenterTitle();
  mgX->GetXaxis()->SetTitleOffset(1.2);



  TLegend *legendX = new TLegend(0.46,0.70,0.70,0.90);
  legendX->SetTextFont(72);
  legendX->SetTextSize(0.04);
  legendX->SetFillColor(0);
  legendX->AddEntry(exclXResiduals, "Exclusive Residual", "pe");
  legendX->AddEntry(inclXResiduals, "Inclusive Residual", "pe");
  legendX->AddEntry(geoMeanResidualsX, "Geometric Mean", "pe");
  legendX->Draw();
  canvasX->SaveAs("XResiduals.root");

  
}
