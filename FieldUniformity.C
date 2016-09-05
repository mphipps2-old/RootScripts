#include <iostream>
void main() {
  TString file = "file.root";
  int n = 15;
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);
  //gStyle->SetTickLength(0.02, "xyz");

  //READ IN TEXT FILE


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

  TCanvas *canvas = new TCanvas("c","c", 15,49,950,500);
  
  TMultiGraph *mg = new TGraph();
  TGraph *y0 = new TGraph(n, x1, value1);
  TGraph *y25 = new TGraph(n, x2, value2);
  TGraph *y12 = new TGraph(n, x3, value3);
  TGraph *y35 = new TGraph(n,x4, value4);
  TGraph *y = new TGraph(n,x5, value5);


  y0->SetMarkerStyle(22);
  y25->SetMarkerStyle(21);
  y12->SetMarkerStyle(20);
  y0->SetMarkerSize(1);
  y25->SetMarkerSize(1);
  y23->SetMarkerSize(1);
  y0->SetMarkerColor(4);




  mg->SetTitle("Electrostatic Uniformity of Drift Region; Drift Region in X (cm); E/E_{0}"); 

  mg->Add(y0);
  mg->Add(y25);
  mg->Add(y12);
  mg->Add(y35);
  mg->Add(y1);


  mg->Draw("ap");

  TLegend *legend = new TLegend(0.46,0.70,0.70,0.90);
  legend->SetTextFont(72);
  legend->SetTextSize(0.04);
  legend->SetFillColor(0);
  legend->AddEntry(exclXResiduals, "Y = 0", "pe");
  legend->AddEntry(inclXResiduals, "Y = (2*height)/2", "pe");
  legend->AddEntry(inclXResiduals, "Y = height/2", "pe");
  legend->AddEntry(inclXResiduals, "Y = (3*height)/5", "pe");
  legend->AddEntry(geoMeanResidualsX, "Y = height", "pe");
  legend->Draw();
  canvas->SaveAs(file);
}
