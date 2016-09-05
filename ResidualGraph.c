#include <iostream>
void main() {
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);
  //gStyle->SetTickLength(0.02, "xyz");
  Float_t exclusiveX[4] = {168,98,89,137};
  Float_t errorsExX[4] = {1,0,0,1 };
  Float_t inclusiveX[4] = {46,69,55,59};
  Float_t errorsInX[4] = {0,0,0,0};
  Float_t geoMeanX[4] = {80,82,70,90};
  Float_t errorsGeoX[4] = {0,1,0,1};
  TString planeNameX[4] = {"REF2X", "REF3X", "UVA3X", "REF1X"}; 
  Float_t XAxis[4] = {0,114.35,308.65,356.95};
  Float_t exclusiveY[4] = {75,44,38,56};
  Float_t errorsExY[4] = {0,0,0,0 };
  Float_t inclusiveY[4] = {21,31,23,25};
  Float_t errorsInY[4] = {0,0,0,0 };
  Float_t geoMeanY[4] = { 39,37,30,38};
  Float_t errorsGeoY[4] = {0,0,0,0};
  TString planeNameY[4] = {"REF2Y", "REF3Y", "UVA3Y", "REF1Y"}; 
  Float_t noErrors[4] = {0.0,0.0,0.0,0.0};

  TCanvas *canvasX = new TCanvas("cX","cX", 15,49,950,500);
  
  TMultiGraph *mgX = new TMultiGraph();
  TGraphErrors *exclXResiduals = new TGraphErrors(4,  XAxis, exclusiveX, noErrors, errorsExX);
  TGraphErrors *inclXResiduals = new TGraphErrors(4,  XAxis, inclusiveX, noErrors,  errorsInX);
  TGraphErrors *geoMeanResidualsX = new TGraphErrors(4,  XAxis, geoMeanX, noErrors, errorsGeoX);
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


  mgX->SetTitle("Reference Detector Resolution: r plane; Detector Position in Z (cm); Sigma of Residual Distribution (#mum)"); 

  mgX->Add(exclXResiduals);
  mgX->Add(inclXResiduals);
  mgX->Add(geoMeanResidualsX);



  mgX->Draw("ap");
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

  TCanvas *canvasY = new TCanvas("cY","cY", 15,49,950,500);
  TMultiGraph *mgY = new TMultiGraph();
  TGraphErrors *exclYResiduals = new TGraphErrors(4,  XAxis, exclusiveY,noErrors, errorsExY);
  TGraphErrors *inclYResiduals = new TGraphErrors(4,  XAxis, inclusiveY, noErrors, errorsInY);
  TGraphErrors *geoMeanResidualsY = new TGraphErrors(4,  XAxis, geoMeanY, noErrors, errorsGeoY);
  exclYResiduals->SetMarkerStyle(22);
  inclYResiduals->SetMarkerStyle(21);
  geoMeanResidualsY->SetMarkerStyle(20);
  exclYResiduals->SetMarkerSize(1);
  inclYResiduals->SetMarkerSize(1);
  geoMeanResidualsY->SetMarkerSize(1);
  geoMeanResidualsY->SetMarkerColor(4);

  det1->Draw();
  det2->Draw();
  det3->Draw();
  det4->Draw();
  mgY->SetTitle("Reference Detector Resolution: #phi Plane; Detector Position along the Beamline (cm); Sigma of Residual Distribution (#murad)"); 

  mgY->Add(exclYResiduals);
  mgY->Add(inclYResiduals);
  mgY->Add(geoMeanResidualsY);

  mgY->Draw("ap");

/*
  for (int k = 0; k <4 ; ++k) {
    if (k == 0) binNb = 5;
    else if (k==1) binNb = 34;
    else if (k==2) binNb = 84;
    else binNb = 96;
    mgY->GetHistogram()->GetXaxis()->SetBinLabel(binNb, planeNameY[k].Data());
  }
*/
  mgY->GetXaxis()->CenterTitle();
  mgY->GetYaxis()->CenterTitle();
  mgY->GetXaxis()->SetTitleOffset(1.2);

  TLegend *legendY = new TLegend(0.46,0.70,0.70,0.90);
  legendY->SetTextFont(72);
  legendY->SetTextSize(0.04);
  legendY->SetFillColor(0);
  legendY->AddEntry(exclXResiduals, "Exclusive Residual", "pe");
  legendY->AddEntry(inclXResiduals, "Inclusive Residual", "pe");
  legendY->AddEntry(geoMeanResidualsX, "Geometric Mean", "pe");
  legendY->Draw();
  canvasY->SaveAs("YResiduals.root");
}
