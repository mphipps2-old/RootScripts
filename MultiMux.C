// To run script, first compile with: g++ `root-config --cflags` srsCalib.C `root-config --glibs`. Then execute with ./a.out

#include <iostream>
#include <fstream>
#include <vector>

#include "TGraphErrors.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TBranch.h"
#include "TTree.h"
#include "TFile.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPad.h"
//#include "TGAxis.h"
using namespace std;


int main() {

  TMultiGraph *mg = new TMultiGraph();
  // TMultiGraph *mg2 = new TMultiGraph();

// TFile *file = TFile::Open("ExtPulserMux2/Mux2Calibration.root");
  // if (!file) return 1;
  // TFile *file2 = TFile::Open("ExtPulserMux4/Mux4Calibration.root");
  // if (!file2) return 1;
/*
 ofstream file1b;
 file1b.open("../IntValuesMux2.txt", ofstream::out | ofstream::trunc);
 file1b << "#Input,     ADC#" << endl;
 file1b << "######################## " << endl << endl;

ofstream file2b;
 file2b.open("IntValuesMux4.txt", ofstream::out | ofstream::trunc);
 file2b << "#Input,     ADC#" << endl;
 file2b << "######################## " << endl << endl;
*/
 
 Int_t N2; 
 Int_t N4;
 Double_t *X2;
 Double_t *Y2;
 
 Double_t *X4;
 Double_t *Y4;

   TFile *file = TFile::Open("/home/mphipps/rcdaqAnalysis/IntMux1Scan/Mux1Imux01CalibrationReverse.root");
   if (!file) return 1;
   TFile *file2 = TFile::Open("/home/mphipps/rcdaqAnalysis/IntMux4/Mux4Imux15Calibration.root");
    
   if (!file2) return 1;
  cout << " 1 " << endl;
   TCanvas *c1 = (TCanvas*) file->Get("c2;1");
  TCanvas *c2 = (TCanvas*) file2->Get("c2;1");
  // TCanvas *c3 = (TCanvas*) file->Get("c2;1");
  // TCanvas *c4 = (TCanvas*) file2->Get("c2;1");
cout << " 1.5 " << endl;
  TGraphErrors *g1 = (TGraphErrors*) c1->GetPrimitive("Graph");
  TGraphErrors *g2 = (TGraphErrors*) c2->GetPrimitive("Graph");
  X2 = g1->GetX();
  Y2 = g1->GetY();
  N2= g1->GetN();
  X4 = g2->GetX();
  Y4 = g2->GetY();
  N4= g2->GetN();

  

  //TF1 *f1 = new TF1("f1","pol1",200,500);
  //g1->Fit(f1,"R");
  //  float slope = (Y2[25]-Y2[0])/(X2[25]-X2[0]);
  //float yInt = Y2[20]-(X2[20]*slope);
  
  //float slope = 162.345  ;  // slope of the nonlin part
  //  float yInt =  -47888.2  ;  // intercept of the nonlin part
  float slopeLin = 87.5636 ; // slope of the linear part: measure of pedestal
  float yIntLin = -1392.03 ; // intercept of the linear part
  for (int i = 0; i < N2; ++i) {
  //X4[i] = (slope * X4[i]) + yInt;
    float linInt = (Y2[i]/slopeLin) - (yIntLin/slopeLin);
    Y2[i] = (Y2[i]/slopeLin) - (yIntLin/slopeLin); // convert from electrons to uncorrected ADC
    cout << "x " << X2[i] << " y " <<  Y2[i] << endl;
    //    Y2[i] += linInt; // set the corrected scale by aligning y intercept for linear and nonlinear parts
  }

  //  delete f1;
  /*
  for (int i=0; i<N2; ++i) {
    file1b << X2[i] << "        " << Y2[i] << endl;
  }

  for (int i=0; i<N4; ++i) {
    file2b << X2[i] << "        " << Y4[i] << endl;
  }
  */
    g1->SetMarkerStyle(20);
  g1->SetMarkerColor(4);
  g1->SetLineColor(4);
 g1->SetMarkerStyle(21);
  g2->SetMarkerColor(2);
 g2->SetLineColor(2);
  // TGraphErrors *g3 = (TGraphErrors*) c4->GetPrimitive("Graph");
  // TGraphErrors *g4 = (TGraphErrors*) c4->GetPrimitive("Graph");
  TCanvas *c = new TCanvas("c","c");
  /*
   TPad *pad1 = new TPad("pad1","",0,0,1,1);
   TPad *pad2 = new TPad("pad2","",0,0,1,1);
   pad2->SetFillStyle(4000); //will be transparent
   pad1->Draw();
   pad1->cd();
cout << " 2 " << endl;
  */
  g1->Draw("apl");
  // mg->Add(g1);
  //mg->Add(g2);
  
  // mg->Draw("apl");
  //    mg->SetTitle("SRS Calibration: Internal Pulser; Corrected ADC; Avg Pedestal-Subtracted ADC")
  g1->SetTitle("SRS Calibration: Internal Pulser; Avg Pedestal-Subtracted ADC; Corrected ADC");

  // mg->SetTitle("SRS Calibration: Internal Pulser; N_{electrons}; Avg ADC");

  //mg->SetTitle("SRS Calibration: Internal Pulser; Input Pulse (mV); Avg ADC");
 c->Update();
  g1->GetXaxis()->CenterTitle();
  g1->GetYaxis()->CenterTitle();
  /* TLegend *leg = new TLegend(0.46,0.7,0.7,0.9);
  leg->SetTextFont(72);
  leg->SetTextSize(0.04);
  leg->SetFillColor(0);
  leg->AddEntry(g1,"Muxgain 1; Imuxin 0x20","ple");
  leg->AddEntry(g2,"Muxgain 5; Imuxin 0x46","ple");
  leg->Draw();
  c->cd();*/
 /*
   //compute the pad range with suitable margins
   Double_t ymin = 0;
   Double_t ymax = 2000;
   Double_t dy = (ymax-ymin)/0.8; //10 per cent margins top and bottom
   Double_t xmin = -3;
   Double_t xmax = 3;
   Double_t dx = (xmax-xmin)/0.8; //10 per cent margins left and right
   pad2->Range(xmin-0.1*dx,ymin-0.1*dy,xmax+0.1*dx,ymax+0.1*dy);
   pad2->Draw();
   pad2->cd();
  //mg2->SetTitle("SRS Calibration; N_{electrons}; Avg ADC");
  mg2->Add(g3);
  mg2->Add(g4);
  mg2->Draw("][same");
 mg->SetTitle("SRS Calibration; N_{electrons}; Avg ADC");
 c->Update();

  Double_t low = 50;
  Double_t high = 950;
TGaxis *axis = new TGaxis(gPad->GetUxmin(),gPad->GetUymax(),gPad->GetUxmax(),gPad->GetUymax(),low,high,510,"+");
axis->Draw();

  TLegend *leg2 = new TLegend(0.12,0.45,0.7,0.9);
  leg2->SetTextFont(72);
  leg2->SetTextSize(0.04);
  leg2->SetFillColor(0);
  leg2->AddEntry(g3,"Mux 2","ple");
  leg2->AddEntry(g4,"Mux 4","ple");
  leg2->Draw();
*/
  char mgName[256];
  sprintf(mgName,"../IntMux1Scan/correctedMux1CurveReverseNew.root");
  c->SaveAs(mgName); 

  //  file1b.close();
  //file2b.close();
  return 0;
}
